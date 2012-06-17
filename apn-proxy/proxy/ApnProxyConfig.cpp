#include "ApnProxyConfig.h"

static bool parseHostPort(string const& strHostPort, CwxHostInfo& host)
{
    if ((strHostPort.find(':') == string::npos) || (0 == strHostPort.find(':'))) return false;
    host.setHostName(strHostPort.substr(0, strHostPort.find(':')));
    host.setPort(strtoul(strHostPort.substr(strHostPort.find(':')+1).c_str(), NULL, 10));
    return true;
}

int ApnProxyConfig::loadConfig(string const & strConfFile){
	CwxIniParse parser;
	string value;
	//解析配置文件
	if (false == parser.load(strConfFile)){
		snprintf(m_szError, 2047, "Failure to Load conf file:%s", strConfFile.c_str());
		return -1;
	}
	//load home
	if (!parser.getAttr("common", "home", value)|| !value.length()){
		snprintf(m_szError, 2047, "Must set [common:home].");
		return -1;
	}
	if ('/' != value[value.length()-1]) value +="/";
	m_strWorkDir = value;

    // load query conn_connect_milli_second
    if (!parser.getAttr("common", "conn_connect_milli_second", value) || !value.length()){
        snprintf(m_szError, 2047, "Must set [common:conn_connect_milli_second].");
        return -1;
    }
    m_uiConnTimeoutMilliSecond = strtoul(value.c_str(), NULL, 10);

    // load query check_milli_second
	if (!parser.getAttr("common", "check_milli_second", value) || !value.length()){
		snprintf(m_szError, 2047, "Must set [common:check_milli_second].");
		return -1;
	}
	m_uiCheckMilliSecond = strtoul(value.c_str(), NULL, 10);

	//load listen
	if (!parser.getAttr("common", "listen", value) || !value.length()){
		snprintf(m_szError, 2047, "Must set [common:listen].");
		return -1;
	}
    if (!parseHostPort(value, m_listen)){
        snprintf(m_szError, 2047, "common:listen is invalid, must be host:port");
        return -1;
    }
    set<string> sessions;
    set<string>::iterator iter;
    ApnProxyConfigChannel* channel=NULL;
    ApnProxyConfigApp* app=NULL;
    parser.getSections(sessions);
    //获取所有的channel
    iter = sessions.begin();
    while(iter != sessions.end()){
        if ((iter->length() > strlen(APN_PROXY_CHANNEL_PREFIX)) &&
            (0 == memcmp(APN_PROXY_CHANNEL_PREFIX, iter->c_str(), strlen(APN_PROXY_CHANNEL_PREFIX))))
        {
            channel = new ApnProxyConfigChannel;
            channel->m_strChannelName = iter->c_str() + strlen(APN_PROXY_CHANNEL_PREFIX);
            if (m_channels.find(channel->m_strChannelName) != m_channels.end()){
                delete channel;
                snprintf(m_szError, 2047, "channel[%s] is duplicate.", iter->c_str());
                return -1;                
            }
            m_channels[channel->m_strChannelName] = channel;
            ///get thread
            if (!parser.getAttr(*iter, "thread", value) || !value.length()){
                snprintf(m_szError, 2047, "Must set [%s:thread].", iter->c_str());
                return -1;
            }
            channel->m_unThreadNum = strtoul(value.c_str(), NULL, 10);
            if (!channel->m_unThreadNum) channel->m_unThreadNum = 1;
            ///get type
            if (!parser.getAttr(*iter, "type", value) || !value.length()){
                snprintf(m_szError, 2047, "Must set [%s:type].", iter->c_str());
                return -1;
            }
            if (value == APN_PROXY_CHANNEL_TYPE_DEV){
                channel->m_bRelease = false;
            }else if (value == APN_PROXY_CHANNEL_TYPE_RELEASE){
                channel->m_bRelease = true;
            }else{
                snprintf(m_szError, 2047, "[%s:type]'s value is invalid, must be [release] or [dev].", iter->c_str());
                return -1;
            }
        }
        iter++;
    }
    //获取所有的app
    ApnProxyConfigApp* pApp = NULL;
    ApnProxyConfigAppChannel appChannel;
    iter = sessions.begin();
    while(iter != sessions.end()){
        if ((iter->length() > strlen(APN_PROXY_APP_PREFIX)) &&
            (0 == memcmp(APN_PROXY_APP_PREFIX, iter->c_str(), strlen(APN_PROXY_APP_PREFIX))))
        {
            pApp = new ApnProxyConfigApp;
            pApp->m_strAppName = iter->c_str() + strlen(APN_PROXY_APP_PREFIX);
            if (m_apps.find(pApp->m_strAppName) != pApp.end()){
                delete pApp;
                snprintf(m_szError, 2047, "pApp[%s] is duplicate.", iter->c_str());
                return -1;                
            }
            m_apps[pApp->m_strAppName] = pApp;
            ///get channel
            if (!parser.getAttr(*iter, "channel", value) || !value.length()){
                snprintf(m_szError, 2047, "Must set [%s:channel].", iter->c_str());
                return -1;
            }
            CwxCommon::split(value, pApp->m_channels, ',');
            list<string>::iterator ch_iter = pApp->m_channels.begin();
            if (!pApp->m_channels.size()){
                snprintf(m_szError, 2047, "Must set [%s:channel].", iter->c_str());
                return -1;
            }
            while(ch_iter != pApp->m_channels.end()){
                if (m_channels.find(*ch_iter) == m_channels.end()){
                    snprintf(m_szError, 2047, "channel[%s] set by [%s:channel] doesn't exist.", ch_iter->c_str(), iter->c_str());
                    return -1;
                }
                appChannel.m_strAppName = pApp->m_strAppName;
                appChannel.m_strChannelName = *ch_iter;
                m_appChannels[appChannel]=pApp;
                ch_iter++;
            }
            ///get cert_file
            if (!parser.getAttr(*iter, "cert_file", value) || !value.length()){
                snprintf(m_szError, 2047, "Must set [%s:cert_file].", iter->c_str());
                return -1;
            }
            if (!CwxFile::isFile(value)){
                snprintf(m_szError, 2047, "[%s:cert_file]'s cert_file doesn't exist, file:%s.", iter->c_str(), value.c_str());
                return -1;
            }
            pApp->m_strCertFile = value;
            ///get key_file
            if (!parser.getAttr(*iter, "key_file", value) || !value.length()){
                snprintf(m_szError, 2047, "Must set [%s:key_file].", iter->c_str());
                return -1;
            }
            if (!CwxFile::isFile(value)){
                snprintf(m_szError, 2047, "[%s:key_file]'s key_file doesn't exist, file:%s.", iter->c_str(), value.c_str());
                return -1;
            }
            pApp->m_strKeyFile = value;
            ///get ca_path
            if (!parser.getAttr(*iter, "ca_path", value) || !value.length()){
                snprintf(m_szError, 2047, "Must set [%s:ca_path].", iter->c_str());
                return -1;
            }
            if (!CwxFile::isDir(value)){
                snprintf(m_szError, 2047, "[%s:ca_path]'s path doesn't exist, path:%s.", iter->c_str(), value.c_str());
                return -1;
            }
        }
        iter++;
    }
    if (!m_channels.size()){
        snprintf(m_szError, 2047, "Not set channel");
        return -1;
    }
    if (!m_apps.size()){
        snprintf(m_szError, 2047, "Not set app");
        return -1;
    }

	return 0;
}

void ApnProxyConfig::outputConfig(){
    CWX_INFO(("*****************begin common conf*******************"));
    CWX_INFO(("home=%s", m_strWorkDir.c_str()));
    CWX_INFO(("listen=%s:%u", m_listen.getHostName().c_str(), m_listen.getPort()));
    CWX_INFO(("conn_connect_milli_second=%u", m_uiConnTimeoutMilliSecond));
    CWX_INFO(("check_milli_second=%u", m_uiCheckMilliSecond));
    CWX_INFO(("*****************end common conf*******************"));
    {
        map<string, ApnProxyConfigChannel*>::iterator iter = m_channels.begin();
        while(iter != m_channels.end()){
            CWX_INFO(("*****************begin channel[%s%s] conf*******************", APN_PROXY_CHANNEL_PREFIX, iter->first.c_str()));
            CWX_INFO(("thread=%u", iter->second->m_unThreadNum));
            CWX_INFO(("type=%s", iter->second->m_bRelease?APN_PROXY_CHANNEL_TYPE_RELEASE:APN_PROXY_CHANNEL_TYPE_DEV));
            CWX_INFO(("*****************end channel[%s%s] conf*******************", APN_PROXY_CHANNEL_PREFIX, iter->first.c_str()));
            iter++;
        }
    }
    {
        map<string, ApnProxyConfigApp*>::iterator iter = m_apps.begin();
        string strValue;
        list<string>::iterator ch_iter;
        while(iter != m_apps.end()){
            CWX_INFO(("*****************begin app[%s%s] conf*******************", APN_PROXY_APP_PREFIX, iter->first.c_str()));
            ch_iter = iter->second->m_channels.begin();
            strValue = "";
            while(ch_iter != iter->second->m_channels.end()){
                if (strValue.length()) strValue += ",";
                strValue += *ch_iter;
                ch_iter++;
            }
            CWX_INFO(("channel=%s", strValue.c_str()));
            CWX_INFO(("cert_file=%s", iter->second->m_strCertFile.c_str()));
            CWX_INFO(("key_file=%s", iter->second->m_strKeyFile.c_str()));
            CWX_INFO(("ca_path=%s", iter->second->m_strCaPath.c_str()));
            CWX_INFO(("*****************end channel[%s%s] conf*******************", APN_PROXY_APP_PREFIX, iter->first.c_str()));
            iter++;
        }
    }


}
