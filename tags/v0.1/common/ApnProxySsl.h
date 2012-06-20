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


///ssl���Ӷ���
class ApnProxySsl{
public:
    ///���캯��
    ApnProxySsl(char const* host,
        CWX_UINT16 port,
        char const *cerfile,
        char const *keyfile,
        char const *capath);
    ///��������
    ~ApnProxySsl();
public:
    ///���ӣ�����ֵ��0���ɹ���-1��ʧ��
    int connect(CWX_UINT32 uiMilliTimeout=0, char* szErr2K=NULL);
    ///��ȡ���ݡ�����ֵ��-1��ʧ�ܣ�0��û�����ݣ�>0���յ����ֽ�����
    int read(char* szBuf, CWX_UINT32 uiBufLen, CWX_UINT32 uiMilliTimeout=0, char* szErr2K=NULL);
    ///�������ݡ�����ֵ��-1��ʧ�ܣ�0�����ӹرգ�>0�����͵��ֽ���
    int send (char const* szDat, CWX_UINT32 uiDatLen, char* szErr2K=NULL);
    ///��ȡ���ݡ�����ֵ��-1��ʧ�ܣ�0��û�����ݣ�>0���յ����ֽ�����
    int read_n(char* szBuf, CWX_UINT32 uiLen, CWX_UINT32 uiMilliTimeout=0, char* szErr2K=NULL);
    ///�������ݡ�����ֵ��-1��ʧ�ܣ�0�����ӹرգ�>0�����͵��ֽ���
    int send_n (char const* szDat, CWX_UINT32 uiDatLen, char* szErr2K=NULL);
    ///����Ƿ�ɶ�
    bool isReadReady(CWX_UINT32 uiMilliTimeout=0);
    ///�ر�����
    void disconnect();
    ///�Ƿ�����
    inline bool isConnected() const{
        return m_bConnected;
    }
private:
    string                  m_strHost; ///<������ַ
    CWX_UINT16              m_unPort;  ///<�˿���Ϣ
    string                  m_strCerFile; ///<��֤�ļ�
    string                  m_strKeyFile; ///<key�ļ�
    string                  m_strCaPath; ///<��֤��Ŀ¼
    SSL_CTX*                m_ctx; ///<ssl��context
    SSL*                    m_ssl;  ///<ssl������
    SSL_METHOD*             m_method; ///<ssl��method
    X509*                   m_cert; ///<ssl��֤��
    EVP_PKEY*               m_key; ///<ssl��key
    CwxSockStream           m_stream; ///<socket������
    bool                    m_bConnected; ///<�����Ƿ���
};

#endif
