#ifndef __APN_PROXY_TSS_H__
#define __APN_PROXY_TSS_H__
/*
��Ȩ������
�������ѭGNU GPL V3��http://www.gnu.org/licenses/gpl.html����
��ϵ��ʽ��email:cwinux@gmail.com��΢��:http://t.sina.com.cn/cwinux
*/

#include "ApnProxyMacro.h"
#include "CwxLogger.h"
#include "CwxTss.h"
#include "CwxPackageReader.h"
#include "CwxPackageWriter.h"
#include "ApnProxySsl.h"
#include "ApnProxyConfig.h"

///apn��������Ϣ
class ApnProxySslInfo{
public:
    ApnProxySslInfo(){
        m_ssl = NULL;
        m_uiLastSendId = 0;
    }
public:
    ApnProxySsl*    m_ssl;  ///<apn��ssl����
    CWX_UINT32      m_uiLastSendId; ///<��һ�η��͵�id
    string          m_strLastSendDevId; ///<��һ�η��͵��豸id
    string          m_strLastSendContent; ///<��һ�η��͵�����
};
//Apn-proxy��tss
class ApnProxyTss:public CwxTss{
public:
    enum{
        DEF_PACKAGE_SIZE = 1024
    };
public:
    ///���캯��
    ApnProxyTss():CwxTss(){
    }
    ///��������
    ~ApnProxyTss();
public:
    ///tss�ĳ�ʼ����0���ɹ���-1��ʧ��
    int init(ApnProxyConfigChannel const* channel,
        map<ApnProxyConfigChannelApp, ApnProxyConfigApp*> const* channelApp);
public:
    CwxPackageReader*         m_pReader; ///<���ݰ��Ľ������
    CwxPackageWriter*         m_pWriter; ///<���ݰ���pack����
    map<string, ApnProxySslInfo*> m_appSsl; ///<app��ssl����
};

#endif
