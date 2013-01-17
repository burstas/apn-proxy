##  Process this file with automake to create Makefile.in

includedir =
pkgconfigdir =

BUILD_DATE=$(shell date +%F)


bin_PROGRAMS = apn_proxy
apn_proxy_CPPFLAGS = -D_BUILD_DATE="$(BUILD_DATE)" -W -Wall
apn_proxy_SOURCES =ApnProxyApp.cpp\
		ApnProxyConfig.cpp\
		apn_proxy.cpp\
		ApnProxyHandler.cpp\
		ApnProxyTss.cpp\
		../common/ApnProxyAppPoco.cpp\
		../common/ApnProxySsl.cpp
apn_proxy_LDADD=
apn_proxy_LDFLAGS = -L@cwinux_dir@/lib -L@openssl_dir@/lib
LIBS=-lcwxapp -lcwxnet -lcwxcmn  -lssl -lcrypto -lpthread -ldl -lrt -lz
INCLUDES=-I@cwinux_dir@/include/app -I@cwinux_dir@/include/cmn -I@cwinux_dir@/include/net  -I@openssl_dir@/include  -I. -I../common

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
