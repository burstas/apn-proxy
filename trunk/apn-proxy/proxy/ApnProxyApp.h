#ifndef __CWX_ECHO_APP_H__
#define __CWX_ECHO_APP_H__
/*
版权声明：
    本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
    联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
*/
#include "CwxAppFramework.h"
#include "ApnProxyConfig.h"
#include "ApnProxyHandler.h"
#include "ApnProxyTss.h"

#define APN_PROXY_APP_VERSION "0.1"
#define APN_PROXY_APP_MODIFY_DATE "2012-06-17"


///多线程的apple APN Proxy服务
class  ApnProxyApp: public CwxAppFramework{
public:
    enum{
        LOG_FILE_SIZE = 30, ///<每个可循环使用日志文件的MByte
        LOG_FILE_NUM = 7, ///<可循环使用日志文件的数量
        SVR_TYPE_APN = CwxAppFramework::SVR_TYPE_USER_START, ///<proxy服务的服务类型，及SVR-ID的数值
        APN_MSG_TYPE_NOTICE = 1, ///<notice的消息类型
        APN_MSG_TYPE_NOTICE_REPLY = 2, ///<notice的reply消息类型
    };
    ///构造函数
	ApnProxyApp();
    ///析构函数
	virtual ~ApnProxyApp();
    ///重载初始化函数
    virtual int init(int argc, char** argv);
public:
    ///时钟响应函数
    virtual void onTime(CwxTimeValue const& current);
    ///signal响应函数
    virtual void onSignal(int signum);
    ///收到echo消息的响应函数
    virtual int onRecvMsg(CwxMsgBlock* msg,///<收到的echo请求数据包
                        CwxAppHandler4Msg& conn,///<收到echo请求的连接对象
                        CwxMsgHead const& header, ///<收到echo请求的消息头
                        bool& bSuspendConn///<true：停止此连接继续接受稍息，false：此连接可以继续接受消息
                        );
protected:
    static int setSockAttr(CWX_HANDLE handle, void* arg);

    ///重载运行环境设置API
	virtual int initRunEnv();
    virtual void destroy();
private:
    CwxEchoEventHandler*         m_eventHandler;///<echo请求处理的commander handle
    CwxThreadPool*               m_threadPool;///<线程池对象
    CwxEchoConfig               m_config;///<配置文件对象
};
#endif

