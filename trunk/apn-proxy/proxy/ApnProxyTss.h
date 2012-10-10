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

///apn的连接信息
class ApnProxySslInfo{
public:
    ApnProxySslInfo(){
        m_ssl = NULL;
        m_uiLastSendId = 0;
    }
public:
    ApnProxySsl*    m_ssl;  ///<apn的ssl连接
    CWX_UINT32      m_uiLastSendId; ///<上一次发送的id
    string          m_strLastSendDevId; ///<上一次发送的设备id
    string          m_strLastSendContent; ///<上一次发送的内容
};
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
public:
    CwxPackageReader*         m_pReader; ///<数据包的解包对象
    CwxPackageWriter*         m_pWriter; ///<数据包的pack对象
    map<string, ApnProxySslInfo*> m_appSsl; ///<app的ssl对象
};

#endif
