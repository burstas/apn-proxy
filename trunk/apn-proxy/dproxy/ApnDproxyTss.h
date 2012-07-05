#ifndef __APN_DPROXY_TSS_H__
#define __APN_DPROXY_TSS_H__
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
class ApnDproxySslInfo{
public:
    ApnDproxySslInfo(){
        m_ssl = NULL;
        m_uiLastSendId = 0;
    }
public:
    ApnProxySsl*    m_ssl;  ///<apn��ssl����
    CWX_UINT32      m_uiLastSendId; ///<��һ�η��͵�id
    string          m_strLastSendDevId; ///<��һ�η��͵��豸id
    string          m_strLastSendContent; ///<��һ�η��͵�����
};
//Apn-dproxy��tss
class ApnDproxyTss:public CwxTss{
public:
    enum{
        DEF_PACKAGE_SIZE = 1024
    };
public:
    ///���캯��
    ApnDproxyTss():CwxTss(){
    }
    ///��������
    ~ApnDproxyTss();
public:
    ///tss�ĳ�ʼ����0���ɹ���-1��ʧ��
    int init();
public:
    CwxPackageReader*         m_pReader; ///<���ݰ��Ľ������
    CwxPackageWriter*         m_pWriter; ///<���ݰ���pack����
    map<string, ApnDproxySslInfo*> m_appSsl; ///<app��ssl����
};

#endif
