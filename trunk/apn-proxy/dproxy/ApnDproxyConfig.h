#ifndef __APN_DPROXY_CONFIG_H__
#define __APN_DPROXY_CONFIG_H__
/*
版权声明：
本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
*/
#include "CwxGlobalMacro.h"
#include "CwxHostInfo.h"
#include "CwxCommon.h"
#include "CwxIniParse.h"
#include "ApnProxyMacro.h"
#include "CwxLogger.h"

///配置文件加载对象
class ApnDproxyConfig{
public:
    enum{
        APN_DPROXY_DEF_CHECK_MILLI_SEC = 10,
        APN_DPROXY_DEF_CONN_TIMEOUT_MILLI_SEC = 1000
    };
public:
    ApnDproxyConfig(){
        m_uiCheckMilliSecond = 10;
        m_uiConnTimeoutMilliSecond = 1000;
    }
    
    ~ApnDproxyConfig(){
    }
public:
    //加载配置文件.-1:failure, 0:success
    int loadConfig(string const & strConfFile);
    //输出加载的配置文件信息
    void outputConfig();
    //获取配置文件加载的失败原因
    char const* getError() { return m_szError; };
public:
    string              m_strWorkDir;///<工作目录
    CwxHostInfo         m_listen;///<tcp的监听ip/port
    CWX_UINT32          m_uiConnTimeoutMilliSecond; ///<连接的超时时间
    CWX_UINT32          m_uiCheckMilliSecond; ///<发送成功检测的时间
    CWX_UINT32          m_uiPartition; ///<所属的分区
    string			    m_strMyHost; ///<mysql的server
    CWX_UINT16          m_unMyPort; ///<mysql的port
    string			    m_strMyUser; ///<mysql的用户
    string			    m_strMyPasswd; ///<mysql的用户
    string			    m_strMyDb;  ///<mysql的数据库
    char                m_szError[2048];///<错误消息的buf
};

#endif
