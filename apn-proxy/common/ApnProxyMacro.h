#ifndef __APN_PROXY_MACRO_H__
#define __APN_PROXY_MACRO_H__

/*
��Ȩ������
�������ѭGNU GPL V3��http://www.gnu.org/licenses/gpl.html����
��ϵ��ʽ��email:cwinux@gmail.com��΢��:http://t.sina.com.cn/cwinux
*/

#include "CwxGlobalMacro.h"
#include "CwxType.h"
#include "CwxStl.h"
#include "CwxStlFunc.h"

CWINUX_USING_NAMESPACE

// Development Connection info
#define APN_PROXY_DEV_HOST          "gateway.sandbox.push.apple.com"
#define APN_PROXY_DEV_PORT          2195
#define APN_PROXY_DEV_FEEDBACK_HOST "feedback.sandbox.push.apple.com"
#define APN_PROXY_DEV_FEEDBACK_PORT 2196
//Release Connection info
#define APN_PROXY_RELEASE_HOST          "gateway.push.apple.com"
#define APN_PROXY_RELEASE_PORT          2195
#define APN_PROXY_RELEASE_FEEDBACK_HOST "feedback.push.apple.com"
#define APN_PROXY_RELEASE_FEEDBACK_PORT 2196

#define APN_PROXY_APP_DEVICE_BINARY_SIZE  32   ///<�豸�ŵĳ���
#define APN_PROXY_APP_MAXPAYLOAD_SIZE     256  ///<��Ϣ��ĳ���

#define APN_PROXY_DEF_EXPIRE              24 * 3600 ///<ȱʡ�ĳ�ʱʱ��

#define APN_PROXY_CHANNEL_PREFIX       "ch_" ///<�����ļ���channel��ǰ׺
#define APN_PROXY_APP_PREFIX           "app_" ///<�����ļ���app��ǰ׺
#define APN_PROXY_CHANNEL_TYPE_DEV     "dev"  ///<develope���͵�channel
#define APN_PROXY_CHANNEL_TYPE_RELEASE "release" ///<release���͵�channel

///key�Ķ���
#define APN_PROXY_KEY_RET              "ret"  ///<����ֵ
#define APN_PROXY_KEY_ERR              "err"  ///<������Ϣ
#define APN_PROXY_KEY_STATUS           "status"  ///<apn���صĴ���״̬
#define APN_PROXY_KEY_RESULT           "result"  ///<���
#define APN_PROXY_KEY_APP              "app"     ///<app��key
#define APN_PROXY_KEY_CH                "ch"     ///<channel��key
#define APN_PROXY_KEY_DEV               "dev"    ///<device �豸��
#define APN_PROXY_KEY_C                 "c"      ///<���͵���Ϣ����
#define APN_PROXY_KEY_ID                "id"     ///<enchancedģʽ����Ϣid
#define APN_PROXY_KEY_E                 "e"      ///<enchancedģʽ����Ϣexpire
#define APN_PROXY_KEY_CHECK             "check"  ///<�Ƿ��⵱ǰ��Ϣ�Ƿ��ͳɹ�
#define APN_PROXY_KEY_LE_DEV            "le_dev"  ///<�����ӹرգ�����һ�η�checkģʽ�µ���Ϣ����ʧ�ܣ��򷵻����Ӧ���豸id��
#define APN_PROXY_KEY_LE_ID             "le_id"   ///<�����ӹرգ�����һ�η�checkģʽ�µ���Ϣ����ʧ�ܣ��򷵻����Ӧ��enchancedģʽ����Ϣid
#define APN_PROXY_KEY_LE_C              "le_c"    ///<�����ӹرգ�����һ�η�checkģʽ�µ���Ϣ����ʧ�ܣ��򷵻����Ӧ�ķ�������


#define  APN_PROXY_CHANNEL_THREAD_STACK   100 * 1024   ///<����channel�̵߳�stack��С

///�������Ķ���
#define APN_PROXY_ERR_SUCCESS           0   ///<�ɹ�
#define APN_PROXY_ERR_UNKNOWN_MSG       1   ///<δ֪����Ϣ����
#define APN_PROXY_ERR_INVALID_PACKAGE   2   ///<���ݰ���Ч
#define APN_PROXY_ERR_MISSING_APP       3   ///<ȱ��APP
#define APN_PROXY_ERR_MISSING_CHANNEL   4   ///<ȱ��channel����
#define APN_PROXY_ERR_NO_APP            5   ///<ָ����app������
#define APN_PROXY_ERR_NO_CHANNEL        6   ///<ָ����channel������
#define APN_PROXY_ERR_MISSING_DEV       7   ///<ȱʡdev����
#define APN_PROXY_ERR_MISSING_CONTENT   8   ///<ȱ��content����
#define APN_PROXY_ERR_FAIL_CONNECT      9   ///<����ʧ��
#define APN_PROXY_ERR_NOTICE_FAIL       10  ///<apn notice fail
#define APN_PROXY_ERR_CONTENT_TOO_LEN   11  ///<notice����Ϣ̫��

#endif
