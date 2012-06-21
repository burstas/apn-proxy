/*
版权声明：
本软件遵循GNU GPL V3（http://www.gnu.org/licenses/gpl.html），
联系方式：email:cwinux@gmail.com；微博:http://t.sina.com.cn/cwinux
*/

#include "CwxAppProcessMgr.h"
#include "ApnProxyApp.h"


int main(int argc, char** argv){
    //创建proxy的app对象实例
    ApnProxyApp* pApp = new ApnProxyApp();
    //初始化双进程管理器
    if (0 != CwxAppProcessMgr::init(pApp)) return 1;
    //启动双进程，一个为监控proxy进程的监控进程，一个为提供proxy服务的工作进程。
    CwxAppProcessMgr::start(argc, argv, 200, 300);
    return 0;
}
