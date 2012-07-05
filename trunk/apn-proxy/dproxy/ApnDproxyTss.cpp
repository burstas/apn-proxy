#include "ApnDproxyTss.h"

///¹¹Ôìº¯Êý
ApnDproxyTss::~ApnDproxyTss(){
    if (m_pReader) delete m_pReader;
    if (m_pWriter) delete m_pWriter;
    map<string, ApnDproxySslInfo*>::iterator iter = m_appSsl.begin();
    while(iter != m_appSsl.end()){
        delete iter->second->m_ssl;
        delete iter->second;
        iter++;
    }
}

int ApnDproxyTss::init(){
    m_pReader = new CwxPackageReader(false);
    m_pWriter = new CwxPackageWriter(DEF_PACKAGE_SIZE);
    return 0;
}
