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
    ///��ȡpackage��buf������NULL��ʾʧ��
    inline char* getBuf(CWX_UINT32 uiSize){
        if (m_uiDataBufLen < uiSize){
            delete [] m_szDataBuf;
            m_szDataBuf = new char[uiSize];
            m_uiDataBufLen = uiSize;
        }
        return m_szDataBuf;
    }

public:
    CwxPackageReader*         m_pReader; ///<���ݰ��Ľ������
    CwxPackageWriter*         m_pWriter; ///<���ݰ���pack����
    map<string, ApnProxySsl*> m_appSsl; ///<app��ssl����
private:
    char*                  m_szDataBuf; ///<����buf
    CWX_UINT32             m_uiDataBufLen; ///<����buf�Ŀռ��С
};

#endif
