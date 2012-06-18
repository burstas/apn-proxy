#include "ApnProxyApp.h"
#include "CwxDate.h"

///构造函数
ApnProxyApp::ApnProxyApp(){
    m_proxyHandler = NULL;
}

///析构函数
ApnProxyApp::~ApnProxyApp(){
}

///初始化
int ApnProxyApp::init(int argc, char** argv){
    string strErrMsg;
    ///首先调用架构的init api
    if (CwxAppFramework::init(argc, argv) == -1) return -1;
    ///检查是否通过-f指定了配置文件，若没有，则采用默认的配置文件
    if ((NULL == this->getConfFile()) || (strlen(this->getConfFile()) == 0)){
        this->setConfFile("apn_proxy.cnf");
    }
    ///加载配置文件，若失败则退出
    if (0 != m_config.loadConfig(getConfFile())){
        CWX_ERROR((m_config.getError()));
        return -1;
    }
    ///设置运行日志的输出level
    setLogLevel(CwxLogger::LEVEL_ERROR|CwxLogger::LEVEL_INFO|CwxLogger::LEVEL_WARNING);
    return 0;
}

///配置运行环境信息
int ApnProxyApp::initRunEnv(){
    ///设置系统的时钟间隔，最小刻度为1ms，此为1s。
    this->setClick(100);//0.1s
    ///设置工作目录
    this->setWorkDir(this->m_config.m_strWorkDir.c_str());
    ///设置循环运行日志的数量
    this->setLogFileNum(LOG_FILE_NUM);
    ///设置每个日志文件的大小
    this->setLogFileSize(LOG_FILE_SIZE*1024*1024);
    ///调用架构的initRunEnv，使以上设置的参数生效
    if (CwxAppFramework::initRunEnv() == -1 ) return -1;
    blockSignal(SIGPIPE);
    //set version
    this->setAppVersion(APN_PROXY_APP_VERSION);
    //set last modify date
    this->setLastModifyDatetime(APN_PROXY_APP_MODIFY_DATE);
    //set compile date
    this->setLastCompileDatetime(CWX_COMPILE_DATE(_BUILD_DATE));

    ///将加载的配置文件信息输出到日志文件中，以供查看检查
    m_config.outputConfig();

    ///初始化tss
    m_tss.init(NULL, NULL);
    
    ///注册proxy请求的处理handle
    m_proxyHandler = new ApnProxyHandler(this);         
    this->getCommander().regHandle(SVR_TYPE_APN, m_proxyHandler);

    ///监听TCP连接，其建立的连接的svr-id都为SVR_TYPE_APN，接收的消息的svr-id都为SVR_TYPE_APN。
    ///全部由m_proxyHandler对象来处理
    if (0 > this->noticeTcpListen(SVR_TYPE_APN, 
        this->m_config.m_listen.getHostName().c_str(),
        this->m_config.m_listen.getPort(),
        false,
        CWX_APP_MSG_MODE))
    {
        CWX_ERROR(("Can't register the proxy acceptor port: addr=%s, port=%d",
            this->m_config.m_listen.getHostName().c_str(),
            this->m_config.m_listen.getPort()));
        return -1;
    }
    ///创建线程池对象，此线程池中线程的group-id为2
    CWX_UINT16 i=0;
    CWX_UINT16 uiThreadId = 2;
    CwxThreadPool* pThreadPool = NULL;
    map<string, ApnProxyConfigChannel*>::iterator iter = m_config.m_channels.begin();
    while(iter != m_config.m_channels.end()){
        pThreadPool = new CwxThreadPool(uiThreadId++, 
            iter->second->m_unThreadNum,
            getThreadPoolMgr(),
            &getCommander());

        ///启动线程
        ApnProxyTss**pTss = new ApnProxyTss*[iter->second->m_unThreadNum];
        for (i=0; i<iter->second->m_unThreadNum; i++){
            pTss[i] = new ApnProxyTss();
            pTss[i]->init(iter->second, &m_config.m_channelApps);
        }
        m_threadPools[iter->first] = pair<CwxThreadPool*, ApnProxyTss**>(pThreadPool, pTss);

        ///启动线程，线程池中线程的线程栈大小为1M。
        if ( 0 != pThreadPool->start((CwxTss**)pTss)){
            CWX_ERROR(("Failure to start thread pool"));
            return -1;
        }
        iter++;
    }

    return 0;

}

///时钟函数，什么也没有做
void ApnProxyApp::onTime(CwxTimeValue const& current){
    CwxAppFramework::onTime(current);
}

///信号处理函数
void ApnProxyApp::onSignal(int signum){
    switch(signum){
    case SIGQUIT: 
        ///若监控进程通知退出，则推出
        CWX_INFO(("Recv exit signal , exit right now."));
        this->stop();
        break;
    default:
        ///其他信号，全部忽略
        CWX_INFO(("Recv signal=%d, ignore it.", signum));
        break;
    }

}

///proxy请求的请求消息
int ApnProxyApp::onRecvMsg(CwxMsgBlock* msg, CwxAppHandler4Msg& conn, CwxMsgHead const& header, bool& ){
    if (!msg) return -1;
    msg->event().setSvrId(conn.getConnInfo().getSvrId());
    msg->event().setHostId(conn.getConnInfo().getHostId());
    msg->event().setConnId(conn.getConnInfo().getConnId());
    msg->event().setMsgHeader(header);
    msg->event().setEvent(CwxEventInfo::RECV_MSG);
    switch(header.getMsgType()){
        case APN_MSG_TYPE_NOTICE:
            return recvNoticeMsg(msg);
        case APN_MSG_TYPE_CHANNEL_INFO:
            return recvQueryChannelMsg(msg);
        case APN_MSG_TYPE_APP_INFO:
            return recvQueryAppMsg(msg);
        case APN_MSG_TYPE_THREAD_INFO:
            return recvQueryThreadMsg(msg);
    }

    CwxCommon::snprintf(m_tss.m_szBuf2K, 2048, "Unknown msg type:%u", header.getMsgType());
    replyMsg(this,
        conn.getConnInfo().getConnId(),
        header.getMsgType() + 1,
        header.getTaskId(),
        true,
        APN_PROXY_ERR_UNKNOWN_MSG,
        m_tss.m_szBuf2K,
        NULL,
        0);
    return 0;
}


void ApnProxyApp::destroy()
{
    map<string, pair<CwxThreadPool*, ApnProxyTss**> >::iterator iter =  m_threadPools.begin();
    while(iter != m_threadPools.end()){
        iter->second.first->stop();
        delete iter->second.first;
        iter++;
    }
    if (m_proxyHandler){
        delete m_proxyHandler;
        m_proxyHandler = NULL;
    }
    CwxAppFramework::destroy();
}

///收到notice消息；返回值：0，成功；-1：失败
int  ApnProxyApp::recvNoticeMsg(CwxMsgBlock* msg){
    int ret = APN_PROXY_ERR_SUCCESS;
    char const* szErrMsg = NULL;
    CwxKeyValueItem const* app = NULL;
    CwxKeyValueItem const* channel = NULL;
    do{
        if (!m_tss.m_pReader->unpack(msg->rd_ptr(), msg->length(), false, true)){
            ret = APN_PROXY_ERR_INVALID_PACKAGE;
            szErrMsg = m_tss.m_pReader->getErrMsg();
            break;
        }
        ///获取app
        app = m_tss.m_pReader->getKey(APN_PROXY_KEY_APP, false);
        if (!app){
            ret = APN_PROXY_ERR_MISSING_APP;
            szErrMsg = "Missing [app] parameter";
            break;
        }
        ///获取channel
        channel = m_tss.m_pReader->getKey(APN_PROXY_KEY_CH, false);
        if (!channel){
            ret = APN_PROXY_ERR_MISSING_CHANNEL;
            szErrMsg = "Missing [ch] parameter";
            break;
        }
        ///检查app是否存在
        if (m_config.m_apps.find(string(app->m_szData)) == m_config.m_apps.end()){
            ret = APN_PROXY_ERR_NO_APP;
            szErrMsg = "The app doesn't exist.";
            break;
        }
        ///检查channel是否存在
        if (m_threadPools.find(string(channel->m_szData)) == m_threadPools.end()){
            ret = APN_PROXY_ERR_NO_CHANNEL;
            szErrMsg = "The channel doesn't exist.";
            break;
        }
        CwxThreadPool* pool= m_threadPools.find(string(channel->m_szData))->second.first;
        pool->append(msg);
        return 0;
    }while(0);

    replyMsg(this,
        msg->event().getConnId(),
        msg->event().getMsgHeader().getMsgType()+1,
        msg->event().getMsgHeader().getTaskId(),
        false,
        ret,
        szErrMsg,
        NULL,
        0);
    CwxMsgBlockAlloc::free(msg);
    return 0;
}
///收到channel查询消息；返回值：0，成功；-1：失败
int  ApnProxyApp::recvQueryChannelMsg(CwxMsgBlock* msg){
    int ret = APN_PROXY_ERR_SUCCESS;
    char const* szErrMsg = NULL;
    char* szResult = NULL;
    CwxKeyValueItem const* channel = NULL;
    CwxThreadPool* pool=NULL;
    ApnProxyConfigChannel* ch=NULL;
    do{
        if (!m_tss.m_pReader->unpack(msg->rd_ptr(), msg->length(), false, true)){
            ret = APN_PROXY_ERR_INVALID_PACKAGE;
            szErrMsg = m_tss.m_pReader->getErrMsg();
            break;
        }
        ///获取channel
        channel = m_tss.m_pReader->getKey(APN_PROXY_KEY_CH, false);
        if (channel){
            ///检查channel是否存在
            if (m_threadPools.find(string(channel->m_szData)) == m_threadPools.end()){
                ret = APN_PROXY_ERR_NO_CHANNEL;
                szErrMsg = "The channel doesn't exist.";
                break;
            }
            pool= m_threadPools.find(string(channel->m_szData))->second.first;
            ch = m_config.m_channels.find(string(channel->m_szData))->second;
            CwxCommon::snprintf(m_szBuf,
                APN_MAX_REPLY_BUF_SIZE -1,
                "%s:%u:%s:%u:%s",
                ch->m_strChannelName.c_str(),
                ch->m_unThreadNum,
                ch->m_bRelease?APN_PROXY_CHANNEL_TYPE_RELEASE:APN_PROXY_CHANNEL_TYPE_DEV,
                pool->getQueuedMsgNum(),
                ch->m_strApps.c_str());
            szResult = m_szBuf;
        }else{
            map<string, pair<CwxThreadPool*, ApnProxyTss**> >::iterator iter = m_threadPools.begin();
            szResult = m_szBuf;
            while(iter != m_threadPools.end()){
                pool = iter->second.first;
                ch = m_config.m_channels.find(iter->first)->second;
                if (szResult != m_szBuf) {
                    szResult[0] = '\n';
                    szResult++;
                }
                CwxCommon::snprintf(szResult,
                    APN_MAX_REPLY_BUF_SIZE -(szResult - m_szBuf) - 1,
                    "%s:%u:%s:%u:%s",
                    ch->m_strChannelName.c_str(),
                    ch->m_unThreadNum,
                    ch->m_bRelease?APN_PROXY_CHANNEL_TYPE_RELEASE:APN_PROXY_CHANNEL_TYPE_DEV,
                    pool->getQueuedMsgNum(),
                    ch->m_strApps.c_str());
                szResult += strlen(szResult);
                iter++;
            }
            szResult = m_szBuf;
        }
    }while(0);
    replyMsg(this,
        msg->event().getConnId(),
        msg->event().getMsgHeader().getMsgType() + 1,
        msg->event().getMsgHeader().getTaskId(),
        false,
        ret,
        szErrMsg,
        szResult,
        0);
    CwxMsgBlockAlloc::free(msg);
    return 0;

}
///收到app查询消息；返回值：0，成功；-1：失败
int  ApnProxyApp::recvQueryAppMsg(CwxMsgBlock* msg){
    int ret = APN_PROXY_ERR_SUCCESS;
    char const* szErrMsg = NULL;
    char* szResult = NULL;
    CwxKeyValueItem const* app = NULL;

    ApnProxyConfigApp const* pApp=NULL;
    list<string>::const_iterator ch_iter;
    string strValue;
    do{
        if (!m_tss.m_pReader->unpack(msg->rd_ptr(), msg->length(), false, true)){
            ret = APN_PROXY_ERR_INVALID_PACKAGE;
            szErrMsg = m_tss.m_pReader->getErrMsg();
            break;
        }
        ///获取app
        app = m_tss.m_pReader->getKey(APN_PROXY_KEY_APP, false);
        if (app){
            ///检查app是否存在
            if (m_config.m_apps.find(string(app->m_szData)) == m_config.m_apps.end()){
                ret = APN_PROXY_ERR_NO_APP;
                szErrMsg = "The app doesn't exist.";
                break;
            }
            pApp = m_config.m_apps.find(string(app->m_szData))->second;
            ch_iter = pApp->m_channels.begin();
            strValue = "";
            while(ch_iter != pApp->m_channels.end()){
                if (strValue.length()) strValue += ",";
                strValue += *ch_iter;
                ch_iter++;
            }
            CwxCommon::snprintf(m_szBuf,
                APN_MAX_REPLY_BUF_SIZE -1,
                "%s:%s",
                pApp->m_strAppName.c_str(),
                strValue.c_str());
            szResult = m_szBuf;
        }else{
            map<string, ApnProxyConfigApp*>::iterator iter = m_config.m_apps.begin();
            szResult = m_szBuf;
            while(iter != m_config.m_apps.end()){
                pApp = iter->second;
                ch_iter = pApp->m_channels.begin();
                strValue = "";
                while(ch_iter != pApp->m_channels.end()){
                    if (strValue.length()) strValue += ",";
                    strValue += *ch_iter;
                    ch_iter++;
                }
                if (szResult != m_szBuf){
                    szResult[0]='\n';
                    szResult++;
                }
                CwxCommon::snprintf(szResult,
                    APN_MAX_REPLY_BUF_SIZE - (szResult - m_szBuf) - 1,
                    "%s:%s",
                    pApp->m_strAppName.c_str(),
                    strValue.c_str());
                szResult += strlen(szResult);
                iter++;
            }
            szResult = m_szBuf;
        }
    }while(0);
    replyMsg(this,
        msg->event().getConnId(),
        msg->event().getMsgHeader().getMsgType()+1,
        msg->event().getMsgHeader().getTaskId(),
        false,
        ret,
        szErrMsg,
        szResult,
        0);
    CwxMsgBlockAlloc::free(msg);
    return 0;
}
///收到thread状态查询消息；返回值：0，成功；-1：失败
int  ApnProxyApp::recvQueryThreadMsg(CwxMsgBlock* msg){
    int ret = APN_PROXY_ERR_SUCCESS;
    char const* szErrMsg = NULL;
    CwxKeyValueItem const* app = NULL;
    CwxKeyValueItem const* channel = NULL;
    char * szResult = NULL;
    do{
        if (!m_tss.m_pReader->unpack(msg->rd_ptr(), msg->length(), false, true)){
            ret = APN_PROXY_ERR_INVALID_PACKAGE;
            szErrMsg = m_tss.m_pReader->getErrMsg();
            break;
        }
        ///获取app
        app = m_tss.m_pReader->getKey(APN_PROXY_KEY_APP, false);
        ///检查app是否存在
        if (app && (m_config.m_apps.find(string(app->m_szData)) == m_config.m_apps.end())){
            ret = APN_PROXY_ERR_NO_APP;
            szErrMsg = "The app doesn't exist.";
            break;
        }
        ///获取channel
        channel = m_tss.m_pReader->getKey(APN_PROXY_KEY_CH, false);
        ///检查channel是否存在
        if (channel && (m_threadPools.find(string(channel->m_szData)) == m_threadPools.end())){
            ret = APN_PROXY_ERR_NO_CHANNEL;
            szErrMsg = "The channel doesn't exist.";
            break;
        }
        ApnProxyTss** pTss = NULL;
        ApnProxyConfigChannel* ch=NULL;
        if (channel){
            szResult = m_szBuf;
            pTss = m_threadPools.find(string(channel->m_szData))->second.second;
            ch = m_config.m_channels.find(string(channel->m_szData))->second;
            outputResult(ch, pTss, app?app->m_szData:NULL, szResult);
            szResult = m_szBuf;
        }else{
            map<string, pair<CwxThreadPool*, ApnProxyTss**> >::iterator iter = m_threadPools.begin();
            szResult = m_szBuf;
            while(iter != m_threadPools.end()){
                pTss = iter->second.second;
                ch = m_config.m_channels.find(iter->first)->second;
                outputResult(ch, pTss, app?app->m_szData:NULL, szResult);
                iter++;
            }
            szResult = m_szBuf;
        }
    }while(0);
    replyMsg(this,
        msg->event().getConnId(),
        msg->event().getMsgHeader().getMsgType()+1,
        msg->event().getMsgHeader().getTaskId(),
        false,
        ret,
        szErrMsg,
        szResult,
        0);
    CwxMsgBlockAlloc::free(msg);
    return 0;
}

void ApnProxyApp::outputResult(ApnProxyConfigChannel* ch, ApnProxyTss** pTss, char const* szApp, char*& szResult){
    CWX_UINT32 i = 0;
    ApnProxySsl* ssl=NULL;
    for (i=0; i<ch->m_unThreadNum; i++){
        if (szResult != m_szBuf){
            szResult[0] = '\n';
            szResult++;
        }
        if (szApp){
            ssl = pTss[i]->m_appSsl.find(string(szApp))->second;
            CwxCommon::snprintf(szResult,
                APN_MAX_REPLY_BUF_SIZE - (szResult - m_szBuf) - 1,
                "%s:%s:%u:%s:%u",
                ch->m_strChannelName.c_str(),
                szApp,
                i,
                ch->m_bRelease?APN_PROXY_CHANNEL_TYPE_RELEASE:APN_PROXY_CHANNEL_TYPE_DEV,
                ssl->isConnected());
            szResult += strlen(szResult);
        }else{
            map<string, ApnProxySsl*>::iterator ssl_iter = pTss[i]->m_appSsl.begin();
            while(ssl_iter != pTss[i]->m_appSsl.end()){
                ssl = ssl_iter->second;
                CwxCommon::snprintf(szResult,
                    APN_MAX_REPLY_BUF_SIZE - (szResult - m_szBuf) - 1,
                    "%s:%s:%u:%s:%u",
                    ch->m_strChannelName.c_str(),
                    ssl_iter->first.c_str(),
                    i,
                    ch->m_bRelease?APN_PROXY_CHANNEL_TYPE_RELEASE:APN_PROXY_CHANNEL_TYPE_DEV,
                    ssl->isConnected());
                szResult += strlen(szResult);
                ssl_iter++;
            }
        }
    }
}

///消息回复
void ApnProxyApp::replyMsg(ApnProxyApp* pApp, ///<app对象
                     CWX_UINT32 uiConnId, ///<连接id
                     CWX_UINT16 unMsgType, ///<消息类型
                     CWX_UINT32 uiTaskId, ///<任务id
                     bool       bCloseConn, ///<是否关闭连接
                     int        ret, ///<返回的状态值
                     char const* szErrMsg, ///<若出错则返回错误消息
                     char const* result, ///<若是状态查询，则指定result，若为空则不添加
                     CWX_UINT8 ucStatus ///<notice的apn状态值，若为0则不添加
                     )
{
    pApp->m_tss.m_pWriter->beginPack();
    pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_RET, strlen(APN_PROXY_KEY_RET),  ret);
    if (APN_PROXY_ERR_SUCCESS != ret){
        pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_ERR, strlen(APN_PROXY_KEY_ERR),  szErrMsg, strlen(szErrMsg), false);
        if (ucStatus) pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_STATUS, strlen(APN_PROXY_KEY_STATUS), ucStatus);
    }else{
        if (result) pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_RESULT, strlen(APN_PROXY_KEY_RESULT), result, strlen(result), false);
    }
    pApp->m_tss.m_pWriter->pack();
    CwxMsgHead head(0, 0, unMsgType, uiTaskId, pApp->m_tss.m_pWriter->getMsgSize());
    CwxMsgBlock* msg = CwxMsgBlockAlloc::pack(head, pApp->m_tss.m_pWriter->getMsg(), pApp->m_tss.m_pWriter->getMsgSize());
    if (!msg){
        CWX_ERROR(("No memory to alloc msg, size:%u", pApp->m_tss.m_pWriter->getMsgSize()));
        pApp->noticeCloseConn(uiConnId);
        return;
    }
    ///发送回复的数据包
    msg->send_ctrl().setMsgAttr(bCloseConn?CwxMsgSendCtrl::CLOSE_NOTICE:CwxMsgSendCtrl::NONE);
    msg->send_ctrl().setConnId(uiConnId);
    if (0 != pApp->sendMsgByConn(msg))	{
        CWX_ERROR(("Failure to send msg to client, conn[%u]", uiConnId));
        CwxMsgBlockAlloc::free(msg);
        pApp->noticeCloseConn(uiConnId);
    }
}

