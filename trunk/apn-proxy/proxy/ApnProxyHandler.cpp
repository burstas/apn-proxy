#include "ApnProxyHandler.h"
#include "ApnProxyApp.h"
#include "ApnProxyAppPoco.h"

///echo请求的处理函数
int ApnProxyHandler::onRecvMsg(CwxMsgBlock*& msg, CwxTss* tss){
    int ret = APN_PROXY_ERR_SUCCESS;
    char const* szErrMsg = NULL;
    CwxKeyValueItem const* app = NULL;
    CwxKeyValueItem const* channel = NULL;
    CwxKeyValueItem const* dev = NULL;
    CwxKeyValueItem const* content = NULL;
    CWX_UINT32   uiId = 0;
    CWX_UINT32   uiExpire=0;
    CWX_UINT32   uiCheck=0;
    CWX_UINT8    ucState = 0;
    CWX_UINT32   i=0;
    char binDevId[APN_PROXY_APP_DEVICE_BINARY_SIZE];
    char szTmp[4];

    ApnProxyTss* pTss = (ApnProxyTss*)tss;
    do{
        if (!pTss->m_pReader->unpack(msg->rd_ptr(), msg->length(), false, true)){
            ret = APN_PROXY_ERR_INVALID_PACKAGE;
            szErrMsg = pTss->m_pReader->getErrMsg();
            break;
        }
        ///获取channel
        channel = pTss->m_pReader->getKey(APN_PROXY_KEY_CH, false);
        ///获取app
        app = pTss->m_pReader->getKey(APN_PROXY_KEY_APP, false);
        if (!app){
            ret = APN_PROXY_ERR_MISSING_APP;
            szErrMsg = "Missing [app] parameter";
            break;
        }
        ///检查app是否存在
        map<string, ApnProxySsl*>::iterator iter = pTss->m_appSsl.find(string(app->m_szData));
        if ( iter == pTss->m_appSsl.end()){
            ret = APN_PROXY_ERR_NO_APP;
            szErrMsg = "The app doesn't exist.";
            break;
        }
        ApnProxySsl* ssl = iter->second;
        ///获取dev
        dev = pTss->m_pReader->getKey(APN_PROXY_KEY_DEV, false);
        if (!dev){
            ret = APN_PROXY_ERR_MISSING_DEV;
            szErrMsg = "Missing [dev] parameter";
            break;
        }
        ///转化id
        memset(binDevId, 0x00, APN_PROXY_APP_DEVICE_BINARY_SIZE);
        while(i < dev->m_uiDataLen/2){
            szTmp[0]=dev->m_szData[i*2];
            szTmp[1]=dev->m_szData[i*2+1];
            szTmp[2]=0;
            binDevId[i] = (CWX_UINT8)strtoul(szTmp, NULL, 16);
            if (i >= APN_PROXY_APP_DEVICE_BINARY_SIZE) break;
        }
        if (i < APN_PROXY_APP_DEVICE_BINARY_SIZE){
            memmove(binDevId + (APN_PROXY_APP_DEVICE_BINARY_SIZE - i), binDevId, i);
            memset(binDevId, 0x00, APN_PROXY_APP_DEVICE_BINARY_SIZE - i);
        }
        ///获取内容
        content = pTss->m_pReader->getKey(APN_PROXY_KEY_DEV, false);
        if (!content){
            ret = APN_PROXY_ERR_MISSING_CONTENT;
            szErrMsg = "Missing [c] parameter";
            break;
        }
        if (content->m_uiDataLen > APN_PROXY_APP_MAXPAYLOAD_SIZE){
            ret = APN_PROXY_ERR_CONTENT_TOO_LEN;
            szErrMsg = "msg is too long";
            break;
        }
        ///获取id
        pTss->m_pReader->getKey(APN_PROXY_KEY_ID, uiId);
        ///获取expire
        pTss->m_pReader->getKey(APN_PROXY_KEY_E, uiExpire);
        if (uiExpire){
            if(uiExpire < time(NULL)) uiExpire += (CWX_UINT32)time(NULL);
        }else if (uiId){
            uiExpire = time(NULL);
            uiExpire += APN_PROXY_DEF_EXPIRE;
        }
        ///获取check
        pTss->m_pReader->getKey(APN_PROXY_KEY_CHECK, uiCheck);

        if (!ssl->isConnected()){
            if (0 != ssl->connect(m_pApp->getConfig().m_uiConnTimeoutMilliSecond, pTss->m_szBuf2K)){
                ret = APN_PROXY_ERR_FAIL_CONNECT;
                szErrMsg = pTss->m_szBuf2K;
                break;
            }
        }

        if (uiExpire){
            if (0 != ApnProxyAppPoco::sendNotice(ssl, binDevId, content->m_szData, content->m_uiDataLen, pTss->m_szBuf2K)){
                ssl->disconnect();
                ret = APN_PROXY_ERR_NOTICE_FAIL;
                szErrMsg = pTss->m_szBuf2K;
                break;
            }
        }else{
            if (0 != ApnProxyAppPoco::sendEnhancedNotice(ssl, uiExpire, uiId, binDevId, content->m_szData, content->m_uiDataLen, pTss->m_szBuf2K)){
                ssl->disconnect();
                ret = APN_PROXY_ERR_NOTICE_FAIL;
                szErrMsg = pTss->m_szBuf2K;
                break;
            }
        }
        if (uiCheck){
            if (!ssl->isReadReady(m_pApp->getConfig().m_uiCheckMilliSecond)) break;
            ret = APN_PROXY_ERR_NOTICE_FAIL;
            szErrMsg = "Failure to send msg.";
            if (uiExpire){
                ApnProxyAppPoco::readEnhancedNoticeErr(ssl,
                    ucState,
                    uiId,
                    m_pApp->getConfig().m_uiCheckMilliSecond,
                    pTss->m_szBuf2K);
            }
            break;
        }
        
    }while(0);

    CWX_INFO(("notice: channel=%s,app=%s,dev=%s,id=%u,expire=%u,ret=%d,c=%s",
        channel?channel->m_szData:"",
        app?app->m_szData:"",
        dev?dev->m_szData:"",
        uiId,
        uiExpire,
        ret,
        content?content->m_szData:""));

    ApnProxyApp::replyMsg(m_pApp,
        msg->event().getConnId(),
        msg->event().getMsgHeader().getMsgType() + 1,
        msg->event().getMsgHeader().getTaskId(),
        false,
        ret,
        szErrMsg,
        NULL,
        ucState);
    return 1;
}

