#ifndef __APN_PROXY_TSS_H__
#define __APN_PROXY_TSS_H__


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
    ///构造函数
    ApnProxyTss():CwxTss(){
    }
    ///析构函数
    ~ApnProxyTss();
public:
    ///tss的初始化，0：成功；-1：失败
    int init(list<);
    ///获取用户的数据
    UnistorTssUserObj* getUserObj() { return m_userObj;}
    ///获取一个write arg对象
    inline UnistorWriteMsgArg* popWriteMsgArg(){
        UnistorWriteMsgArg* arg = m_writeMsgHead;
        if (arg){
            m_writeMsgHead = m_writeMsgHead->m_next;
            arg->reset();
        }else{
            arg = new UnistorWriteMsgArg();
        }
        return arg;
    }
    ///释放一个write arg对象
    inline void pushWriteMsgArg(UnistorWriteMsgArg* arg){
        arg->m_next = m_writeMsgHead;
        m_writeMsgHead = arg;

    }
    ///获取package的buf，返回NULL表示失败
    inline char* getBuf(CWX_UINT32 uiSize){
        if (m_uiDataBufLen < uiSize){
            delete [] m_szDataBuf;
            m_szDataBuf = new char[uiSize];
            m_uiDataBufLen = uiSize;
        }
        return m_szDataBuf;
    }
    ///获取package的buf，返回NULL表示失败
    inline char* getBuf1(CWX_UINT32 uiSize){
        if (m_uiDataBufLen1 < uiSize){
            delete [] m_szDataBuf1;
            m_szDataBuf1 = new char[uiSize];
            m_uiDataBufLen1 = uiSize;
        }
        return m_szDataBuf1;
    }
    ///获取package的buf，返回NULL表示失败
    inline char* getBuf2(CWX_UINT32 uiSize){
        if (m_uiDataBufLen2 < uiSize){
            delete [] m_szDataBuf2;
            m_szDataBuf2 = new char[uiSize];
            m_uiDataBufLen2 = uiSize;
        }
        return m_szDataBuf2;
    }
    ///获取package的buf，返回NULL表示失败
    inline char* getBuf3(CWX_UINT32 uiSize){
        if (m_uiDataBufLen3 < uiSize){
            delete [] m_szDataBuf3;
            m_szDataBuf3 = new char[uiSize];
            m_uiDataBufLen3 = uiSize;
        }
        return m_szDataBuf3;
    }
    ///是否是master idc
    bool isMasterIdc(){
        if (m_pZkConf && m_pZkConf->m_bMasterIdc) return true;
        return false;
    }
    ///获取master idc的名字
    char const* getMasterIdc() const{
        if (m_pZkConf && m_pZkConf->m_strMasterIdc.length()) return m_pZkConf->m_strMasterIdc.c_str();
        return "";
    }
    ///自己是否是master
    bool isMaster(){
        if (m_pZkLock && m_pZkLock->m_bMaster) return true;
        return false;
    }
    ///是否存在master
    bool isExistMaster(){
        if (m_pZkLock && m_pZkLock->m_strMaster.length()) return true;
        return false;
    }
    ///获取master的主机名
    char const* getMasterHost() const{
        if (m_pZkLock && m_pZkLock->m_strMaster.length()) return m_pZkLock->m_strMaster.c_str();
        return "";
    }
    ///获取idc内的上一个sync的主机名
    char const* getSyncHost() const{
        if (m_pZkLock){
            if (m_pZkLock->m_strPrev.length()) return m_pZkLock->m_strPrev.c_str();
            if (m_pZkLock->m_strMaster.length()) return m_pZkLock->m_strMaster.c_str();
        }
        return "";
    }
    inline void resetStats(){
        ///统计初始化
        m_ullStatsGetNum = 0; ///<get查询的数量
        m_ullStatsGetReadCacheNum = 0; ///<get查询使用Cache数量
        m_ullStatsGetExistNum = 0; ///<get查询存在结果的数量
        m_ullStatsGetsNum = 0; ///<gets查询的数量
        m_ullStatsGetsKeyNum = 0; ///<gets的key的数量
        m_ullStatsGetsKeyReadCacheNum = 0; ///<gets的key的cache数量
        m_ullStatsGetsKeyExistNum = 0; ///<gets的key的存在的数量
        m_ullStatsListNum = 0; ///<list的数量
        m_ullStatsExistNum = 0; ///<exist的数量
        m_ullStatsExistReadCacheNum = 0; ///<exist的cache数量
        m_ullStatsExistExistNum = 0; ///<exist的存在的数量
        m_ullStatsAddNum = 0; ///<add的数量
        m_ullStatsAddReadCacheNum = 0; ///<add的read cache数量
        m_ullStatsAddWriteCacheNum = 0; ///<add的write cache数量
        m_ullStatsSetNum = 0; ///<set的数量
        m_ullStatsSetReadCacheNum = 0; ///<set的read cache数量
        m_ullStatsSetWriteCacheNum = 0; ///<set的write cache数量
        m_ullStatsUpdateNum = 0; ///<update的数量
        m_ullStatsUpdateReadCacheNum = 0; ///<update的read cache数量
        m_ullStatsUpdateWriteCacheNum = 0; ///<update的write cache数量
        m_ullStatsIncNum = 0; ///<inc的数量
        m_ullStatsIncReadCacheNum = 0; ///<inc的read cache数量
        m_ullStatsIncWriteCacheNum = 0; ///<inc的write cache数量
        m_ullStatsDelNum = 0; ///<del的数量
        m_ullStatsDelReadCacheNum = 0; ///<del的read cache数量
        m_ullStatsDelWriteCacheNum = 0; ///<del的write cache数量
        m_ullStatsImportNum = 0; ///<del的数量
        m_ullStatsImportReadCacheNum = 0; ///<del的read cache数量
        m_ullStatsImportWriteCacheNum = 0; ///<del的write cache数量
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
