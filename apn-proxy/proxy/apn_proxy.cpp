#include "CwxAppProcessMgr.h"
#include "ApnProxyApp.h"

int main(int argc, char** argv){
    //����proxy��app����ʵ��
    ApnProxyApp* pApp = new ApnProxyApp();
    //��ʼ��˫���̹�����
    if (0 != CwxAppProcessMgr::init(pApp)) return 1;
    //����˫���̣�һ��Ϊ���proxy���̵ļ�ؽ��̣�һ��Ϊ�ṩproxy����Ĺ������̡�
    CwxAppProcessMgr::start(argc, argv, 200, 300);
    return 0;
}
