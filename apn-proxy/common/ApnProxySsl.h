#ifndef __APN_PROXY_SSL_H__
#define __APN_PROXY_SSL_H__

#include "ApnProxyMacro.h"
#include "CwxCommon.h"
#include "CwxSockStream.h"
#include "CwxINetAddr.h"
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


///ssl连接对象
class ApnProxySsl{
public:
    ///构造函数
    ApnProxySsl(char const* host,
        CWX_UINT16 port,
        char const *cerfile,
        char const *keyfile,
        char const *capath);
    ///析构函数
    ~ApnProxySsl();
public:
    ///连接；返回值：0，成功；-1：失败
    int connect(CWX_UINT32 uiMilliTimeout=0, char* szErr2K=NULL);
    ///读取内容。返回值，-1：失败；0：没有数据；>0：收到的字节数。
    int read(char* szBuf, CWX_UINT32 uiBufLen, CWX_UINT32 uiMilliTimeout=0, char* szErr2K=NULL);
    ///发送内容。返回值，-1：失败；0：连接关闭；>0：发送的字节数
    int send (char const* szDat, CWX_UINT32 uiDatLen, char* szErr2K=NULL);
    ///读取内容。返回值，-1：失败；0：没有数据；>0：收到的字节数。
    int read_n(char* szBuf, CWX_UINT32 uiLen, CWX_UINT32 uiMilliTimeout=0, char* szErr2K=NULL);
    ///发送内容。返回值，-1：失败；0：连接关闭；>0：发送的字节数
    int send_n (char const* szDat, CWX_UINT32 uiDatLen, char* szErr2K=NULL);
    ///检查是否可读
    bool isReadReady(CWX_UINT32 uiMilliTimeout=0);
    ///关闭连接
    void disconnect();
    ///是否连接
    inline bool isConnected() const{
        return m_bConnected;
    }
private:
    string                  m_strHost; ///<主机地址
    CWX_UINT16              m_unPort;  ///<端口信息
    string                  m_strCerFile; ///<认证文件
    string                  m_strKeyFile; ///<key文件
    string                  m_strCaPath; ///<认证的目录
    SSL_CTX*                m_ctx; ///<ssl的context
    SSL*                    m_ssl;  ///<ssl的连接
    SSL_METHOD*             m_method; ///<ssl的method
    X509*                   m_cert; ///<ssl的证书
    EVP_PKEY*               m_key; ///<ssl的key
    CwxSockStream           m_stream; ///<socket的连接
    bool                    m_bConnected; ///<连接是否建立
};

#endif
