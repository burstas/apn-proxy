#include "ApnProxySsl.h"

ApnProxySsl::ApnProxySsl(char const* host,
                         CWX_UINT16 port,
                         char const *cerfile,
                         char const *keyfile,
                         char const *capath)
{
    m_strHost = host;
    m_unPort = port;
    m_strCerFile = cerfile;
    m_strKeyFile = keyfile;
    m_strCaPath = capath;
    m_ctx = NULL;
    m_ssl = NULL;
    m_method = NULL;
    m_cert = NULL;
    m_key = NULL;
    m_bConnected = false;
}

///析构函数
ApnProxySsl::~ApnProxySsl(){
    disconnect();
}

///连接；返回值：0，成功；-1：失败
int ApnProxySsl::connect(CWX_UINT32 uiMilliTimeout, char* szErr2K){
    int ret = 0;
    ///确保连接关闭
    disconnect();
    /* Load encryption & hashing algorithms for the SSL program */
    SSL_library_init();
    /* Load the error strings for SSL & CRYPTO APIs */
    SSL_load_error_strings();
    /* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
    m_method = SSLv3_method();
    /* Create an SSL_CTX structure */
    m_ctx = SSL_CTX_new(m_method);                        
    if(!m_ctx){
        if (szErr2K) CwxCommon::snprintf(szErr2K, 2048, "Failure to create SSL Context, err=%d", errno);
        return -1;
    }
    /* Load the CA from the Path */
    if(SSL_CTX_load_verify_locations(m_ctx, NULL, m_strCaPath.c_str()) <= 0){
        if (szErr2K) ERR_error_string_n(ERR_get_error(), szErr2K, 2047);
        return -1;
    }
    /* Load the client certificate into the SSL_CTX structure */
    if (SSL_CTX_use_certificate_file(m_ctx, m_strCerFile.c_str, SSL_FILETYPE_PEM) <= 0) {
        if (szErr2K) ERR_error_string_n(ERR_get_error(), szErr2K, 2047);
        return -1;
    }
    /* Load the private-key corresponding to the client certificate */
    if (SSL_CTX_use_PrivateKey_file(m_ctx, m_strKeyFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
        if (szErr2K) ERR_error_string_n(ERR_get_error(), szErr2K, 2047);
        return -1;
    }
    /* Check if the client certificate and private-key matches */
    if (!SSL_CTX_check_private_key(m_ctx)) {
        if (szErr2K) ERR_error_string_n(ERR_get_error(), szErr2K, 2047);
        return -1;
    }
    CwxINetAddr addr;
    if (0 != addr.set(m_unPort, m_strHost.c_str())){
        if (szErr2K) CwxCommon::snprintf(szErr2K,
            2047,
            "Failure to init addr, addr:%s, port:%u, err=%d",
            m_strHost.c_str(),
            m_unPort,
            errno);
        return -1;
    }
    if (m_stream.open(addr.getType(), SOCK_STREAM, 0, true) == -1){
        if (szErr2K) CwxCommon::snprintf(szErr2K,
            2047,
            "Failure to open socket, addr:%s, port:%u, err=%d",
            m_strHost.c_str(),
            m_unPort,
            errno);
        return -1;
    }
    CwxSockConnector conn;
    CwxTimeValue timeValue(uiMilliTimeout/1000, (uiMilliTimeout%1000)*1000);
    CwxTimeouter timer(&timeValue);
    if (0 != conn.connect(m_stream, addr, CwxAddr::sap_any, uiMilliTimeout?&timer:NULL)){
        if (szErr2K) CwxCommon::snprintf(szErr2K,
            2047,
            "Failure to connect host:port, addr:%s, port:%u, err=%d",
            m_strHost.c_str(),
            m_unPort,
            errno);
        return 1;
    }
    /* An SSL structure is created */
    m_ssl = SSL_new(m_ctx);
    if(!m_ssl){
        if (szErr2K) CwxCommon::snprintf(szErr2K,
            2047,
            "Could not get SSL Socket, addr:%s, port:%u",
            m_strHost.c_str(),
            m_unPort);
        return -1;
    }    

    /* Assign the socket into the SSL structure (SSL and socket without BIO) */
    SSL_set_fd(m_ssl, m_stream.getHandle);

    /* Perform SSL Handshake on the SSL client */
    ret = SSL_connect(m_ssl);
    if(ret <= 0){
        if (szErr2K) CwxCommon::snprintf(szErr2K,
            2047,
            "Could not connect to SSL Server, addr:%s, port:%u",
            m_strHost.c_str(),
            m_unPort);
        return -1;
    }
    m_bConnected = true;
    return 0;
}

///读取内容。返回值，-1：失败；0：没有数据；>0：收到的字节数。
int ApnProxySsl::read(char* szBuf, CWX_UINT32 uiBufLen, CWX_UINT32 uiMilliTimeout, char* szErr2K){
    if ((-1 == m_stream.getHandle()) || !m_ssl){
        if (szErr2K) strcpy(szErr2K, "No connect.");
        return -1;
    }
    if (uiMilliTimeout){
        CwxTimeValue timeValue(uiMilliTimeout/1000, (uiMilliTimeout%1000)*1000);
        CwxTimeouter timer(&timeValue);
        if (CwxSocket::handleReady(m_stream.getHandle(), &timer, true, false, false, true)<=0)
            return 0;
    }
    int ret = SSL_read(m_ssl, szBuf, uiBufLen);
    if (0 > ret){
        if (szErr2K) ERR_error_string_n(SSL_get_error(m_ssl, ret), szErr2K, 2047);
        return -1;
    }else if (0 == ret){
        if (szErr2K) strcpy(szErr2K, "Connection is closed.");
        return -1;
    }
    return ret;
}
///发送内容。返回值，-1：失败；0：连接关闭；>0：发送的字节数
int ApnProxySsl::send (char const* szDat, CWX_UINT32 uiDatLen, char* szErr2K){
    if ((-1 == m_stream.getHandle()) || !m_ssl){
        if (szErr2K) strcpy(szErr2K, "No connect.");
        return -1;
    }
    int ret = SSL_write(m_ssl, szDat, uiDatLen);
    if (0 > ret){
        if (szErr2K) ERR_error_string_n(SSL_get_error(m_ssl, ret), szErr2K, 2047);
        return -1;
    }else if (0 == ret){
        if (szErr2K) strcpy(szErr2K, "Connection is closed.");
        return 0;
    }
    return ret;

}

///读取内容。返回值，-1：失败；0：没有数据；>0：收到的字节数。
int ApnProxySsl::read_n(char* szBuf, CWX_UINT32 uiLen, CWX_UINT32 uiMilliTimeout, char* szErr2K)
{
    if ((-1 == m_stream.getHandle()) || !m_ssl){
        if (szErr2K) strcpy(szErr2K, "No connect.");
        return -1;
    }
    CwxTimeValue timeValue(uiMilliTimeout/1000, (uiMilliTimeout%1000)*1000);
    CwxTimeouter timer(&timeValue);
    int ret = 0;
    CWX_UINT32 uiPos = 0;
    while(1){
        if (uiMilliTimeout){
            if (CwxSocket::handleReady(m_stream.getHandle(), &timer, true, false, false, true)<=0)
                return uiPos;
            if (timer.timeout()) return uiPos;
        }
        ret = SSL_read(m_ssl, szBuf + uiPos, uiLen - uiPos);
        if (0 > ret){
            if (szErr2K) ERR_error_string_n(SSL_get_error(m_ssl, ret), szErr2K, 2047);
            return -1;
        }else if (0 == ret){
            if (szErr2K) strcpy(szErr2K, "Connection is closed.");
            return -1;
        }
        uiPos += ret;
        if (uiPos == uiLen) break;
    }
    return uiPos;
}
///发送内容。返回值，-1：失败；0：连接关闭；>0：发送的字节数
int ApnProxySsl::send_n (char const* szDat, CWX_UINT32 uiDatLen, char* szErr2K){
    if ((-1 == m_stream.getHandle()) || !m_ssl){
        if (szErr2K) strcpy(szErr2K, "No connect.");
        return -1;
    }
    int ret = 0;
    CWX_UINT32 uiPos = 0;
    while(1){
        ret = SSL_write(m_ssl, szDat + uiPos, uiDatLen - uiPos);
        if (0 > ret){
            if (szErr2K) ERR_error_string_n(SSL_get_error(m_ssl, ret), szErr2K, 2047);
            return -1;
        }else if (0 == ret){
            if (szErr2K) strcpy(szErr2K, "Connection is closed.");
            return 0;
        }
        uiDatLen -= ret;
        if (!uiDatLen) break;
    }
    return uiDatLen;

}

///检查是否可读
bool ApnProxySsl::isReadReady(CWX_UINT32 uiMilliTimeout){
    if ((-1 == m_stream.getHandle()) || !m_ssl){
        return true;
    }
    CwxTimeValue timeValue(uiMilliTimeout/1000, (uiMilliTimeout%1000)*1000);
    CwxTimeouter timer(&timeValue);
    if (uiMilliTimeout){
        if (CwxSocket::handleReady(m_stream.getHandle(), &timer, true, false, false, true)<=0)
            return true;
        if (timer.timeout()) return false;
        return true;
    }
    CwxSocket::handleReady(m_stream.getHandle(), NULL, true, false, false, true);
    return true;
}

///关闭连接
void ApnProxySsl::disconnect(){
    int ret=0;
    if (!m_method) return;
    m_bConnected = false;
    m_method = NULL;
    if (m_ssl){
        SSL_shutdown(m_ssl);
        SSL_free(m_ssl);
        m_ssl = NULL;
    }
    /* Free the SSL_CTX structure */
    if (m_ctx){
        SSL_CTX_free(m_ctx);
        m_ctx = NULL;
    }
    /* Terminate communication on a socket */
    m_stream.close();
}
