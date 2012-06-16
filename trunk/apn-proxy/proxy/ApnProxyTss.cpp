#include "UnistorTss.h"

///构造函数
UnistorTss::~UnistorTss(){
    if (m_pReader) delete m_pReader;
	if (m_pItemReader) delete m_pItemReader;
    if (m_pWriter) delete m_pWriter;
	if (m_pItemWriter) delete m_pItemWriter;
	if (m_szDataBuf) delete [] m_szDataBuf;
	if (m_connMap) delete m_connMap;
}

int UnistorTss::init(){
    m_pReader = new CwxPackageReader(false);
	m_pItemReader = new CwxPackageReader(false);
    m_pWriter = new CwxPackageWriter(DEF_PACKAGE_SIZE);
	m_pItemWriter = new CwxPackageWriter(DEF_PACKAGE_SIZE);
    m_szDataBuf = new char[DEF_PACKAGE_SIZE];
    m_uiDataBufLen= DEF_PACKAGE_SIZE;
	m_uiConnId = 1;
	m_connMap = new hash_map<CWX_UINT32/*conn id*/, UnistorRecvHandler*/*连接对象*/>(10000);
    return 0;
}
