/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_sock.h
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : SOCK封装的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/

#ifndef _VOS_SOCK_H_
#define _VOS_SOCK_H_


//Create TCP type socket , AF_INET
#define VOS_SOCK_TYPE_TCP   0 
//Create UDP type socket , AF_INET
#define VOS_SOCK_TYPE_UDP   1
//Create RAW type socket , AF_INET
#define VOS_SOCK_TYPE_RAW   2
//Create ARP type socket , PF_INET
#define VOS_SOCK_TYPE_ARP   3

//INVALID Socket defined
#define VOS_SOCKET_INVALID  0

//Socket Status
#define VOS_SOCK_OK                             0
#define VOS_SOCK_ERROR                      -1
#define VOS_SOCK_EWOULDBLOCK         -2
#define VOS_SOCK_TCP_CONNECTING     -3
#define VOS_SOCK_TCP_CONNECTED       -4
#define VOS_SOCK_SSL_CONNECTING      -5
#define VOS_SOCK_SSL_ACCEPTING         -6
#define VOS_SOCK_SSL_ACCEPTED           -7
#define VOS_SOCK_SSL_EWOULDBLOCK   -8
#define VOS_SOCK_SSL_RECONNECTING  -9
#define VOS_SOCK_SSL_ERROR               -10
#define VOS_SOCK_FINISH                      -11
#define VOS_SOCK_SSL_CONNECTED       -12

//Socket windows size
#define VOS_SOCK_SNDWINSIZE         65535
#define VOS_SOCK_RCVWINSIZE         65535

/*MacOS中最大设置为6M, 因此定位6M， 设置这么大的原因是
开辟足够大的缓存，防止127.0.0.1的内部发送出现丢包*/
#define VOS_SOCK_MSGUDP_RCVWINSIZE    6*1024*1024   

#define VOS_SOCK_IPADDR_LEN         16

/*最大的系统socket号，linux是从小到大依次增加的*/
#define VOS_SOCK_FDSIZE_MAX     20000

#define VOS_SOCK_LOCALHOST_STR     "127.0.0.1"


#define VOS_SOCK_INVALID        -1

int32_t VOS_SOCK_Init();
int32_t VOS_SOCK_UnInit();
int32_t VOS_SOCK_Send(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, int32_t *peErrorStatus);
int32_t VOS_SOCK_SendSize(int32_t iFd, char *pcBuf, int32_t size);
int32_t VOS_SOCK_SendTo(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, char *pcPeerAddress, uint32_t ulPeerPort, int32_t *peErrorStatus);
int32_t VOS_SOCK_SendToN(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, VOS_IPADDR_T uiPeerAddress, uint32_t ulPeerPort, int32_t *plErrorStatus);

int32_t VOS_SOCK_Recv(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, int32_t *peErrorStatus);
int32_t VOS_SOCK_Recvfrom(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, char *pcPeerAddress, uint32_t *ulPeerPort, int32_t *peErrorStatus);
int32_t VOS_SOCK_RecvfromN(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, VOS_IPADDR_T *puiPeerAddress, uint32_t *pulPeerPort, int32_t *plErrorStatus);
int32_t VOS_SOCK_Bind(int32_t lSockfd, char *pcLocalAddress, uint32_t ulPort);
int32_t VOS_SOCK_Listen(int32_t lSockfd, uint32_t ulNum);
int32_t VOS_SOCK_Accept(int32_t lListenfd, char *pcClientAddr, uint16_t *pusClientPort, int32_t *peErrorStatus);
int32_t VOS_SOCK_Connect(int32_t lSockfd, char *pcServAddr, uint16_t usServPort, int32_t *peErrorStatus);
int32_t VOS_SOCK_GetHostSockAddr(int32_t lSockfd, char *pcHostAddress, uint32_t *pulPort);
int32_t VOS_SOCK_GetPeerSockAddr(int32_t lSockfd, char *pcPeerAddress, uint32_t *pulPort);
int32_t VOS_SOCK_Create(uint32_t ulType);
int32_t VOS_SOCK_Close(int32_t lSockfd);
int32_t VOS_SOCK_Shutdown(int32_t lSockfd);
int32_t VOS_SOCK_SetOption(int32_t lSockfd);
int32_t VOS_SOCK_UDPSetOption(int32_t lUdpSockfd);
int32_t VOS_SOCK_SetBPFOption(int32_t lSockfd);
int32_t VOS_SOCK_SetProMiscMode(int32_t lSockfd, char *pcIfName);
int32_t VOS_SOCK_ServCreate(char *pcAddr, int32_t iPort);
int32_t VOS_SOCK_ClntCreate();


int32_t VOS_SOCK_RAW_SendTo(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, char *acIfName, int32_t *plErrorStatus);
int32_t VOS_SOCK_RAW_Recvfrom(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, int32_t *plErrorStatus);

VOS_IPADDR_T  VOS_SOCK_GetHostIpByName(char *acDomain);
int32_t VOS_SOCK_ConnectNetAddr(int32_t lSockfd, VOS_IPADDR_T uiIpaddr, uint16_t usServPort, int32_t *plErrorStatus);

#endif


