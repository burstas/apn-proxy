#ifndef __APN_DPROXY_CONFIG_H__
#define __APN_DPROXY_CONFIG_H__
/*
��Ȩ������
�������ѭGNU GPL V3��http://www.gnu.org/licenses/gpl.html����
��ϵ��ʽ��email:cwinux@gmail.com��΢��:http://t.sina.com.cn/cwinux
*/
#include "CwxGlobalMacro.h"
#include "CwxHostInfo.h"
#include "CwxCommon.h"
#include "CwxIniParse.h"
#include "ApnProxyMacro.h"
#include "CwxLogger.h"

///�����ļ����ض���
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
    CWX_UINT32          m_uiPartition; ///<�����ķ���
    string			    m_strMyHost; ///<mysql��server
    CWX_UINT16          m_unMyPort; ///<mysql��port
    string			    m_strMyUser; ///<mysql���û�
    string			    m_strMyPasswd; ///<mysql���û�
    string			    m_strMyDb;  ///<mysql�����ݿ�
    char                m_szError[2048];///<������Ϣ��buf
};

#endif
