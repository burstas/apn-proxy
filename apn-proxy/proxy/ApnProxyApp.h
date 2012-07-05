#ifndef __APN_PROXY_APP_H__
#define __APN_PROXY_APP_H__
/*
��Ȩ������
    �������ѭGNU GPL V3��http://www.gnu.org/licenses/gpl.html����
    ��ϵ��ʽ��email:cwinux@gmail.com��΢��:http://t.sina.com.cn/cwinux
*/
#include "CwxAppFramework.h"
#include "ApnProxyConfig.h"
#include "ApnProxyHandler.h"
#include "ApnProxyTss.h"

#define APN_PROXY_APP_VERSION "0.1"
#define APN_PROXY_APP_MODIFY_DATE "2012-06-17"


///���̵߳�apple APN Proxy����
class  ApnProxyApp: public CwxAppFramework{
public:
    enum{
        APN_MAX_REPLY_BUF_SIZE = 1024 * 1024, ///<״̬��ѯ�����buf size
        LOG_FILE_SIZE = 30, ///<ÿ����ѭ��ʹ����־�ļ���MByte
        LOG_FILE_NUM = 7, ///<��ѭ��ʹ����־�ļ�������
        SVR_TYPE_APN = CwxAppFramework::SVR_TYPE_USER_START, ///<proxy����ķ������ͣ���SVR-ID����ֵ
        APN_MSG_TYPE_NOTICE = 1, ///<notice����Ϣ����
        APN_MSG_TYPE_NOTICE_REPLY = 2, ///<notice��reply��Ϣ����
        APN_MSG_TYPE_CHANNEL_INFO = 3, ///<��ȡchannel����Ϣ
        APN_MSG_TYPE_CHANNEL_INFO_REPLY = 4, ///<channel��Ϣ��reply��Ϣ����
        APN_MSG_TYPE_APP_INFO = 5, ///<��ȡapp����Ϣ
        APN_MSG_TYPE_APP_INFO_REPLY = 6, ///<app��Ϣ��reply��Ϣ����
        APN_MSG_TYPE_THREAD_INFO = 7, ///<��ȡ�߳���Ϣ
        APN_MSG_TYPE_THREAD_INFO_REPLY = 8 ///<�߳���Ϣ��reply��Ϣ����
    };
    ///���캯��
	ApnProxyApp();
    ///��������
	virtual ~ApnProxyApp();
    ///���س�ʼ������
    virtual int init(int argc, char** argv);
public:
    ///ʱ����Ӧ����
    virtual void onTime(CwxTimeValue const& current);
    ///signal��Ӧ����
    virtual void onSignal(int signum);
    ///�յ�echo��Ϣ����Ӧ����
    virtual int onRecvMsg(CwxMsgBlock* msg,///<�յ���proxy�������ݰ�
                        CwxAppHandler4Msg& conn,///<�յ�proxy��������Ӷ���
                        CwxMsgHead const& header, ///<�յ�proxy�������Ϣͷ
                        bool& bSuspendConn///<true��ֹͣ�����Ӽ���������Ϣ��false�������ӿ��Լ���������Ϣ
                        );
public:
    ///��Ϣ�ظ�
    static void replyMsg(ApnProxyApp* pApp, ///<app����
        CWX_UINT32 uiConnId, ///<����id
        CWX_UINT16 unMsgType, ///<��Ϣ����
        CWX_UINT32 uiTaskId, ///<����id
        bool       bCloseConn, ///<�Ƿ�ر�����
        int        ret, ///<���ص�״ֵ̬
        char const* szErrMsg, ///<�������򷵻ش�����Ϣ
        char const* result, ///<����״̬��ѯ����ָ��result����Ϊ�������
        CWX_UINT8 ucStatus, ///<notice��apn״ֵ̬����Ϊ0�����
        CWX_UINT32  uiLastId, ///<��һ�ε�ʧ��id����Ϊ0�����
        char const* szLastDevId, ///<��һ�ε�ʧ��dev id����Ϊ�������
        char const* szLastContent ///<��һ��ʧ�ܵ����ݣ���Ϊ�������
        );
    ///��ȡ������Ϣ
    ApnProxyConfig const& getConfig() const { return  m_config;}
protected:
    ///�������л�������API
	virtual int initRunEnv();
    virtual void destroy();
private:
    void outputResult(ApnProxyConfigChannel* ch,
        ApnProxyTss** pTss,
        char const* szApp,
        char*& szResult);
    ///�յ�notice��Ϣ������ֵ��0���ɹ���-1��ʧ��
    int  recvNoticeMsg(CwxMsgBlock* msg);
    ///�յ�channel��ѯ��Ϣ������ֵ��0���ɹ���-1��ʧ��
    int  recvQueryChannelMsg(CwxMsgBlock* msg);
    ///�յ�app��ѯ��Ϣ������ֵ��0���ɹ���-1��ʧ��
    int  recvQueryAppMsg(CwxMsgBlock* msg);
    ///�յ�thread״̬��ѯ��Ϣ������ֵ��0���ɹ���-1��ʧ��
    int  recvQueryThreadMsg(CwxMsgBlock* msg);
private:
    ApnProxyHandler*             m_proxyHandler;///<proxy�������commander handle
    map<string, pair<CwxThreadPool*, ApnProxyTss**> >  m_threadPools; ///<�̳߳�map
    ApnProxyConfig               m_config;///<�����ļ�����
    ApnProxyTss                  m_tss; ///<���̵߳�tss����
    char                         m_szBuf[APN_MAX_REPLY_BUF_SIZE];
};
#endif

