#ifndef __APN_PROXY_CONFIG_H__
#define __APN_PROXY_CONFIG_H__

#include "CwxGlobalMacro.h"
#include "CwxHostInfo.h"
#include "CwxCommon.h"
#include "CwxIniParse.h"
#include "ApnProxyMacro.h"

///channel�Ķ���
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
    string          m_strChannelName; ///<channel������
    string          m_strApps;       ///<app���б�
    CWX_UINT16      m_unThreadNum; ///<ͨ�����߳�����
    bool            m_bRelease; ///<�Ƿ����release����
};

//app�Ķ���
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
    string          m_strAppName; ///<app������
    list<string>    m_channels; ///<channel������
    string          m_strCertFile; ///<cert�ļ�������
    string          m_strKeyFile; ///<key�ļ�������
    string          m_strCaPath; ///<ca��path
};

///app��channel�Ķ���
class ApnProxyConfigChannelApp{
public:
    ///���캯��
    ApnProxyConfigChannelApp(){}
public:
    ///���Ʋ���
    ApnProxyConfigChannelApp& operator=(ApnProxyConfigChannelApp const& item){
        if (this != &item){
            m_strAppName = item.m_strAppName;
            m_strChannelName = item.m_strChannelName;
        }
        return *this;
    }
    ///equal�Ƚϲ���
    bool operator == (ApnProxyConfigChannelApp const& item) const{
        return (m_strAppName==item.m_strAppName) && (m_strChannelName == item.m_strChannelName);
    }
    ///<�Ƚϲ���
    bool operator < (ApnProxyConfigChannelApp const& item) const{
        if (m_strChannelName < item.m_strChannelName) return true;
        if (m_strChannelName > item.m_strChannelName) return false;
        return m_strAppName < item.m_strAppName;
    }
public:
    string          m_strAppName; ///<app������
    string          m_strChannelName; ///<channel������
};

///�����ļ����ض���
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
            map<string, ApnProxyConfigChannel*>::iterator iter = m_channels.begin(); ///<channel�Ķ���
            while(iter != m_channels.end()){
                delete iter->second;
                iter++;
            }

        }
        {
            map<string, ApnProxyConfigApp*>::iterator iter = m_apps.begin(); ///<���е�app
            while(iter != m_apps.end()){
                delete iter->second;
                iter++;
            }

        }

    }
public:
    //���������ļ�.-1:failure, 0:success
    int loadConfig(string const & strConfFile);
    //������ص������ļ���Ϣ
    void outputConfig();
    //��ȡ�����ļ����ص�ʧ��ԭ��
    char const* getError() { return m_szError; };
public:
    string              m_strWorkDir;///<����Ŀ¼
    CwxHostInfo         m_listen;///<tcp�ļ���ip/port
    CWX_UINT32          m_uiConnTimeoutMilliSecond; ///<���ӵĳ�ʱʱ��
    CWX_UINT32          m_uiCheckMilliSecond; ///<���ͳɹ�����ʱ��
    map<string, ApnProxyConfigChannel*>  m_channels; ///<channel�Ķ���
    map<string, ApnProxyConfigApp*> m_apps; ///<���е�app
    map<ApnProxyConfigChannelApp, ApnProxyConfigApp*>   m_channelApps; ///<channel app�ļ���
    char                m_szError[2048];///<������Ϣ��buf
};

#endif
