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
            m_strApps = item.m_strApps;
        }
        return *this;
    }
public:
    string          m_strChannelName; ///<channel的名字
    string          m_strApps;       ///<app的列表
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
class ApnProxyConfigChannelApp{
public:
    ///构造函数
    ApnProxyConfigChannelApp(){}
public:
    ///复制操作
    ApnProxyConfigChannelApp& operator=(ApnProxyConfigChannelApp const& item){
        if (this != &item){
            m_strAppName = item.m_strAppName;
            m_strChannelName = item.m_strChannelName;
        }
        return *this;
    }
    ///equal比较操作
    bool operator == (ApnProxyConfigChannelApp const& item) const{
        return (m_strAppName==item.m_strAppName) && (m_strChannelName == item.m_strChannelName);
    }
    ///<比较操作
    bool operator < (ApnProxyConfigChannelApp const& item) const{
        if (m_strChannelName < item.m_strChannelName) return true;
        if (m_strChannelName > item.m_strChannelName) return false;
        return m_strAppName < item.m_strAppName;
    }
public:
    string          m_strAppName; ///<app的名字
    string          m_strChannelName; ///<channel的名字
};

///配置文件加载对象
class ApnProxyConfig{
public:
    enum{
        APN_PROXY_DEF_CHECK_MILLI_SEC = 10,
        APN_PROXY_DEF_CONN_TIMEOUT_MILLI_SEC = 1000
    };
public:
    ApnProxyConfig(){
        m_uiCheckMilliSecond = 10;
        m_uiConnTimeoutMilliSecond = 1000;
    }
    
    ~ApnProxyConfig(){
        {
            map<string, ApnProxyConfigChannel*>::iterator iter = m_channels.begin(); ///<channel的定义
            while(iter != m_channels.end()){
                delete iter->second;
                iter++;
            }

        }
        {
            map<string, ApnProxyConfigApp*>::iterator iter = m_apps.begin(); ///<所有的app
            while(iter != m_apps.end()){
                delete iter->second;
                iter++;
            }

        }

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
    map<string, ApnProxyConfigChannel*>  m_channels; ///<channel的定义
    map<string, ApnProxyConfigApp*> m_apps; ///<所有的app
    map<ApnProxyConfigChannelApp, ApnProxyConfigApp*>   m_channelApps; ///<channel app的集合
    char                m_szError[2048];///<错误消息的buf
};

#endif
