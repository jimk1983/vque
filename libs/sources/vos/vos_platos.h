/******************************************************************************

                  版权所有 (C), 2018-2028, 技术有限公司

 ******************************************************************************
  文 件 名   : vos_platos.h
  版 本 号   : 初稿
  作    者  : 蒋康
  生成日期   : 
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   :
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#ifndef _VOS_PLATOS_H_
#define _VOS_PLATOS_H_

/**
 * @file    vos_platos.h
 * @brief   define the platform macro
 */

#ifdef __cplusplus
#define VOS_EXCBEGIN extern "C" {
#define VOS_EXCEND }
#else
#define VOS_EXCBEGIN
#define VOS_EXCEND
#endif /* __cplusplus */

#define CINLINE static inline

/**************************************************/
/***************** 系统配置 ***********************/
/**************************************************/
/*定义的平台是MAC系统*/
#define     VOS_PLAT_MAC                0
/*定义的平台是Win系统*/
#define     VOS_PLAT_WIN                0
/*定义的平台是Linux系统*/
#define     VOS_PLAT_LINUX              1
/*定义的是32位系统*/
#define     VOS_PLATCPU_32              1
/*定义的是64位系统*/
#define     VOS_PLATCPU_64              0
/*定义的明确大小端:默认小端*/
#define     VOS_PLAT_BIGENDIAN          0
/*定义是否需要C++11支持*/
#define     VOS_PLAT_SUPPORT_Class11    0
/*定义是否开启DBG*/
#define     VOS_PLAT_DBG                1



/**************************************************/
/***************** 系统定义 ***********************/
/**************************************************/
#ifndef VOID
typedef void                VOID, *PVOID;
#endif

#ifndef CHAR
typedef signed char         CHAR, *PCHAR;
#endif

#ifndef UCHAR
typedef unsigned char       UCHAR, *PUCHAR;
#endif
    
#ifndef BOOL_T
typedef unsigned char       BOOL_T, *PBOOL_T;
#endif

#ifndef INT8_T
typedef signed char         INT8_T, *PINT8_T;
#endif

#ifndef UINT8_T
typedef unsigned char       UINT8_T, *PUINT8_T;
#endif
#ifndef INT16_T
typedef signed short        INT16_T, *PINT16_T;
#endif
#ifndef UINT16_T
typedef unsigned short      UINT16_T, *PUINT16_T;
#endif
#ifndef INT32_T
typedef signed int          INT32_T, *PINT32_T;
#endif
#ifndef UINT32_T
typedef unsigned int        UINT32_T, *PUINT32_T;
#endif


#ifndef VINT16_T
typedef volatile signed short        VINT16_T, *PVINT16_T;
#endif
#ifndef VUINT16_T
typedef volatile unsigned short      VUINT16_T, *PVUINT16_T;
#endif
#ifndef VINT32_T
typedef volatile signed int          VINT32_T, *PVINT32_T;
#endif
#ifndef VUINT32_T
typedef volatile unsigned int        VUINT32_T, *PVUINT32_T;
#endif


typedef struct tagDoublelong
{
    struct tagulong_ul
    {
         UINT32_T ulHi_l;
         UINT32_T ulLo_l;
    }x;
    
    struct taglong_us
    {
        #if VOS_PLAT_BIGENDIAN
            UINT16_T usHiHi_s;
            UINT16_T usHiLo_s;
            UINT16_T usLoHi_s;
            UINT16_T usLoLo_s;
        #else
            UINT16_T usHiLo_s;
            UINT16_T usHiHi_s;
            UINT16_T usLoLo_s;
            UINT16_T usLoHi_s;
        #endif
    }y;
    #define     ulHi    x.ulHi_l
    #define     ulLo    x.ulLo_l
}DULONG, *PDULONG;

#if VOS_PLATCPU_32

#ifndef INT64_T
typedef signed long    INT64_T, *PINT64_T;
#endif
#ifndef UINT64_T
typedef unsigned long  UINT64_T, *PUINT64_T;
#endif

#ifndef VINT64_T
typedef volatile signed long    VINT64_T, *PVINT64_T;
#endif
#ifndef VUINT64_T
typedef volatile unsigned long  UVINT64_T, *PVUINT64_T;
#endif

#define VOS_INVALID_VAL   0xFFFFFFFF

#elif VOS_PLATCPU_64
#ifndef INT64_T
typedef signed long long    INT64_T, *PINT64_T;
#endif

#ifndef UINT64_T
typedef unsigned long long  UINT64_T, *PUINT64_T;
#endif

#ifndef VINT64_T
typedef volatile signed long long    VINT64_T, *PVINT64_T;
#endif

#ifndef UINT64_T
typedef volatile unsigned long long  VUINT64_T, *PVUINT64_T;
#endif

#define VOS_INVALID_VAL   0xFFFFFFFFFFFFFFFF

#endif

/*将DULONG 转为ULONG64*/
#define VOS_DULONG_TO_ULONG64(dulSrc, ullDst64) \
        (ullDst64) = ((UINT64_T)((dulSrc).ulHi) << 32 \
        | ((UINT64_T)((dulSrc).ulLo)) );

/*将ULONG转化为DULONG*/
#define VOS_ULONG64_TO_DULONG(ullSrc64, dulDst) \
    (dulDst).ulHi  =  (UINT32_T)((ullSrc64) >> 32;\
    (dulDst).ulLo =  (UINT32_T)((ullSrc64);
    
#define IN 
#define OUT
#define INOUT

#define VOS_COREDUMP        {char *ptr=0x00;*(ptr)=0x00;}

#if VOS_PLAT_DBG
#define VOS_ASSERT(expr)     if(!(expr))VOS_COREDUMP
#else
#define VOS_ASSERT(expr)
#endif
    
#define VOS_SSL_ENABLE  0
#if VOS_SSL_ENABLE
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>
#include <openssl/pkcs12.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#endif

/**************************************************/
/***************** 系统头文件 *********************/
/**************************************************/
/* Linux系统 */
#if VOS_PLAT_LINUX
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/param.h>
#include <sys/syscall.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>
#include <sys/timerfd.h>
#include <netdb.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>
#include <linux/rtnetlink.h>
#include <dirent.h>

/*BPF规格*/
#include <linux/if_ether.h>
#include <net/if.h>
#include <linux/filter.h>

/*支持pcap扩展*/
//#include <pcap/pcap.h>   //suse有该头文件
//#include <pcap.h>

#if defined(__SUNPRO_C)
#define VAPI __global
#elif ((defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__)
#define VAPI __attribute__ ((visibility("default")))
#else
#define VAPI
#endif /*__SUNPRO_C */

#elif  VOS_PLAT_WIN
/*该处定义的是纯C的头文件，C++的请另外定义头文件*/
#include <Winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <TimeAPI.h>
#include <mswsock.h>
#include <stdio.h>
#include <process.h>
#include <winsvc.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ShlObj.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h> 
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

/*
 * For windows library
 * VEXPORT = 0 use code directory
 * VEXPORT = 1 export vos API
 * VEXPORT = 2 import vos API
 */
#ifndef VEXPORT
#define VEXPORT 2
#endif /* VEXPORT */

#if (VEXPORT == 0)
#define VAPI
#elif (VEXPORT == 1)
#define VAPI __declspec(dllexport)
#define VEXTERN __declspec(dllexport)
#elif (VEXPORT == 2)
#define VAPI __declspec(dllimport)
#define VEXTERN __declspec(dllimport)
#endif

#elif  VOS_PLAT_MAC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h> 
#include <math.h>
#endif


#if VOS_PLAT_LINUX
#ifndef NULL
#define NULL ((void*)0)
#endif
#elif VOS_PLAT_WIN
#endif


#define PERR_FORMAT " [FILE=%s, FUNC=%s,LINE=%d]"

#define PError(ErrMsg, ...)\
{\
  (void)printf(("%s [ERR] " ErrMsg PERR_FORMAT "\n"),VOS_GetSysTimeNowStr(), ##__VA_ARGS__, __FILE__,__FUNCTION__,__LINE__);\
}


#if VOS_PLAT_DBG
#define PEVENT_FORMAT " [FILE=%s,FUNC=%s,LINE=%d]"
#define PEvent(EvtMsg, ...)\
{\
  (void)printf(("%s [EVT] " EvtMsg PEVENT_FORMAT "\n"),VOS_GetSysTimeNowStr(),##__VA_ARGS__,__FILE__,__FUNCTION__,__LINE__);\
}
#else
#define PEvent(EvtMsg, ...)
#endif

#endif

