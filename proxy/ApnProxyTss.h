#ifndef __APN_PROXY_TSS_H__
#define __APN_PROXY_TSS_H__
/*
版权声明：
本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
*/

#include "ApnProxyMacro.h"
#include "CwxLogger.h"
#include "CwxTss.h"
#include "CwxPackageReader.h"
#include "CwxPackageWriter.h"
#include "ApnProxySsl.h"
#include "ApnProxyConfig.h"

//Apn-proxy的tss
class ApnProxyTss:public CwxTss{
public:
    enum{
        DEF_PACKAGE_SIZE = 1024
    };
public:
    ///构造函数
    ApnProxyTss():CwxTss(){
    }
    ///析构函数
    ~ApnProxyTss();
public:
    ///tss的初始化，0：成功；-1：失败
    int init(ApnProxyConfigChannel const* channel,
        map<ApnProxyConfigChannelApp, ApnProxyConfigApp*> const* channelApp);
    ///获取package的buf，返回NULL表示失败
    inline char* getBuf(CWX_UINT32 uiSize){
        if (m_uiDataBufLen < uiSize){
            delete [] m_szDataBuf;
            m_szDataBuf = new char[uiSize];
            m_uiDataBufLen = uiSize;
        }
        return m_szDataBuf;
    }

public:
    CwxPackageReader*         m_pReader; ///<数据包的解包对象
    CwxPackageWriter*         m_pWriter; ///<数据包的pack对象
    map<string, ApnProxySsl*> m_appSsl; ///<app的ssl对象
private:
    char*                  m_szDataBuf; ///<数据buf
    CWX_UINT32             m_uiDataBufLen; ///<数据buf的空间大小
};

#endif
