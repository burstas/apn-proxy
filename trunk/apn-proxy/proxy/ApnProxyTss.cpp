#include "ApnProxyTss.h"

///构造函数
ApnProxyTss::~ApnProxyTss(){
    if (m_pReader) delete m_pReader;
    if (m_pWriter) delete m_pWriter;
    map<string, ApnProxySslInfo*>::iterator iter = m_appSsl.begin();
    while(iter != m_appSsl.end()){
        delete iter->second->m_ssl;
        delete iter->second;
        iter++;
    }
}

int ApnProxyTss::init(ApnProxyConfigChannel const* channel,
                     map<ApnProxyConfigChannelApp, ApnProxyConfigApp*> const* channelApp)
{
    m_pReader = new CwxPackageReader(false);
    m_pWriter = new CwxPackageWriter(DEF_PACKAGE_SIZE);
    if (channel){
        ApnProxyConfigChannelApp obj;
        obj.m_strChannelName = channel->m_strChannelName;
        obj.m_strAppName = "";
        ApnProxySsl* ssl = NULL;
        ApnProxySslInfo* sslInfo = NULL;
        map<ApnProxyConfigChannelApp, ApnProxyConfigApp*>::const_iterator iter = channelApp->upper_bound(obj);
        while(iter != channelApp->end()){
            if (channel->m_bRelease){
                ssl = new ApnProxySsl(APN_PROXY_RELEASE_HOST,
                    APN_PROXY_RELEASE_PORT,
                    iter->second->m_strCertFile.c_str(),
                    iter->second->m_strKeyFile.c_str(),
                    iter->second->m_strCaPath.c_str());
            }else{
                ssl = new ApnProxySsl(APN_PROXY_DEV_HOST,
                    APN_PROXY_DEV_PORT,
                    iter->second->m_strCertFile.c_str(),
                    iter->second->m_strKeyFile.c_str(),
                    iter->second->m_strCaPath.c_str());
            }
            sslInfo = new ApnProxySslInfo();
            sslInfo->m_ssl = ssl;
            m_appSsl[iter->second->m_strAppName] = sslInfo;
            iter++;
        }
    }
    return 0;
}
