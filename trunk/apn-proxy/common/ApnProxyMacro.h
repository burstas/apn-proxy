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

///key的定义
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



///错误代码的定义
#define APN_PROXY_ERR_SUCCESS           0   ///<成功
#define APN_PROXY_ERR_UNKNOWN_MSG       1   ///<未知的消息类型
#define APN_PROXY_ERR_INVALID_PACKAGE   2   ///<数据包无效
#define APN_PROXY_ERR_MISSING_APP       3   ///<缺少APP
#define APN_PROXY_ERR_MISSING_CHANNEL   4   ///<缺少channel参数
#define APN_PROXY_ERR_NO_APP            5   ///<指定的app不存在
#define APN_PROXY_ERR_NO_CHANNEL        6   ///<指定的channel不存在
#endif
