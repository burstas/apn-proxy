#ifndef __APN_PROXY_CONFIG_H__
#define __APN_PROXY_CONFIG_H__

#include "CwxGlobalMacro.h"
#include "CwxHostInfo.h"
#include "CwxCommon.h"
#include "CwxIniParse.h"
#include "ApnProxyMacro.h"

///channel的定义
class ApnProxyConfigChannel{
public:
    ApnProxyConfigChannel(){
        m_unThreadNum = 1;
        m_bRelease = true;
    }
public:
    ApnProxyConfigChannel& operator=(ApnProxyConfigChannel const& item){
        if (this != &item){
            m_strChannelName = item.m_strChannelName;
            m_unThreadNum = item.m_unThreadNum;
            m_bRelease = item.m_bRelease;
        }
        return *this;
    }
public:
    string          m_strChannelName; ///<channel的名字
    CWX_UINT16      m_unThreadNum; ///<通道的线程数量
    bool            m_bRelease; ///<是否采用release配置
};

//app的定义
class ApnProxyConfigApp{
public:
    ApnProxyConfigApp(){}
public:
    ApnProxyConfigApp& operator = (ApnProxyConfigApp const& item){
        if (this != &item){
            m_strAppName = item.m_strAppName;
            m_channels = item.m_channels;
            m_strCertFile = item.m_strCertFile;
            m_strKeyFile = item.m_strKeyFile;
            m_strCaPath = item.m_strCaPath;
        }
        return *this;
    }
public:
    string          m_strAppName; ///<app的名字
    list<string>    m_channels; ///<channel的名字
    string          m_strCertFile; ///<cert文件的名字
    string          m_strKeyFile; ///<key文件的名字
    string          m_strCaPath; ///<ca的path
};

///app与channel的对象
class ApnProxyConfigAppChannel{
public:
    ///构造函数
    ApnProxyConfigAppChannel(){}
public:
    ///复制操作
    ApnProxyConfigAppChannel& operator=(ApnProxyConfigAppChannel const& item){
        if (this != &item){
            m_strAppName = item.m_strAppName;
            m_strChannelName = item.m_strChannelName;
        }
        return *this;
    }
    ///equal比较操作
    bool operator == (ApnProxyConfigAppChannel const& item) const{
        return (m_strAppName==item.m_strAppName) && (m_strChannelName == item.m_strChannelName);
    }
    ///<比较操作
    bool operator < (ApnProxyConfigAppChannel const& item) const{
        if (m_strAppName < item.m_strAppName) return true;
        if (m_strAppName > item.m_strAppName) return false;
        return m_strChannelName < item.m_strChannelName;
    }
public:
    string          m_strAppName; ///<app的名字
    string          m_strChannelName; ///<channel的名字
};

///配置文件加载对象
class ApnProxyConfig
{
public:
    ApnProxyConfig(){
        m_uiCheckMilliSecond = 1;
    }
    
    ~ApnProxyConfig(){}
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
    CWX_UINT32          m_uiCheckMilliSecond; ///<发送成功检测的时间
    map<string, ApnProxyConfigChannel*>  m_channels; ///<channel的定义
    map<string, ApnProxyConfigApp*> m_apps; ///<所有的app
    set<ApnProxyConfigAppChannel>   m_appChannel; ///<app channel的集合
    char                m_szError[2048];///<错误消息的buf
};

#endif
