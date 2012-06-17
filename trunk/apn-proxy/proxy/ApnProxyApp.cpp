#include "ApnProxyApp.h"
#include "CwxDate.h"

///���캯��
ApnProxyApp::ApnProxyApp(){
    m_proxyHandler = NULL;
}

///��������
ApnProxyApp::~ApnProxyApp(){
}

///��ʼ��
int ApnProxyApp::init(int argc, char** argv){
    string strErrMsg;
    ///���ȵ��üܹ���init api
    if (CwxAppFramework::init(argc, argv) == -1) return -1;
    ///����Ƿ�ͨ��-fָ���������ļ�����û�У������Ĭ�ϵ������ļ�
    if ((NULL == this->getConfFile()) || (strlen(this->getConfFile()) == 0)){
        this->setConfFile("apn_proxy.cnf");
    }
    ///���������ļ�����ʧ�����˳�
    if (0 != m_config.loadConfig(getConfFile())){
        CWX_ERROR((m_config.getError()));
        return -1;
    }
    ///����������־�����level
    setLogLevel(CwxLogger::LEVEL_ERROR|CwxLogger::LEVEL_INFO|CwxLogger::LEVEL_WARNING);
    return 0;
}

///�������л�����Ϣ
int ApnProxyApp::initRunEnv(){
    ///����ϵͳ��ʱ�Ӽ������С�̶�Ϊ1ms����Ϊ1s��
    this->setClick(100);//0.1s
    ///���ù���Ŀ¼
    this->setWorkDir(this->m_config.m_strWorkDir.c_str());
    ///����ѭ��������־������
    this->setLogFileNum(LOG_FILE_NUM);
    ///����ÿ����־�ļ��Ĵ�С
    this->setLogFileSize(LOG_FILE_SIZE*1024*1024);
    ///���üܹ���initRunEnv��ʹ�������õĲ�����Ч
    if (CwxAppFramework::initRunEnv() == -1 ) return -1;
    blockSignal(SIGPIPE);
    //set version
    this->setAppVersion(APN_PROXY_APP_VERSION);
    //set last modify date
    this->setLastModifyDatetime(APN_PROXY_APP_MODIFY_DATE);
    //set compile date
    this->setLastCompileDatetime(CWX_COMPILE_DATE(_BUILD_DATE));

    ///�����ص������ļ���Ϣ�������־�ļ��У��Թ��鿴���
    m_config.outputConfig();

    ///��ʼ��tss
    m_tss.init(NULL, NULL);
    
    ///ע��proxy����Ĵ���handle
    m_proxyHandler = new ApnProxyHandler(this);         
    this->getCommander().regHandle(SVR_TYPE_APN, m_proxyHandler);

    ///����TCP���ӣ��佨�������ӵ�svr-id��ΪSVR_TYPE_APN�����յ���Ϣ��svr-id��ΪSVR_TYPE_APN��
    ///ȫ����m_proxyHandler����������
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
    ///�����̳߳ض��󣬴��̳߳����̵߳�group-idΪ2
    CWX_UINT16 i=0;
    CWX_UINT16 uiThreadId = 2;
    CwxThreadPool* pThreadPool = NULL;
    map<string, ApnProxyConfigChannel*>::iterator iter = m_config.m_channels.begin();
    while(iter != m_config.m_channels.end()){
        pThreadPool = new CwxThreadPool(uiThreadId++, 
            iter->second->m_unThreadNum,
            getThreadPoolMgr(),
            &getCommander());

        ///�����߳�
        ApnProxyTss**pTss = new ApnProxyTss*[iter->second->m_unThreadNum];
        for (i=0; i<iter->second->m_unThreadNum; i++){
            pTss[i] = new ApnProxyTss();
            pTss[i]->init(iter->second, &m_config.m_channelApps);
        }
        m_threadPools[iter->first] = pair<CwxThreadPool*, ApnProxyTss**>(pThreadPool, pTss);

        ///�����̣߳��̳߳����̵߳��߳�ջ��СΪ1M��
        if ( 0 != pThreadPool->start((CwxTss**)pTss)){
            CWX_ERROR(("Failure to start thread pool"));
            return -1;
        }
        iter++;
    }

    return 0;

}

///ʱ�Ӻ�����ʲôҲû����
void ApnProxyApp::onTime(CwxTimeValue const& current){
    CwxAppFramework::onTime(current);
}

///�źŴ�����
void ApnProxyApp::onSignal(int signum){
    switch(signum){
    case SIGQUIT: 
        ///����ؽ���֪ͨ�˳������Ƴ�
        CWX_INFO(("Recv exit signal , exit right now."));
        this->stop();
        break;
    default:
        ///�����źţ�ȫ������
        CWX_INFO(("Recv signal=%d, ignore it.", signum));
        break;
    }

}

///proxy�����������Ϣ
int ApnProxyApp::onRecvMsg(CwxMsgBlock* msg, CwxAppHandler4Msg& conn, CwxMsgHead const& header, bool& bSuspendConn){
    if (!msg) return -1;
    msg->event().setSvrId(conn.getConnInfo().getSvrId());
    msg->event().setHostId(conn.getConnInfo().getHostId());
    msg->event().setConnId(conn.getConnInfo().getConnId());
    msg->event().setMsgHeader(header);
    msg->event().setEvent(CwxEventInfo::RECV_MSG);
    switch(header.m_unMsgType){
        case APN_MSG_TYPE_NOTICE:
            return recvNoticeMsg(msg);
        case APN_MSG_TYPE_CHANNEL_INFO:
            return recvQueryChannelMsg(msg);
        case APN_MSG_TYPE_APP_INFO:
            return recvQueryAppMsg(msg);
        case APN_MSG_TYPE_THREAD_INFO:
            return recvQueryThreadMsg(msg);
    }

    CwxCommon::snprintf(m_tss->m_szBuf2K, 2048, "Unknown msg type:%u", header.getMsgType());
    replyMsg(this,
        conn.getConnInfo().getConnId(),
        header.getMsgType() + 1,
        header.getTaskId(),
        true,
        APN_PROXY_ERR_UNKNOWN_MSG,
        NULL,
        0);
    return 0;
}


void ApnProxyApp::destroy()
{
    map<string, pair<CwxThreadPool*, ApnProxyTss**> >::iterator iter =  m_threadPools.begin;
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

///�յ�notice��Ϣ������ֵ��0���ɹ���-1��ʧ��
int  ApnProxyApp::recvNoticeMsg(CwxMsgBlock* msg){

}
///�յ�channel��ѯ��Ϣ������ֵ��0���ɹ���-1��ʧ��
int  ApnProxyApp::recvQueryChannelMsg(CwxMsgBlock* msg){

}
///�յ�app��ѯ��Ϣ������ֵ��0���ɹ���-1��ʧ��
int  ApnProxyApp::recvQueryAppMsg(CwxMsgBlock* msg){

}
///�յ�thread״̬��ѯ��Ϣ������ֵ��0���ɹ���-1��ʧ��
int  ApnProxyApp::recvQueryThreadMsg(CwxMsgBlock* msg){

}
///��Ϣ�ظ�
void ApnProxyApp::replyMsg(ApnProxyApp* pApp, ///<app����
                     CWX_UINT32 uiConnId, ///<����id
                     CWX_UINT16 unMsgType, ///<��Ϣ����
                     CWX_UINT32 uiTaskId, ///<����id
                     bool       bCloseConn, ///<�Ƿ�ر�����
                     int        ret, ///<���ص�״ֵ̬
                     char const* szErrMsg, ///<�������򷵻ش�����Ϣ
                     char const* result, ///<����״̬��ѯ����ָ��result����Ϊ�������
                     CWX_UINT8 ucStatus ///<notice��apn״ֵ̬����Ϊ0�����
                     )
{
    pApp->m_tss.m_pWriter->beginPack();
    pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_RET, strlen(APN_PROXY_KEY_RET),  ret);
    if (APN_PROXY_ERR_SUCCESS != ret){
        pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_ERR, strlen(APN_PROXY_KEY_ERR),  szErrMsg, strlen(szErrMsg), false);
        if (ucStatus) pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_STATUS, strlen(APN_PROXY_KEY_STATUS), ucStatus);
    }else{
        if (result) pApp->m_tss.m_pWriter->addKeyValue(APN_PROXY_KEY_RESULT, stlren(APN_PROXY_KEY_RESULT), result, strlen(result), false);
    }
    pApp->m_tss.m_pWriter->->pack();
    CwxMsgHead head(0, 0, unMsgType, uiTaskId, pApp->m_tss.m_pWriter->getMsgSize());
    CwxMsgBlock* msg = CwxMsgBlockAlloc::pack(head, pApp->m_tss.m_pWriter->getMsg(), pApp->m_tss.m_pWriter->getMsgSize());
    if (!msg){
        CWX_ERROR(("No memory to alloc msg, size:%u", pApp->m_tss.m_pWriter->getMsgSize()));
        pApp->noticeCloseConn(uiConnId);
        return;
    }
    ///���ͻظ������ݰ�
    msg->send_ctrl().setMsgAttr(bCloseConn?CwxMsgSendCtrl::CLOSE_NOTICE:CwxMsgSendCtrl::NONE);
    if (!pApp->sendMsgByConn(msg))	{
        CWX_ERROR(("Failure to send msg to client, conn[%u]", uiConnId));
        CwxMsgBlockAlloc::free(msg);
        pApp->noticeCloseConn(uiConnId);
    }
}

