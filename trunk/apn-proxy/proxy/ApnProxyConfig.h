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
        }
        return *this;
    }
public:
    string          m_strChannelName; ///<channel������
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
class ApnProxyConfigAppChannel{
public:
    ///���캯��
    ApnProxyConfigAppChannel(){}
public:
    ///���Ʋ���
    ApnProxyConfigAppChannel& operator=(ApnProxyConfigAppChannel const& item){
        if (this != &item){
            m_strAppName = item.m_strAppName;
            m_strChannelName = item.m_strChannelName;
        }
        return *this;
    }
    ///equal�Ƚϲ���
    bool operator == (ApnProxyConfigAppChannel const& item) const{
        return (m_strAppName==item.m_strAppName) && (m_strChannelName == item.m_strChannelName);
    }
    ///<�Ƚϲ���
    bool operator < (ApnProxyConfigAppChannel const& item) const{
        if (m_strAppName < item.m_strAppName) return true;
        if (m_strAppName > item.m_strAppName) return false;
        return m_strChannelName < item.m_strChannelName;
    }
public:
    string          m_strAppName; ///<app������
    string          m_strChannelName; ///<channel������
};

///�����ļ����ض���
class ApnProxyConfig
{
public:
    ApnProxyConfig(){
        m_uiCheckMilliSecond = 1;
    }
    
    ~ApnProxyConfig(){}
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
    CWX_UINT32          m_uiCheckMilliSecond; ///<���ͳɹ�����ʱ��
    map<string, ApnProxyConfigChannel*>  m_channels; ///<channel�Ķ���
    map<string, ApnProxyConfigApp*> m_apps; ///<���е�app
    set<ApnProxyConfigAppChannel>   m_appChannel; ///<app channel�ļ���
    char                m_szError[2048];///<������Ϣ��buf
};

#endif
