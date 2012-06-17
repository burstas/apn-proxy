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
#endif
