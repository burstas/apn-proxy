#ifndef __APN_PROXY_APP_POCO_H__
#define __APN_PROXY_APP_POCO_H__

/*
��Ȩ������
�������ѭGNU GPL V3��http://www.gnu.org/licenses/gpl.html����
��ϵ��ʽ��email:cwinux@gmail.com��΢��:http://t.sina.com.cn/cwinux
*/

#include "ApnProxySsl.h"


///������Ƕ��̰߳�ȫ��
class ApnProxyAppPoco{
public:
    enum{
        APN_ERR_SUCCESS=0, ///< No errors encountered
        APN_ERR_PROCESS=1, ///< Processing error
        APN_ERR_MISS_DEV_TOKEN=2, ///<Missing device token
        APN_ERR_MISS_TOPIC=3, ///<Missing topic
        APN_ERR_MISS_PAYLOAD=4, ///<Missing payload
        APN_ERR_INVALID_TOKEN_SIZE=5, ///<Invalid token size
        APN_ERR_INVALID_TOPIC_SIZE=6, ///<Invalid topic size
        APN_ERR_INVALID_PAYLOAD_SIZE=7, ///<Invalid payload size
        APN_ERR_INVALID_TOKEN=8, ///<Invalid token
        APN_ERR_UNKNOWN=255 ///<None (unknown)
    };
public:
    ///����һ��notice������ֵ��0���ɹ���-1��ʧ�ܡ�
    static int sendNotice(ApnProxySsl *ssl, ///<ssl����
        char const* devBinToken, ///<�����Ƶ��豸��
        char const* payloadBuf, ///<���͵�notice��payload�����ܳ���APN_PROXY_APP_MAXPAYLOAD_SIZE
        size_t payloadLen,  ///<���ݵĳ���
        char* szErr2K ///<������Ϣ
        );
    ///����һ��enhanced notice������ֵ��0���ɹ���-1��ʧ�ܡ�
    static int sendEnhancedNotice(ApnProxySsl *ssl, ///<ssl����
        CWX_UINT32 uiExpire, ///<��Чʱ��
        CWX_UINT32 uiId, ///<��Ϣ��id
        char const* devBinToken, ///<�����Ƶ��豸��
        char const* payloadBuf, ///<���͵�notice��payload�����ܳ���APN_PROXY_APP_MAXPAYLOAD_SIZE
        size_t payloadLen, ///<���ݵĳ���
        char* szErr2K ///<������Ϣ
        );
    ///��ȡenhanced notice�Ĵ�����Ϣ������ֵ��0�������ڣ�1���ɹ�����
    static int readEnhancedNoticeErr(ApnProxySsl *ssl, ///<ssl����
        CWX_UINT8& ucStatus, ///<�������
        CWX_UINT32& uiId,  ///<��Ӧ����Ϣid
        CWX_UINT32 uiMilliTimeout, ///<��ʱʱ��
        char* szErr2K ///<������Ϣ
        );
    ///��ȡ�������Ĵ�����Ϣ
    static char const* getErrStatusMsg(CWX_UINT8 ucStatus);
private:
    ApnProxyAppPoco(){}
    ~ApnProxyAppPoco(){}
};




#endif
