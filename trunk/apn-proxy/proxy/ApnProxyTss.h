#ifndef __UNISTOR_TSS_H__
#define __UNISTOR_TSS_H__


#include "UnistorMacro.h"
#include "CwxLogger.h"
#include "CwxTss.h"
#include "CwxPackageReader.h"
#include "CwxPackageWriter.h"

class UnistorRecvHandler;
///存储key定义
struct UnistorStoreKey
{
public:
	unsigned char    m_type; ///<key的类型
	unsigned char    m_key[0]; ///<key的内容
}__attribute__ ((__packed__));

//kv的tss
class UnistorTss:public CwxTss
{
public:
    enum
    {
        DEF_PACKAGE_SIZE = UNISTOR_DEF_KV_SIZE ///<分发数据包的最大长度
    };
public:
    ///构造函数
    UnistorTss():CwxTss(){
        m_pReader = NULL;
		m_pItemReader = NULL;
        m_pWriter = NULL;
		m_pItemWriter = NULL;
        m_szDataBuf = NULL;
        m_uiDataBufLen = 0;
		m_uiConnId = 1;
		m_connMap = NULL;
    }
    ///析构函数
    ~UnistorTss();
public:
    ///tss的初始化，0：成功；-1：失败
    int init();
    ///获取package的buf，返回NULL表示失败
    inline char* getBuf(CWX_UINT32 uiSize){
        if (m_uiDataBufLen < uiSize){
            delete [] m_szDataBuf;
            m_szDataBuf = new char[uiSize];
            m_uiDataBufLen = uiSize;
        }
        return m_szDataBuf;
    }
	CWX_UINT32 getNextConnId()
	{
		m_uiConnId++;
		if (0 == m_uiConnId) m_uiConnId = 1;
		while (m_connMap->find(m_uiConnId) != m_connMap->end())
		{
			m_uiConnId++;
			if (0 == m_uiConnId) m_uiConnId = 1;
		}
		return m_uiConnId;
	}
	UnistorRecvHandler* getConn(CWX_UINT32 uiConnId)
	{
		hash_map<CWX_UINT32/*conn id*/, UnistorRecvHandler*/*连接对象*/>::iterator iter = m_connMap->find(uiConnId);
		if (iter != m_connMap->end()) return iter->second;
		return NULL;
	}
	void removeConn(CWX_UINT32 uiConnId)
	{
		hash_map<CWX_UINT32/*conn id*/, UnistorRecvHandler*/*连接对象*/>::iterator iter = m_connMap->find(uiConnId);
		if (iter != m_connMap->end()) m_connMap->erase(iter);
	}
	void addConn(CWX_UINT32 uiConnId, UnistorRecvHandler* conn)
	{
		(*m_connMap)[uiConnId] = conn;
	}

public:
    CwxPackageReader*      m_pReader; ///<数据包的解包对象
	CwxPackageReader*      m_pItemReader; ///<数据包的解包对象
    CwxPackageWriter*      m_pWriter; ///<数据包的pack对象
	CwxPackageWriter*      m_pItemWriter; ///<chunk时的一个消息的数据包的pack对象
	char			       m_szStoreKey[sizeof(UnistorStoreKey) + UNISTOR_MAX_KEY_SIZE + 1024]; ///<存储的key
private:
    char*                  m_szDataBuf; ///<数据buf
    CWX_UINT32             m_uiDataBufLen; ///<数据buf的空间大小
	CWX_UINT32			   m_uiConnId; ///<连接id
	hash_map<CWX_UINT32/*conn id*/, UnistorRecvHandler*/*连接对象*/>* m_connMap;
};





#endif
