#include "ApnProxyAppPoco.h"

///发送一个notice。返回值，0：成功；-1：失败。
int ApnProxyAppPoco::sendNotice(ApnProxySsl *ssl, ///<ssl连接
                                char const* devBinToken, ///<二进制的设备号
                                char const* payloadBuf, ///<发送的notice的payload，不能超过APN_PROXY_APP_MAXPAYLOAD_SIZE
                                size_t payloadLen,  ///<数据的长度
                                char* szErr2K ///<错误信息
                                )
{
    CWX_UINT8 ucCmd = 0; /* command number */
    char sendBuf[sizeof(CWX_UINT8) + sizeof(CWX_UINT16) +
            APN_PROXY_APP_DEVICE_BINARY_SIZE + sizeof(CWX_UINT16) + APN_PROXY_APP_MAXPAYLOAD_SIZE];
    /* message format is, |COMMAND|TOKENLEN|TOKEN|PAYLOADLEN|PAYLOAD| */
    char* pPos = sendBuf;
    CWX_UINT16  unLen=0;
    
    /* command */
    *pPos = ucCmd; pPos++;
    /* token length network order */
    unLen = CWX_HTONS((CWX_UINT16)APN_PROXY_APP_DEVICE_BINARY_SIZE);
    memcpy(pPos, &unLen, sizeof(unLen)); pPos += sizeof(unLen);
    /* device token */
    memcpy(pPos, devBinToken, APN_PROXY_APP_DEVICE_BINARY_SIZE);
    pPos += APN_PROXY_APP_DEVICE_BINARY_SIZE;
    /* payload length network order */
    unLen = CWX_HTONS((CWX_UINT16)payloadLen);
    memcpy(pPos, &unLen, sizeof(unLen)); pPos += sizeof(unLen);
    /* payload */
    memcpy(pPos, payloadBuf, payloadLen);
    pPos += payloadLen;
    if (ssl->send_n(sendBuf, pPos - sendBuf, szErr2K) <=0) return -1;
    return 0;
}

///发送一个enhanced notice。返回值，0：成功；-1：失败。
int ApnProxyAppPoco::sendEnhancedNotice(ApnProxySsl *ssl, ///<ssl连接
                                        CWX_UINT32 uiExpire, ///<有效时间
                                        CWX_UINT32 uiId, ///<消息的id
                                        char const* devBinToken, ///<二进制的设备号
                                        char const* payloadBuf, ///<发送的notice的payload，不能超过APN_PROXY_APP_MAXPAYLOAD_SIZE
                                        size_t payloadLen, ///<数据的长度
                                        char* szErr2K ///<错误信息
                                        )
{
    CWX_UINT8 ucCmd = 1; /* command number */
    char sendBuf[sizeof(CWX_UINT8) + sizeof(CWX_UINT32) * 2 + sizeof(CWX_UINT16) + 
        APN_PROXY_APP_DEVICE_BINARY_SIZE + sizeof(CWX_UINT16) + APN_PROXY_APP_MAXPAYLOAD_SIZE];
    /* message format is, |COMMAND|TOKENLEN|TOKEN|PAYLOADLEN|PAYLOAD| */
    char* pPos = sendBuf;
    CWX_UINT16  unLen=0;
    CWX_UINT32  uiValue=0;

    /* message format is, |COMMAND|ID|EXPIRY|TOKENLEN|TOKEN|PAYLOADLEN|PAYLOAD| */
    /* command */
    pPos=ucCmd; pPos++;
    /* provider preference ordered ID */
    memcpy(pPos, &uiId, sizeof(uiId)); pPos += sizeof(uiId);
    /* expiry date network order */
    uiExpire = CWX_HTONL(uiExpire);
    memcpy(pPos, &uiExpire, sizeof(uiExpire)); pPos += sizeof(uiExpire);
    /* token length network order */
    unLen = CWX_HTONS(APN_PROXY_APP_DEVICE_BINARY_SIZE);
    memcpy(pPos, &unLen, sizeof(unLen)); pPos += sizeof(unLen);
    /* device token */
    memcpy(pPos, devBinToken, APN_PROXY_APP_DEVICE_BINARY_SIZE); pPos += APN_PROXY_APP_DEVICE_BINARY_SIZE;
    /* payload length network order */
    unLen = CWX_HTONS((CWX_UINT16)payloadLen);
    memcpy(pPos, &unLen, sizeof(unLen)); pPos += sizeof(unLen);
    /* payload */
    memcpy(pPos, payloadBuf, payloadLen);
    pPos += payloadLen;
    if (ssl->send_n(sendBuf, pPos - sendBuf, szErr2K) <=0) return -1;
    return 0;
}
///获取enhanced notice的错误信息。返回值，0：不存在；1：成功返回
int ApnProxyAppPoco::readEnhancedNoticeErr(ApnProxySsl *ssl, ///<ssl连接
                                           CWX_UINT8& ucStatus, ///<错误代码
                                           CWX_UINT32& uiId,  ///<对应的消息id
                                           CWX_UINT32 uiMilliTimeout=1, ///<超时时间
                                           char* szErr2K ///<错误信息
                                           )
{
    char szBuf[8];
    int ret = ssl->read_n(szBuf, 6, uiMilliTimeout, szErr2K);
    if (6 != ret){
        return 0;
    }
    ucStatus = szBuf[1];
    memcpy(&uiId, szBuf + 2, 4);
    return 1;
}

char const* ApnProxyAppPoco::getErrStatusMsg(CWX_UINT8 ucStatus)
{
    switch(ucStatus){
        case APN_ERR_SUCCESS:
            return "No errors encountered";
        case APN_ERR_PROCESS:
            return "Processing error";
        case APN_ERR_MISS_DEV_TOKEN:
            return "Missing device token";
        case APN_ERR_MISS_TOPIC:
            return "Missing topic";
        case APN_ERR_MISS_PAYLOAD:
            return "Missing payload";
        case APN_ERR_INVALID_TOKEN_SIZE:
            return "Invalid token size";
        case APN_ERR_INVALID_TOPIC_SIZE:
            return "Invalid topic size";
        case APN_ERR_INVALID_PAYLOAD_SIZE:
            return "Invalid payload size";
        case APN_ERR_INVALID_TOKEN:
            return "Invalid token";
    }
    return "None (unknown)";
}
