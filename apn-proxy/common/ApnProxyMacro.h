#ifndef __APN_PROXY_MACRO_H__
#define __APN_PROXY_MACRO_H__


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


#define APN_PROXY_CHANNEL_PREFIX       "ch_" ///<�����ļ���channel��ǰ׺
#define APN_PROXY_APP_PREFIX           "app_" ///<�����ļ���app��ǰ׺
#define APN_PROXY_CHANNEL_TYPE_DEV     "dev"  ///<develope���͵�channel
#define APN_PROXY_CHANNEL_TYPE_RELEASE "release" ///<release���͵�channel

///key�Ķ���
#define APN_PROXY_KEY_RET              "ret"
#define APN_PROXY_KEY_ERR              "err"
#define APN_PROXY_KEY_STATUS           "status"
#define APN_PROXY_KEY_RESULT           "result"
#define APN_PROXY_KEY_APP              "app"
#define APN_PROXY_KEY_CH                "ch"
#define APN_PROXY_KEY_DEV               "dev"
#define APN_PROXY_KEY_C                 "c"
#define APN_PROXY_KEY_ID                "id"
#define APN_PROXY_KEY_E                 "e"



///�������Ķ���
#define APN_PROXY_ERR_SUCCESS           0   ///<�ɹ�
#define APN_PROXY_ERR_UNKNOWN_MSG       1   ///<δ֪����Ϣ����
#define APN_PROXY_ERR_INVALID_PACKAGE   2   ///<���ݰ���Ч
#define APN_PROXY_ERR_MISSING_APP       3   ///<ȱ��APP
#define APN_PROXY_ERR_MISSING_CHANNEL   4   ///<ȱ��channel����
#define APN_PROXY_ERR_NO_APP            5   ///<ָ����app������
#define APN_PROXY_ERR_NO_CHANNEL        6   ///<ָ����channel������
#endif
