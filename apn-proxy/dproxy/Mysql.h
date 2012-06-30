#ifndef __MYSQL_H__
#define __MYSQL_H__

/**
@file Mysql.h
@brief Mysql���ݿ�Ľӿ���Mysql��
@author cwinux@gmail.com
@version 0.1
@date 2011-11-10
@warning
@bug
*/
#include "CwxGlobalMacro.h"
#include "CwxHostInfo.h"
#include "CwxCommon.h"
#include "CwxStl.h"
#include "CwxStlFunc.h"
#include "JPoolMacro.h"
#include "CwxLogger.h"
#include "CwxTss.h"


#include <mysql.h>

/**
@class Mysql
@brief Mysql���ݿ�����Ӷ���
*/
class CWX_API Mysql{
public:
    ///���캯��
    Mysql(){
        m_bInit = false;
        m_bConnected = false;
        m_iAffectedRow = 0;
        m_result = NULL;
    }
    ///��������
    ~Mysql(){
        disconnect();
    }
public:
    ///��ʼ�����ݿ����Ӷ���
    bool init();
    /**
    @brief ����MYSQL���ݿ⡣
    @param [in] szSvrName ���ݿ�ķ�����
    @param [in] szUser ���ݿ����ӵ��û���
    @param [in] szPasswd �û�����
    @param [in] szDbName �������ݿ�����
    @param [in] unPort ���ӵĶ˿ںţ�Ĭ��Ϊ3306
    @param [in] szUnixsock ���ǲ���UNIX DOMAIN����ָ����sock�ļ�
    @param [in] uiClientFlag ���ӵ����ԣ�ȱʡΪCLIENT_MULTI_STATEMENTS|CLIENT_FOUND_ROWS
    @param [in] uiTimeoutSecond ���ӵĳ�ʱʱ�䣬Ĭ��Ϊ��������
    @return false������ʧ�ܣ�true�����ӳɹ�����ʧ�ܵ�����£�getErrMsg()��ȡ������Ϣ��
    */
    bool connect(char const* szSvrName,
                 char const* szUser,
                 char const* szPasswd,
                 char const* szDbName,
                 unsigned short unPort = 3306,
                 char const* szUnixsock = NULL,
                 unsigned long uiClientFlag = CLIENT_MULTI_STATEMENTS|CLIENT_FOUND_ROWS,
                 unsigned long uiTimeoutSecond=0);
    ///�ر�����
    void disconnect();
    ///�������ӵ��ַ���������Ҫ���ã�������init()���趨��
    bool setCharacterSet(char const* szCharsetName);
    ///�������ӵ����ԣ�����Ҫ���ã�������init()���趨��
    void setOption(mysql_option option, const char *arg);
    ///�����Զ��ύģʽ
    bool setAutoCommit(bool bAutoCommit);
    ///�ύ���ݵ��޸ģ�ֻ�Ƿ��Զ��ύ��ģʽ����С
    bool commit();
    ///�ع������ݿ���޸�
    bool rollback();
    ///ִ�з�query�����ݿ����������ֵ��-1��ʧ�ܣ�>=0������Ӱ������ݼ�¼����
    int  execute(char const* szSql);
    ///��ȡsql��count����,-1��ʾʧ��
    bool count(char const* szSql, CWX_UINT32& num);
    ///ִ�����ݲ�ѯ������false����ѯʧ�ܣ�true����ѯ�ɹ�
    bool query(char const* szSql);
    ///ִ��binary sql�Ĳ�ѯ��false����ѯʧ�ܣ�true����ѯ�ɹ�
    bool query(char const* szSql, unsigned long uiSqlLen);
    ///��ȡquery���������һ����¼������ֵ��-1:failure; 0: finish; 1: get one row
    int	 next();
    ///��ȡ��ǰ��¼�ĵ�uiFieldIndex���ֶ�,��ͨ��bNull�������Ƿ�ΪNULL��
    char const*  fetch(unsigned long uiFieldIndex, bool& bNull);
    ///��ȡ��ǰ��¼�ĵ�uiFieldIndex���ֶε����֡�
    char const* getFieldName(unsigned long uiFieldIndex);
    ///��ȡ��ǰ��¼�ĵ�uiFieldIndex���ֶε���Ϣ��
    MYSQL_FIELD* getFieldInfo(unsigned long uiFieldIndex);
    ///��ȡ��ǰ��������ֶ�����
    int	 getFieldNum();
    ///��ȡǰһ�����ݿ����Ӱ������ݿ��¼����
    int  getAffectedRow() const { return m_iAffectedRow;}
    ///�ͷŵ�ǰ�Ĳ�ѯ�����
    int	 freeResult();
    ///������ݿ������Ƿ���Ч
    bool ping();
    ///�����ݿ����ʧ�ܵ�����£���ȡʧ�ܵĴ�����Ϣ
    char const* getErrMsg() const { return m_strErrMsg.c_str();}
    ///����Ƿ��������ݿ�����
    bool IsConnected() const {return m_bConnected;}
    ///��ȡMYSQL�����Ӿ��
    MYSQL& getHandle() { return m_handle;}
private:
    MYSQL	    m_handle;///<mysql�����Ӿ��
    MYSQL_RES*  m_result;///<��ѯ�������صĽ����
    MYSQL_ROW   m_row;///<������ĵ�ǰ��
    bool        m_bInit;///<�����Ƿ��ʼ��
    bool	    m_bConnected;///<���ݿ�������Ƿ���
    long       m_iAffectedRow;///<���ݿ����Ӱ������ݿ�ļ�¼����
    string	    m_strErrMsg;///<����ʧ��ʱ�Ĵ�����Ϣ
};


#endif
