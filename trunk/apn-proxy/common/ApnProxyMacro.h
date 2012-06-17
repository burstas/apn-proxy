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

#define APN_PROXY_APP_DEVICE_BINARY_SIZE  32   ///<设备号的长度
#define APN_PROXY_APP_MAXPAYLOAD_SIZE     256  ///<消息体的长度


#define APN_PROXY_CHANNEL_PREFIX       "ch_" ///<配置文件中channel的前缀
#define APN_PROXY_APP_PREFIX           "app_" ///<配置文件中app的前缀
#define APN_PROXY_CHANNEL_TYPE_DEV     "dev"  ///<develope类型的channel
#define APN_PROXY_CHANNEL_TYPE_RELEASE "release" ///<release类型的channel
#endif
