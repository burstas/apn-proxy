#ifndef __APN_PROXY_HANDLER_H__
#define __APN_PROXY_HANDLER_H__

#include "ApnProxyMacro.h"
#include "CwxCommander.h"


class ApnProxyApp;
///��Ϣ������handle
class ApnProxyHandler : public CwxCmdOp 
{
public:
    ///���캯��
    ApnProxyHandler(ApnProxyApp* pApp):m_pApp(pApp){
    }
    ///��������
    virtual ~ApnProxyHandler(){
    }
public:
    ///�յ�����Ĵ�����
    virtual int onRecvMsg(CwxMsgBlock*& msg,///<echo���ݰ�����ص�����������Ϣ
                            CwxTss* pThrEnv///<�����̵߳�thread-specific-store
                            );
private:
    ApnProxyApp*     m_pApp;  ///<app����
};

#endif 
