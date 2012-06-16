#ifndef __UNISTOR_TSS_H__
#define __UNISTOR_TSS_H__


#include "UnistorMacro.h"
#include "CwxLogger.h"
#include "CwxTss.h"
#include "CwxPackageReader.h"
#include "CwxPackageWriter.h"

class UnistorRecvHandler;
///�洢key����
struct UnistorStoreKey
{
public:
	unsigned char    m_type; ///<key������
	unsigned char    m_key[0]; ///<key������
}__attribute__ ((__packed__));

//kv��tss
class UnistorTss:public CwxTss
{
public:
    enum
    {
        DEF_PACKAGE_SIZE = UNISTOR_DEF_KV_SIZE ///<�ַ����ݰ�����󳤶�
    };
public:
    ///���캯��
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
    ///��������
    ~UnistorTss();
public:
    ///tss�ĳ�ʼ����0���ɹ���-1��ʧ��
    int init();
    ///��ȡpackage��buf������NULL��ʾʧ��
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
		hash_map<CWX_UINT32/*conn id*/, UnistorRecvHandler*/*���Ӷ���*/>::iterator iter = m_connMap->find(uiConnId);
		if (iter != m_connMap->end()) return iter->second;
		return NULL;
	}
	void removeConn(CWX_UINT32 uiConnId)
	{
		hash_map<CWX_UINT32/*conn id*/, UnistorRecvHandler*/*���Ӷ���*/>::iterator iter = m_connMap->find(uiConnId);
		if (iter != m_connMap->end()) m_connMap->erase(iter);
	}
	void addConn(CWX_UINT32 uiConnId, UnistorRecvHandler* conn)
	{
		(*m_connMap)[uiConnId] = conn;
	}

public:
    CwxPackageReader*      m_pReader; ///<���ݰ��Ľ������
	CwxPackageReader*      m_pItemReader; ///<���ݰ��Ľ������
    CwxPackageWriter*      m_pWriter; ///<���ݰ���pack����
	CwxPackageWriter*      m_pItemWriter; ///<chunkʱ��һ����Ϣ�����ݰ���pack����
	char			       m_szStoreKey[sizeof(UnistorStoreKey) + UNISTOR_MAX_KEY_SIZE + 1024]; ///<�洢��key
private:
    char*                  m_szDataBuf; ///<����buf
    CWX_UINT32             m_uiDataBufLen; ///<����buf�Ŀռ��С
	CWX_UINT32			   m_uiConnId; ///<����id
	hash_map<CWX_UINT32/*conn id*/, UnistorRecvHandler*/*���Ӷ���*/>* m_connMap;
};





#endif
