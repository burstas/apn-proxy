#include "ApnProxyTss.h"

///¹¹Ôìº¯Êý
ApnProxyTss::~ApnProxyTss(){
    if (m_pReader) delete m_pReader;
    if (m_pWriter) delete m_pWriter;
	if (m_szDataBuf) delete [] m_szDataBuf;
    map<string, ApnProxySsl*>::iterator iter = m_appSsl.begin();
    while(iter != m_appSsl.end()){
        delete iter->second;
        iter++;
    }
}

int ApnProxyTss::init(ApnProxyConfigChannel const* channel,
                     map<ApnProxyConfigChannelApp, ApnProxyConfigApp*> const* channelApp)
{
    m_pReader = new CwxPackageReader(false);
    m_pWriter = new CwxPackageWriter(DEF_PACKAGE_SIZE);
    m_szDataBuf = new char[DEF_PACKAGE_SIZE];
    m_uiDataBufLen= DEF_PACKAGE_SIZE;
    if (channel){
        ApnProxyConfigChannelApp obj;
        obj.m_strChannelName = channel->m_strChannelName;
        obj.m_strAppName = "";
        ApnProxySsl* ssl;
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
            m_appSsl[iter->second->m_strAppName] = ssl;
            iter++;
        }
    }
    return 0;
}
