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
        memset(m_szAscii, 0x00, 255);
        m_szAscii['0']=0;
        m_szAscii['1']=1;
        m_szAscii['2']=2;
        m_szAscii['3']=3;
        m_szAscii['4']=4;
        m_szAscii['5']=5;
        m_szAscii['6']=6;
        m_szAscii['7']=7;
        m_szAscii['8']=8;
        m_szAscii['9']=1;
        m_szAscii['a']=10;
        m_szAscii['b']=11;
        m_szAscii['c']=12;
        m_szAscii['d']=13;
        m_szAscii['e']=14;
        m_szAscii['f']=15;
        m_szAscii['A']=10;
        m_szAscii['B']=11;
        m_szAscii['C']=12;
        m_szAscii['D']=13;
        m_szAscii['E']=14;
        m_szAscii['F']=15;
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
    unsigned char    m_szAscii[256];
};

#endif 
