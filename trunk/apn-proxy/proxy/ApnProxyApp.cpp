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

    ///注册echo请求的处理handle，echo请求的svr-id为SVR_TYPE_ECHO
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
            pTss[i]->init(iter->second, m_config.m_channelApps);
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
int ApnProxyApp::onRecvMsg(CwxMsgBlock* msg, CwxAppHandler4Msg& conn, CwxMsgHead const& header, bool& bSuspendConn){
    int ret = 0;
    if (!msg) return -1;
    msg->event().setSvrId(conn.getConnInfo().getSvrId());
    msg->event().setHostId(conn.getConnInfo().getHostId());
    msg->event().setConnId(conn.getConnInfo().getConnId());
    msg->event().setMsgHeader(header);
    msg->event().setEvent(CwxEventInfo::RECV_MSG);
    switch(header.m_unMsgType){

    }
    if (msg) CwxMsgBlockAlloc::free(msg);
    return ret;
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



