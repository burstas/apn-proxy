#ifndef __APN_PROXY_MACRO_H__
#define __APN_PROXY_MACRO_H__

/*
版权声明：
本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
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

#define APN_PROXY_APP_DEVICE_BINARY_SIZE  32   ///<设备号的长度
#define APN_PROXY_APP_MAXPAYLOAD_SIZE     256  ///<消息体的长度

#define APN_PROXY_DEF_EXPIRE              24 * 3600 ///<缺省的超时时间

#define APN_PROXY_CHANNEL_PREFIX       "ch_" ///<配置文件中channel的前缀
#define APN_PROXY_APP_PREFIX           "app_" ///<配置文件中app的前缀
#define APN_PROXY_CHANNEL_TYPE_DEV     "dev"  ///<develope类型的channel
#define APN_PROXY_CHANNEL_TYPE_RELEASE "release" ///<release类型的channel

///key的定义
#define APN_PROXY_KEY_RET              "ret"  ///<返回值
#define APN_PROXY_KEY_ERR              "err"  ///<错误信息
#define APN_PROXY_KEY_STATUS           "status"  ///<apn返回的错误状态
#define APN_PROXY_KEY_RESULT           "result"  ///<结果
#define APN_PROXY_KEY_APP              "app"     ///<app的key
#define APN_PROXY_KEY_CH                "ch"     ///<channel的key
#define APN_PROXY_KEY_DEV               "dev"    ///<device 设备号
#define APN_PROXY_KEY_C                 "c"      ///<发送的消息内容
#define APN_PROXY_KEY_ID                "id"     ///<enchanced模式的消息id
#define APN_PROXY_KEY_E                 "e"      ///<enchanced模式的消息expire
#define APN_PROXY_KEY_CHECK             "check"  ///<是否检测当前消息是否发送成功
#define APN_PROXY_KEY_LE_DEV            "le_dev"  ///<（连接关闭）若上一次非check模式下的消息发送失败，则返回其对应的设备id。
#define APN_PROXY_KEY_LE_ID             "le_id"   ///<（连接关闭）若上一次非check模式下的消息发送失败，则返回其对应的enchanced模式的消息id
#define APN_PROXY_KEY_LE_C              "le_c"    ///<（连接关闭）若上一次非check模式下的消息发送失败，则返回其对应的发送内容


#define  APN_PROXY_CHANNEL_THREAD_STACK   100 * 1024   ///<定义channel线程的stack大小

///错误代码的定义
#define APN_PROXY_ERR_SUCCESS           0   ///<成功
#define APN_PROXY_ERR_UNKNOWN_MSG       1   ///<未知的消息类型
#define APN_PROXY_ERR_INVALID_PACKAGE   2   ///<数据包无效
#define APN_PROXY_ERR_MISSING_APP       3   ///<缺少APP
#define APN_PROXY_ERR_MISSING_CHANNEL   4   ///<缺少channel参数
#define APN_PROXY_ERR_NO_APP            5   ///<指定的app不存在
#define APN_PROXY_ERR_NO_CHANNEL        6   ///<指定的channel不存在
#define APN_PROXY_ERR_MISSING_DEV       7   ///<缺省dev参数
#define APN_PROXY_ERR_MISSING_CONTENT   8   ///<缺少content参数
#define APN_PROXY_ERR_FAIL_CONNECT      9   ///<连接失败
#define APN_PROXY_ERR_NOTICE_FAIL       10  ///<apn notice fail
#define APN_PROXY_ERR_CONTENT_TOO_LEN   11  ///<notice的消息太大

#endif
