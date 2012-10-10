#ifndef __APN_DPROXY_APP_H__
#define __APN_DPROXY_APP_H__
/*
版权声明：
    本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
    联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
*/
#include "CwxAppFramework.h"
#include "ApnDproxyConfig.h"
#include "ApnProxyHandler.h"
#include "ApnDproxyTss.h"
#include "Mysql.h"

#define APN_DPROXY_APP_VERSION "0.1"
#define APN_DPROXY_APP_MODIFY_DATE "2012-06-17"


///多线程的apple APN DProxy服务
class  ApnDproxyApp: public CwxAppFramework{
public:
    enum{
        APN_MAX_REPLY_BUF_SIZE = 1024 * 1024, ///<状态查询的最大buf size
        LOG_FILE_SIZE = 30, ///<每个可循环使用日志文件的MByte
        LOG_FILE_NUM = 7, ///<可循环使用日志文件的数量
        SVR_TYPE_APN = CwxAppFramework::SVR_TYPE_USER_START, ///<proxy服务的服务类型，及SVR-ID的数值
        APN_MSG_TYPE_NOTICE = 1, ///<notice的消息类型
        APN_MSG_TYPE_NOTICE_REPLY = 2, ///<notice的reply消息类型
        APN_MSG_TYPE_CHANNEL_INFO = 3, ///<获取channel的信息
        APN_MSG_TYPE_CHANNEL_INFO_REPLY = 4, ///<channel信息的reply消息类型
        APN_MSG_TYPE_APP_INFO = 5, ///<获取app的信息
        APN_MSG_TYPE_APP_INFO_REPLY = 6, ///<app信息的reply消息类型
        APN_MSG_TYPE_THREAD_INFO = 7, ///<获取线程信息
        APN_MSG_TYPE_THREAD_INFO_REPLY = 8 ///<线程信息的reply消息类型
    };
    ///构造函数
	ApnDproxyApp();
    ///析构函数
	virtual ~ApnDproxyApp();
    ///重载初始化函数
    virtual int init(int argc, char** argv);
public:
    ///时钟响应函数
    virtual void onTime(CwxTimeValue const& current);
    ///signal响应函数
    virtual void onSignal(int signum);
    ///收到echo消息的响应函数
    virtual int onRecvMsg(CwxMsgBlock* msg,///<收到的proxy请求数据包
                        CwxAppHandler4Msg& conn,///<收到proxy请求的连接对象
                        CwxMsgHead const& header, ///<收到proxy请求的消息头
                        bool& bSuspendConn///<true：停止此连接继续接受稍息，false：此连接可以继续接受消息
                        );
public:
    ///消息回复
    static void replyMsg(ApnProxyApp* pApp, ///<app对象
        CWX_UINT32 uiConnId, ///<连接id
        CWX_UINT16 unMsgType, ///<消息类型
        CWX_UINT32 uiTaskId, ///<任务id
        bool       bCloseConn, ///<是否关闭连接
        int        ret, ///<返回的状态值
        char const* szErrMsg, ///<若出错则返回错误消息
        char const* result, ///<若是状态查询，则指定result，若为空则不添加
        CWX_UINT8 ucStatus, ///<notice的apn状态值，若为0则不添加
        CWX_UINT32  uiLastId, ///<上一次的失败id，若为0则不添加
        char const* szLastDevId, ///<上一次的失败dev id，若为空则不添加
        char const* szLastContent ///<上一次失败的内容，若为空则不添加
        );
    ///获取配置信息
    ApnDproxyConfig const& getConfig() const { return  m_config;}
    ///获取mysql对象
    Mysql& getMysql() { return m_mysql;}
protected:
    ///重载运行环境设置API
	virtual int initRunEnv();
    virtual void destroy();
private:
    void outputResult(ApnProxyConfigChannel* ch,
        ApnProxyTss** pTss,
        char const* szApp,
        char*& szResult);
    ///收到notice消息；返回值：0，成功；-1：失败
    int  recvNoticeMsg(CwxMsgBlock* msg);
    ///收到channel查询消息；返回值：0，成功；-1：失败
    int  recvQueryChannelMsg(CwxMsgBlock* msg);
    ///收到app查询消息；返回值：0，成功；-1：失败
    int  recvQueryAppMsg(CwxMsgBlock* msg);
    ///收到thread状态查询消息；返回值：0，成功；-1：失败
    int  recvQueryThreadMsg(CwxMsgBlock* msg);
private:
    ApnProxyHandler*             m_proxyHandler;///<proxy请求处理的commander handle
    map<string, pair<CwxThreadPool*, ApnProxyTss**> >  m_threadPools; ///<线程池map
    ApnDproxyConfig              m_config;///<配置文件对象
    ApnProxyTss                  m_tss; ///<主线程的tss对象
    Mysql                        m_mysql; ///<数据库对象
    char                         m_szBuf[APN_MAX_REPLY_BUF_SIZE];
};
#endif

