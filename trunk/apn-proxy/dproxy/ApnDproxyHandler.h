#ifndef __APN_PROXY_HANDLER_H__
#define __APN_PROXY_HANDLER_H__

/*
版权声明：
本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
*/

#include "ApnProxyMacro.h"
#include "CwxCommander.h"


class ApnProxyApp;
///消息处理函数handle
class ApnProxyHandler : public CwxCmdOp 
{
public:
    ///构造函数
    ApnProxyHandler(ApnProxyApp* pApp):m_pApp(pApp){
        memset(m_szAscii, 0x00, 255);
        m_szAscii[(CWX_UINT8)'0']=0;
        m_szAscii[(CWX_UINT8)'1']=1;
        m_szAscii[(CWX_UINT8)'2']=2;
        m_szAscii[(CWX_UINT8)'3']=3;
        m_szAscii[(CWX_UINT8)'4']=4;
        m_szAscii[(CWX_UINT8)'5']=5;
        m_szAscii[(CWX_UINT8)'6']=6;
        m_szAscii[(CWX_UINT8)'7']=7;
        m_szAscii[(CWX_UINT8)'8']=8;
        m_szAscii[(CWX_UINT8)'9']=9;
        m_szAscii[(CWX_UINT8)'a']=10;
        m_szAscii[(CWX_UINT8)'b']=11;
        m_szAscii[(CWX_UINT8)'c']=12;
        m_szAscii[(CWX_UINT8)'d']=13;
        m_szAscii[(CWX_UINT8)'e']=14;
        m_szAscii[(CWX_UINT8)'f']=15;
        m_szAscii[(CWX_UINT8)'A']=10;
        m_szAscii[(CWX_UINT8)'B']=11;
        m_szAscii[(CWX_UINT8)'C']=12;
        m_szAscii[(CWX_UINT8)'D']=13;
        m_szAscii[(CWX_UINT8)'E']=14;
        m_szAscii[(CWX_UINT8)'F']=15;
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
    unsigned char    m_szAscii[256];
};

#endif 
