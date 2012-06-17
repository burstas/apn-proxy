#ifndef __APN_PROXY_TSS_H__
#define __APN_PROXY_TSS_H__


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
    ///���캯��
    ApnProxyTss():CwxTss(){
    }
    ///��������
    ~ApnProxyTss();
public:
    ///tss�ĳ�ʼ����0���ɹ���-1��ʧ��
    int init(list<);
    ///��ȡ�û�������
    UnistorTssUserObj* getUserObj() { return m_userObj;}
    ///��ȡһ��write arg����
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
    ///�ͷ�һ��write arg����
    inline void pushWriteMsgArg(UnistorWriteMsgArg* arg){
        arg->m_next = m_writeMsgHead;
        m_writeMsgHead = arg;

    }
    ///��ȡpackage��buf������NULL��ʾʧ��
    inline char* getBuf(CWX_UINT32 uiSize){
        if (m_uiDataBufLen < uiSize){
            delete [] m_szDataBuf;
            m_szDataBuf = new char[uiSize];
            m_uiDataBufLen = uiSize;
        }
        return m_szDataBuf;
    }
    ///��ȡpackage��buf������NULL��ʾʧ��
    inline char* getBuf1(CWX_UINT32 uiSize){
        if (m_uiDataBufLen1 < uiSize){
            delete [] m_szDataBuf1;
            m_szDataBuf1 = new char[uiSize];
            m_uiDataBufLen1 = uiSize;
        }
        return m_szDataBuf1;
    }
    ///��ȡpackage��buf������NULL��ʾʧ��
    inline char* getBuf2(CWX_UINT32 uiSize){
        if (m_uiDataBufLen2 < uiSize){
            delete [] m_szDataBuf2;
            m_szDataBuf2 = new char[uiSize];
            m_uiDataBufLen2 = uiSize;
        }
        return m_szDataBuf2;
    }
    ///��ȡpackage��buf������NULL��ʾʧ��
    inline char* getBuf3(CWX_UINT32 uiSize){
        if (m_uiDataBufLen3 < uiSize){
            delete [] m_szDataBuf3;
            m_szDataBuf3 = new char[uiSize];
            m_uiDataBufLen3 = uiSize;
        }
        return m_szDataBuf3;
    }
    ///�Ƿ���master idc
    bool isMasterIdc(){
        if (m_pZkConf && m_pZkConf->m_bMasterIdc) return true;
        return false;
    }
    ///��ȡmaster idc������
    char const* getMasterIdc() const{
        if (m_pZkConf && m_pZkConf->m_strMasterIdc.length()) return m_pZkConf->m_strMasterIdc.c_str();
        return "";
    }
    ///�Լ��Ƿ���master
    bool isMaster(){
        if (m_pZkLock && m_pZkLock->m_bMaster) return true;
        return false;
    }
    ///�Ƿ����master
    bool isExistMaster(){
        if (m_pZkLock && m_pZkLock->m_strMaster.length()) return true;
        return false;
    }
    ///��ȡmaster��������
    char const* getMasterHost() const{
        if (m_pZkLock && m_pZkLock->m_strMaster.length()) return m_pZkLock->m_strMaster.c_str();
        return "";
    }
    ///��ȡidc�ڵ���һ��sync��������
    char const* getSyncHost() const{
        if (m_pZkLock){
            if (m_pZkLock->m_strPrev.length()) return m_pZkLock->m_strPrev.c_str();
            if (m_pZkLock->m_strMaster.length()) return m_pZkLock->m_strMaster.c_str();
        }
        return "";
    }
    inline void resetStats(){
        ///ͳ�Ƴ�ʼ��
        m_ullStatsGetNum = 0; ///<get��ѯ������
        m_ullStatsGetReadCacheNum = 0; ///<get��ѯʹ��Cache����
        m_ullStatsGetExistNum = 0; ///<get��ѯ���ڽ��������
        m_ullStatsGetsNum = 0; ///<gets��ѯ������
        m_ullStatsGetsKeyNum = 0; ///<gets��key������
        m_ullStatsGetsKeyReadCacheNum = 0; ///<gets��key��cache����
        m_ullStatsGetsKeyExistNum = 0; ///<gets��key�Ĵ��ڵ�����
        m_ullStatsListNum = 0; ///<list������
        m_ullStatsExistNum = 0; ///<exist������
        m_ullStatsExistReadCacheNum = 0; ///<exist��cache����
        m_ullStatsExistExistNum = 0; ///<exist�Ĵ��ڵ�����
        m_ullStatsAddNum = 0; ///<add������
        m_ullStatsAddReadCacheNum = 0; ///<add��read cache����
        m_ullStatsAddWriteCacheNum = 0; ///<add��write cache����
        m_ullStatsSetNum = 0; ///<set������
        m_ullStatsSetReadCacheNum = 0; ///<set��read cache����
        m_ullStatsSetWriteCacheNum = 0; ///<set��write cache����
        m_ullStatsUpdateNum = 0; ///<update������
        m_ullStatsUpdateReadCacheNum = 0; ///<update��read cache����
        m_ullStatsUpdateWriteCacheNum = 0; ///<update��write cache����
        m_ullStatsIncNum = 0; ///<inc������
        m_ullStatsIncReadCacheNum = 0; ///<inc��read cache����
        m_ullStatsIncWriteCacheNum = 0; ///<inc��write cache����
        m_ullStatsDelNum = 0; ///<del������
        m_ullStatsDelReadCacheNum = 0; ///<del��read cache����
        m_ullStatsDelWriteCacheNum = 0; ///<del��write cache����
        m_ullStatsImportNum = 0; ///<del������
        m_ullStatsImportReadCacheNum = 0; ///<del��read cache����
        m_ullStatsImportWriteCacheNum = 0; ///<del��write cache����
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
