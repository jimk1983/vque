/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_platos.h
  �� �� ��   : ����
  ��    ��  : ����
  ��������   : 
  ����޸�   :
  ��������   : 
  �����б�   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_PLATOS_H_
#define _VOS_PLATOS_H_

/**
 * @file    vos_platos.h
 * @brief   define the platform macro
 */

#ifdef __cplusplus
#define VOS_EXBEGIN extern "C" {
#define VOS_EXEND }
#else
#define VOS_EXBEGIN
#define VOS_EXEND
#endif /* __cplusplus */

/**************************************************/
/***************** ϵͳ���� ***********************/
/**************************************************/
/*�����ƽ̨��MACϵͳ*/
#define     VOS_PLAT_MAC                0
/*�����ƽ̨��Winϵͳ*/
#define     VOS_PLAT_WIN                0
/*�����ƽ̨��Linuxϵͳ*/
#define     VOS_PLAT_LINUX              1
/*�������32λϵͳ*/
#define     VOS_PLATCPU_32              1
/*�������64λϵͳ*/
#define     VOS_PLATCPU_64              0
/*�������ȷ��С��:Ĭ��С��*/
#define     VOS_PLAT_BIGENDIAN          0
/*�����Ƿ���ҪC++11֧��*/
#define     VOS_PLAT_SUPPORT_Class11    0


/**************************************************/
/***************** ϵͳ���� ***********************/
/**************************************************/
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

#define VINLINE static _inline

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

#define VOS_INVALID_VAL32   0xFFFFFFFF

#elif VOS_PLATCPU_64
#ifndef INT64_T
typedef signed long long    INT64_T, *PINT64_T;
#endif
#ifndef UINT64_T
typedef unsigned long long  UINT64_T, *PUINT64_T;
#endif

#define VOS_INVALID_VAL64   0xFFFFFFFFFFFFFFFF

#endif

/*��DULONG תΪULONG64*/
#define VOS_DULONG_TO_ULONG64(dulSrc, ullDst64) \
        (ullDst64) = ((UINT64_T)((dulSrc).ulHi) << 32 \
        | ((UINT64_T)((dulSrc).ulLo)) );

/*��ULONGת��ΪDULONG*/
#define VOS_ULONG64_TO_DULONG(ullSrc64, dulDst) \
    (dulDst).ulHi  =  (UINT32_T)((ullSrc64) >> 32;\
    (dulDst).ulLo =  (UINT32_T)((ullSrc64);
    
#define IN 
#define OUT
#define INOUT

#define VOS_COREDUMP        {char *ptr=0x00;*(ptr)=0x00;}
#define VOS_ASSERT(expr)     if(!(expr))VOS_COREDUMP
    
    
#ifndef VOS_OK
#define VOS_OK               0
#endif
        
#ifndef VOS_ERR
#define VOS_ERR             (~(0))
#endif

#ifndef SYS_OK
#define SYS_OK              VOS_OK
#endif

#ifndef SYS_ERR
#define SYS_ERR             VOS_ERR
#endif

#ifndef VOS_TRUE
#define VOS_TRUE            1
#endif
    
#ifndef VOS_FALSE
#define VOS_FALSE           0
#endif
    

/**************************************************/
/***************** ϵͳ������       ********************/
/**************************************************/

/*ϵͳ����*/
#define VOS_SYS_ERR                         -1
/*��������*/
#define VOS_SYS_PARAM_INVALID               -2
/*��ʱ����*/
#define VOS_SYS_TIMEOUT                     -3
/*����*/
#define VOS_SYS_EWOULDBLOCK                 -4
/*�Ѿ�����*/
#define VOS_SYS_EXIST                       -5
/*�Ѿ�����*/
#define VOS_SYS_NOTEXIST                    -6
/*д����*/
#define VOS_SYS_WERR                        -7

/**************************************************/
/***************** ϵͳ�ض���       ********************/
/**************************************************/


#ifndef SYS_TRUE
#define SYS_TRUE                            VOS_TRUE
#endif
    
#ifndef SYS_FALSE
#define SYS_FALSE                           VOS_FALSE
#endif

#ifndef SYS_ERR_PARAM
#define SYS_ERR_PARAM                       VOS_SYS_PARAM_INVALID
#endif

#ifndef SYS_ERR_TIMEOUT
#define SYS_ERR_TIMEOUT                     VOS_SYS_TIMEOUT
#endif

#ifndef SYS_ERR_EWOULDBLOCK                
#define SYS_ERR_EWOULDBLOCK                 VOS_SYS_EWOULDBLOCK
#endif

#ifndef SYS_ERR_EXIST
#define SYS_ERR_EXIST                       VOS_SYS_EXIST
#endif

#ifndef SYS_ERR_NOTEXIST
#define SYS_ERR_NOTEXIST                    VOS_SYS_NOTEXIST
#endif

#ifndef SYS_ERR_WERR
#define SYS_ERR_WERR                        VOS_SYS_WERR
#endif

#ifndef MAX_PATH
#define MAX_PATH                            260
#endif

#ifndef SYS_IPV4LEN
#define SYS_IPV4LEN                         32
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
/***************** ϵͳͷ�ļ� *********************/
/**************************************************/
/* Linuxϵͳ */
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

/*BPF���*/
#include <linux/if_ether.h>
#include <net/if.h>
#include <linux/filter.h>

/*֧��pcap��չ*/
//#include <pcap/pcap.h>   //suse�и�ͷ�ļ�
//#include <pcap.h>

#if defined(__SUNPRO_C)
#define VAPI __global
#elif ((defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__)
#define VAPI __attribute__ ((visibility("default")))
#else
#define VAPI
#endif /*__SUNPRO_C */

#elif  VOS_PLAT_WIN
/*�ô�������Ǵ�C��ͷ�ļ���C++�������ⶨ��ͷ�ļ�*/
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


#endif
