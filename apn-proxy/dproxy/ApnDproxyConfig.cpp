#include "ApnDproxyConfig.h"
#include "CwxFile.h"

static bool parseHostPort(string const& strHostPort, CwxHostInfo& host)
{
    if ((strHostPort.find(':') == string::npos) || (0 == strHostPort.find(':'))) return false;
    host.setHostName(strHostPort.substr(0, strHostPort.find(':')));
    host.setPort(strtoul(strHostPort.substr(strHostPort.find(':')+1).c_str(), NULL, 10));
    return true;
}

int ApnDproxyConfig::loadConfig(string const & strConfFile){
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
    if (!m_uiConnTimeoutMilliSecond) m_uiConnTimeoutMilliSecond = APN_DPROXY_DEF_CONN_TIMEOUT_MILLI_SEC;

    // load query check_milli_second
	if (!parser.getAttr("common", "check_milli_second", value) || !value.length()){
		snprintf(m_szError, 2047, "Must set [common:check_milli_second].");
		return -1;
	}
	m_uiCheckMilliSecond = strtoul(value.c_str(), NULL, 10);
    if (!m_uiCheckMilliSecond) m_uiCheckMilliSecond = APN_DPROXY_DEF_CHECK_MILLI_SEC;
    // load partition
    if (!parser.getAttr("common", "partition", value) || !value.length()){
        snprintf(m_szError, 2047, "Must set [common:partition].");
        return -1;
    }
    m_uiPartition = strtoul(value.c_str(), NULL, 10);

	//load listen
	if (!parser.getAttr("common", "listen", value) || !value.length()){
		snprintf(m_szError, 2047, "Must set [common:listen].");
		return -1;
	}
    if (!parseHostPort(value, m_listen)){
        snprintf(m_szError, 2047, "common:listen is invalid, must be host:port");
        return -1;
    }

    //load mysql
    //load mysql:server
    if (!parser.getAttr("mysql", "server", value) || !value.length()){
        snprintf(m_szError, 2047, "Must set [mysql:server] for mysql server.");
        return -1;
    }
    m_strMyHost = value;

    //load mysql:port
    if (!parser.getAttr("mysql", "port", value) || !value.length()){
        snprintf(m_szError, 2047, "Must set [mysql:port] for mysql port.");
        return -1;
    }
    m_unMyPort = strtoul(value.c_str(), NULL, 10);

    //load mysql:user
    if (!parser.getAttr("mysql", "user", value) || !value.length()){
        snprintf(m_szError, 2047, "Must set [mysql:user] for mysql user.");
        return -1;
    }
    m_strMyUser = value;
    //load mysql:passwd
    if (!parser.getAttr("mysql", "passwd", value) || !value.length()){
        snprintf(m_szError, 2047, "Must set [mysql:passwd] for mysql passwd.");
        return -1;
    }
    m_strMyPasswd = value;
    //load mysql:db
    if (!parser.getAttr("mysql", "db", value) || !value.length()){
        snprintf(m_szError, 2047, "Must set [mysql:db] for mysql database name.");
        return -1;
    }
    m_strMyDb = value;

	return 0;
}

void ApnDproxyConfig::outputConfig(){
    CWX_INFO(("*****************begin common conf*******************"));
    CWX_INFO(("home=%s", m_strWorkDir.c_str()));
    CWX_INFO(("listen=%s:%u", m_listen.getHostName().c_str(), m_listen.getPort()));
    CWX_INFO(("conn_connect_milli_second=%u", m_uiConnTimeoutMilliSecond));
    CWX_INFO(("check_milli_second=%u", m_uiCheckMilliSecond));
    CWX_INFO(("partition=%u", m_uiPartition));
    CWX_INFO(("*****************end common conf*******************"));
    CWX_INFO(("*****************begin mysql conf*******************"));
    CWX_INFO(("server=%s", m_strMyHost.c_str()));
    CWX_INFO(("port=%u", m_unMyPort));
    CWX_INFO(("usr=%s", m_strMyUser.c_str()));
    CWX_INFO(("passwd=%s", m_strMyPasswd.c_str()));
    CWX_INFO(("db=%s", m_strMyDb.c_str()));
    CWX_INFO(("*****************end mysql conf*******************"));


}
