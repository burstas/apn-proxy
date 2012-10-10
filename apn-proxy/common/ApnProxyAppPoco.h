#ifndef __APN_PROXY_APP_POCO_H__
#define __APN_PROXY_APP_POCO_H__

/*
版权声明：
本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
*/

#include "ApnProxySsl.h"


///这个类是多线程安全的
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
    ///发送一个notice。返回值，0：成功；-1：失败。
    static int sendNotice(ApnProxySsl *ssl, ///<ssl连接
        char const* devBinToken, ///<二进制的设备号
        char const* payloadBuf, ///<发送的notice的payload，不能超过APN_PROXY_APP_MAXPAYLOAD_SIZE
        size_t payloadLen,  ///<数据的长度
        char* szErr2K ///<错误信息
        );
    ///发送一个enhanced notice。返回值，0：成功；-1：失败。
    static int sendEnhancedNotice(ApnProxySsl *ssl, ///<ssl连接
        CWX_UINT32 uiExpire, ///<有效时间
        CWX_UINT32 uiId, ///<消息的id
        char const* devBinToken, ///<二进制的设备号
        char const* payloadBuf, ///<发送的notice的payload，不能超过APN_PROXY_APP_MAXPAYLOAD_SIZE
        size_t payloadLen, ///<数据的长度
        char* szErr2K ///<错误信息
        );
    ///获取enhanced notice的错误信息。返回值，0：不存在；1：成功返回
    static int readEnhancedNoticeErr(ApnProxySsl *ssl, ///<ssl连接
        CWX_UINT8& ucStatus, ///<错误代码
        CWX_UINT32& uiId,  ///<对应的消息id
        CWX_UINT32 uiMilliTimeout, ///<超时时间
        char* szErr2K ///<错误信息
        );
    ///获取错误代码的错误消息
    static char const* getErrStatusMsg(CWX_UINT8 ucStatus);
private:
    ApnProxyAppPoco(){}
    ~ApnProxyAppPoco(){}
};




#endif
