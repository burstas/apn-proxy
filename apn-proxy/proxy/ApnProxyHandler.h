#ifndef __APN_PROXY_HANDLER_H__
#define __APN_PROXY_HANDLER_H__

#include "ApnProxyMacro.h"
#include "CwxCommander.h"


class ApnProxyApp;
///消息处理函数handle
class ApnProxyHandler : public CwxCmdOp 
{
public:
    ///构造函数
    ApnProxyHandler(ApnProxyApp* pApp):m_pApp(pApp){
    }
    ///析构函数
    virtual ~ApnProxyHandler(){
    }
public:
    ///收到请求的处理函数
    virtual int onRecvMsg(CwxMsgBlock*& msg,///<echo数据包及相关的请求连接信息
                            CwxTss* pThrEnv///<处理线程的thread-specific-store
                            );
private:
    ApnProxyApp*     m_pApp;  ///<app对象
};

#endif 
