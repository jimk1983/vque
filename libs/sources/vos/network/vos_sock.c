/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_sock.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : socket的函数封装
  函数列表   :
              VOS_SOCK_Accept
              VOS_SOCK_Bind
              VOS_SOCK_Close
              VOS_SOCK_Connect
              VOS_SOCK_Create
              VOS_SOCK_GetHostAddress
              VOS_SOCK_GetPeerAddress
              VOS_SOCK_Listen
              VOS_SOCK_Recv
              VOS_SOCK_Recvfrom
              VOS_SOCK_Send
              VOS_SOCK_SendTo
              VOS_SOCK_SetOption
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/

#include <vos/vos_pub.h>



void VOS_SOCK_SignalHandle(int32_t signleNo)     
{     
    //do   something;     
}

int32_t VOS_SOCK_Init()
{
#if VOS_PLAT_WIN

    WORD      wVersionRequested;
    WSADATA wsaData;
    int32_t       err;

    /*请求1.1版本的winsock库*/
    wVersionRequested = MAKEWORD(1,1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if ( 0 != err )
    {
        return VOS_ERR;
    }
    
#elif VOS_PLAT_LINUX
#if 0
    /*捕捉异常SIGPIPE信号，不结束进程*/
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    
    if ( sigemptyset(&sa.sa_mask) == -1         //初始化信号集为空
        || sigaction(SIGPIPE, &sa, 0) == -1)    //屏蔽SIGPIPE信号
    {   
        printf("failed to ignore SIGPIPE; sigaction\n");
        return VOS_ERR;
    }
#elif 1
//线程级的设置
    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGPIPE);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0) {
        printf("block sigpipe error\n");
} 
#else

//自定义
signal(SIGPIPE, signal_handle);  
 
#endif
#endif
    
    return VOS_OK;
}

int32_t VOS_SOCK_UnInit()
{
#if VOS_PLAT_WIN
    WSACleanup();
#endif

    return VOS_OK;
}


int32_t VOS_SOCK_SendSize(int32_t iFd, char *pcBuf, int32_t size)
{
    int32_t         iRet=0, offset=0;
    int32_t         ileft = size;
    for ( ;; ) 
    {
        iRet = send(iFd, pcBuf+offset, ileft, MSG_NOSIGNAL);
        if (iRet > 0) 
        {
            offset+=iRet;
            ileft-=offset;
            if ( 0 == ileft )
            {
                return offset;
            }
            
            continue;
        }
        else if( iRet == 0) 
        {
            printf("fd=%d, send zero packet! size=%d\n", iFd, size);
            return offset;
        }
        else
        {
            if ( errno == EAGAIN )
            {
                printf("fd=%d, send blocking! errno=%d:%s,ileft=%d, size=%d\n",
                        iFd, errno, strerror(errno), ileft, size);
                usleep(100);
                continue;
            }
        
            if ( errno == EINTR )
            {
                continue;
            }
            
            printf("fd=%d, send errno=%d:%s\n", iFd, errno, strerror(errno));
            
            return -1;
        }
    }
}

int32_t VOS_SOCK_Send(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    
    if ( NULL == pcBuf)
    {
        return VOS_ERR;
    }

    lRet = send(lSockfd, pcBuf, ulBufLen, MSG_NOSIGNAL);// MSG_NOSIGNAL);
    if( lRet <= 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            printf("VOS_SOCK_Send, send errno=%d:%s\n", errno, strerror(errno));
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }
    
    return lRet;
}

int32_t VOS_SOCK_RAW_SendTo(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, char *acIfName, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    struct sockaddr sa;
    
    if ( NULL == pcBuf
        || NULL == acIfName )
    {
        return VOS_ERR;
    }

    memset((char *)&sa, 0, sizeof(sa));
    strcpy((char *)sa.sa_data, acIfName);
    
    lRet = sendto(lSockfd, pcBuf, ulBufLen, 0, &sa, sizeof(sa));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            printf("vos raw socket send error, errno=%d", errno);
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }        

    return lRet;
    
}

int32_t VOS_SOCK_RAW_Recvfrom(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    socklen_t lLen = 0;
    if ( NULL == pcBuf)
    {
        return VOS_ERR;
    }

    memset(pcBuf, 0, ulBufLen);
    
    lRet = recvfrom(lSockfd, pcBuf, ulBufLen, 0, NULL, &lLen);
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            
            return VOS_ERR;
        }
    }
    else if ( lRet == 0)
    {
        (*plErrorStatus) = VOS_SOCK_FINISH;
        
        return VOS_ERR;
    }
    
    return lRet;
}


int32_t VOS_SOCK_SendTo(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, char *pcPeerAddress, uint32_t ulPeerPort, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    
    if ( NULL == pcBuf
        || NULL == pcPeerAddress )
    {
        return VOS_ERR;
    }
    stPeerAddr.sin_family = AF_INET;
    stPeerAddr.sin_addr.s_addr = inet_addr(pcPeerAddress);
    stPeerAddr.sin_port = htons(ulPeerPort);
    
    lRet = sendto(lSockfd, pcBuf, ulBufLen, MSG_NOSIGNAL, (struct sockaddr *)&stPeerAddr, sizeof(stPeerAddr));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }        

    return lRet;
}

int32_t VOS_SOCK_SendToN(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, VOS_IPADDR_T uiPeerAddress, uint32_t ulPeerPort, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    
    if ( NULL == pcBuf )
    {
        return VOS_ERR;
    }
    stPeerAddr.sin_family = AF_INET;
    stPeerAddr.sin_addr.s_addr = uiPeerAddress;
    stPeerAddr.sin_port = htons(ulPeerPort);
    
    lRet = sendto(lSockfd, pcBuf, ulBufLen, 0, (struct sockaddr *)&stPeerAddr, sizeof(stPeerAddr));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }        

    return lRet;
}


int32_t VOS_SOCK_Recv(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    
    if ( NULL == pcBuf)
    {
        return VOS_ERR;
    }
    
AGAIN:
    lRet = recv(lSockfd, pcBuf, ulBufLen, 0);
    if ( lRet > 0 )
    {
        return lRet;
    }
    else if ( 0 == lRet )
    {
        if ( errno == 4 )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            goto AGAIN;
        }
        
        (*plErrorStatus) = VOS_SOCK_FINISH;
        
        return VOS_ERR;
    }
    else
    {
        if ( errno == 4 )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            goto AGAIN;
        }
        
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = errno;
            
            return VOS_ERR;
        }
    }
    
    return lRet;
}




int32_t VOS_SOCK_Recvfrom(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, char *pcPeerAddress, uint32_t *pulPeerPort, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    socklen_t lSize = sizeof(stPeerAddr);
    
    if ( NULL == pcBuf  || NULL == plErrorStatus)
    {
        return VOS_ERR;
    }
        
    lRet = recvfrom(lSockfd, pcBuf, ulBufLen, 0, (struct sockaddr *)&stPeerAddr, &lSize);
    if( lRet <= 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            //VOS_Printf("VOS_SOCK_Recvfrom error!errno=%d", errno);
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }   
    
    if ( NULL != pcPeerAddress )
    {
        memcpy(pcPeerAddress, inet_ntoa(stPeerAddr.sin_addr), VOS_SOCK_IPADDR_LEN);
    }
    
    if ( NULL != pulPeerPort )
    {
        (*pulPeerPort) = ntohs(stPeerAddr.sin_port);
    }
    
    return lRet;
}


int32_t VOS_SOCK_RecvfromN(int32_t lSockfd, char *pcBuf, uint32_t ulBufLen, VOS_IPADDR_T *puiPeerAddress, uint32_t *pulPeerPort, int32_t *plErrorStatus)
{
    int32_t lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    socklen_t lSize = sizeof(stPeerAddr);
    
    if ( NULL == pcBuf  || NULL == plErrorStatus)
    {
        return VOS_ERR;
    }
        
    lRet = recvfrom(lSockfd, pcBuf, ulBufLen, 0, (struct sockaddr *)&stPeerAddr, &lSize);
    if( lRet <= 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            //VOS_Printf("VOS_SOCK_Recvfrom error!errno=%d", errno);
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
        }
        return VOS_ERR;
    }   
    
    if ( NULL != puiPeerAddress )
    {
        *puiPeerAddress = stPeerAddr.sin_addr.s_addr;
        printf("recvfrom the addr=%08x\n", stPeerAddr.sin_addr.s_addr);
    }
    
    if ( NULL != pulPeerPort )
    {
        (*pulPeerPort) = ntohs(stPeerAddr.sin_port);
    }
    
    return lRet;
}


/*****************************************************************************
 函 数 名  : VOS_SOCK_Bind
 功能描述  : 绑定具体的地址和端口
 输入参数  : int32_t lSockfd          
             char *pcLocalAddress  
             uint32_t ulPort          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月30日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
int32_t VOS_SOCK_Bind(int32_t lSockfd, char *pcLocalAddress, uint32_t ulPort)
{
    struct sockaddr_in stLocalAddr = {0};
    
    if(NULL == pcLocalAddress
    || 0 == ulPort
    || 0>= lSockfd)
    {
        return VOS_ERR;
    }

    stLocalAddr.sin_family = AF_INET;
    stLocalAddr.sin_addr.s_addr = inet_addr(pcLocalAddress);
    stLocalAddr.sin_port = htons(ulPort);
    
    if(-1 == bind(lSockfd, (struct sockaddr *)&stLocalAddr, sizeof(stLocalAddr)))
    {
        printf("bind socket error!(errno=%d)",errno);
        return VOS_ERR;
    }
    
    return VOS_OK;
}

int32_t VOS_SOCK_Listen(int32_t lSockfd, uint32_t ulNum)
{
    if ( 0 >= lSockfd )
    {
        return VOS_ERR;
    }
    
    if( -1 == listen(lSockfd,ulNum) )
    {
        return VOS_ERR;
    }
    
    return VOS_OK;
}

int32_t VOS_SOCK_Accept(int32_t lListenfd, char *pcClientAddr, uint16_t *pusClientPort, int32_t *plErrorStatus)
{
    int32_t lClientFd = 0;
    struct sockaddr_in stClientAddr = {0};
    socklen_t stSerLen = sizeof(stClientAddr);

    
    if ( 0 >= lListenfd 
        || NULL == pcClientAddr
        || NULL == pusClientPort)
    {
        (*plErrorStatus) = VOS_SOCK_ERROR;
        return VOS_SOCK_INVALID;
    }
    lClientFd = accept(lListenfd, (struct sockaddr *)&stClientAddr, &stSerLen);
    if( lClientFd < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            usleep(1000);
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_SOCK_INVALID;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_SOCK_INVALID;
        }
    }

    (*pusClientPort) = ntohs(stClientAddr.sin_port);
    
    memcpy(pcClientAddr,inet_ntoa(stClientAddr.sin_addr), strlen(inet_ntoa(stClientAddr.sin_addr)));
    
    return lClientFd;
}

int32_t VOS_SOCK_Connect(int32_t lSockfd, char *pcServAddr, uint16_t usServPort, int32_t *plErrorStatus)
{
    struct sockaddr_in stServAddr = {0};
    int32_t lRet =0;

    if ( 0 >= lSockfd 
        || NULL == pcServAddr
        || NULL == plErrorStatus)
    {
        (*plErrorStatus) = VOS_SOCK_ERROR;
        return VOS_ERR;
    }

    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(usServPort);
    stServAddr.sin_addr.s_addr = inet_addr(pcServAddr);
    
    lRet = connect(lSockfd, (struct sockaddr *)&stServAddr, sizeof(stServAddr));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_OK;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }
    
    (*plErrorStatus) = VOS_SOCK_OK;
    return VOS_OK;
}



int32_t VOS_SOCK_ConnectNetAddr(int32_t lSockfd, VOS_IPADDR_T uiIpaddr, uint16_t usServPort, int32_t *plErrorStatus)
{
    struct sockaddr_in stServAddr = {0};
    int32_t lRet =0;

    if ( 0 >= lSockfd 
        || NULL == plErrorStatus)
    {
        (*plErrorStatus) = VOS_SOCK_ERROR;
        return VOS_ERR;
    }

    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(usServPort);
    stServAddr.sin_addr.s_addr = uiIpaddr;
    
    lRet = connect(lSockfd, (struct sockaddr *)&stServAddr, sizeof(stServAddr));
    if( lRet < 0 )
    {
        /*115: operation now in progress, 正常*/
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_OK;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }

    (*plErrorStatus) = VOS_SOCK_OK;
    
    return VOS_OK;
}


int32_t VOS_SOCK_GetHostSockAddr(int32_t lSockfd, char *pcHostAddress, uint32_t *pulPort)
{
    struct sockaddr_in stHostAddr = {0};
    socklen_t stSerLen = sizeof(stHostAddr);
    
    if ( 0 >= lSockfd 
        || NULL == pcHostAddress)
    {
        return VOS_ERR;
    }
    
    if(-1 == getsockname(lSockfd, (struct sockaddr *)&stHostAddr, &stSerLen))
    {
        return VOS_ERR;
    }
    
    memcpy(pcHostAddress,inet_ntoa(stHostAddr.sin_addr),VOS_SOCK_IPADDR_LEN);
    (*pulPort) = stHostAddr.sin_port;
    
    return VOS_OK;
}



int32_t VOS_SOCK_GetPeerSockAddr(int32_t lSockfd, char *pcPeerAddress, uint32_t *pulPort)
{
    struct sockaddr_in stPeerAddr = {0};
    socklen_t ulSerLen = sizeof(stPeerAddr);
    
    if ( 0 >= lSockfd 
        || NULL == pcPeerAddress)
    {
        return VOS_ERR;
    }
    
    if( -1 == getpeername((int32_t)lSockfd, (struct sockaddr *)&stPeerAddr, &ulSerLen))
    {
        return VOS_ERR;
    }

    memcpy(pcPeerAddress,inet_ntoa(stPeerAddr.sin_addr), VOS_SOCK_IPADDR_LEN);
    (*pulPort) = stPeerAddr.sin_port;
        
    return VOS_OK;
}


int32_t VOS_SOCK_Create(uint32_t ulType)
{
    int32_t lSockfd = VOS_SOCKET_INVALID;

    if ( VOS_SOCK_TYPE_TCP == ulType )
    {
        lSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    }
    else if ( VOS_SOCK_TYPE_UDP == ulType )
    {
        lSockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    else if (VOS_SOCK_TYPE_ARP == ulType)
    {
        /*注意，该模式需要将网卡设置为混杂模式?
        ifconfig eth0 promisc 设置混杂
        ifconfig eth0 -promisc　取消混杂*/
        lSockfd = socket ( PF_INET,SOCK_PACKET, htons( ETH_P_ARP));
    }
    else if (VOS_SOCK_TYPE_RAW == ulType)
    {
        /*注意，该模式需要将网卡设置为混杂模式?
        ifconfig eth0 promisc 设置混杂
        ifconfig eth0 -promisc　取消混杂*/
        //--fedora下面工作很好，但是centos下就会出现拷贝报文现象
        //lSockfd = socket ( PF_INET,SOCK_PACKET, VOS_htons( ETH_P_IP));   

        //--centos下面接收可以工作，发送不行
        //lSockfd = socket (PF_PACKET, SOCK_RAW, VOS_htons(ETH_P_IP)); 
        lSockfd = socket (PF_PACKET, SOCK_PACKET, htons(ETH_P_IP)); 
    }
    else 
    {
        lSockfd = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    }

    if ( VOS_SOCKET_INVALID > lSockfd )
    {
        return VOS_SOCKET_INVALID;
    }

    return lSockfd;
}

int32_t VOS_SOCK_Close(int32_t lSockfd)
{
    if ( VOS_SOCKET_INVALID == lSockfd 
        || 0 == lSockfd )
    {
        return VOS_ERR;
    }
    /*保护不能关闭fd=0的句柄*/
    close(lSockfd);

    return VOS_OK;
}

int32_t VOS_SOCK_Shutdown(int32_t lSockfd)
{
    if ( VOS_SOCKET_INVALID == lSockfd )
    {
        return VOS_ERR;
    }

    shutdown(lSockfd, SHUT_RDWR); //SHUT_RDWR(2), SHUT_RD(0), SHUT_WR(1)

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_SOCK_SetBPFOption
 功能描述  : 设置rawsocket的BPF过滤规则
 输入参数  : int32_t lSockfd  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月9日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
int32_t VOS_SOCK_SetBPFOption(int32_t lSockfd)
{
    struct sock_filter BPF_code[6]= {
        { 0x20, 0, 0, 0x00000002 },
        { 0x15, 0, 3, 0x29f4cd78 },
        { 0x28, 0, 0, 0x00000000 },
        { 0x15, 0, 1, 0x0000000c },
        { 0x6, 0, 0, 0x00000060 },
        { 0x6, 0, 0, 0x00000000 }
    };                           
    struct sock_fprog Filter;

    Filter.len = 6;
    Filter.filter = BPF_code;

    /*BPF过滤规则*/
    if(setsockopt(lSockfd, SOL_SOCKET, SO_ATTACH_FILTER,&Filter, sizeof(Filter))<0)
    {
        return VOS_ERR;
    }
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_SOCK_SetProMiscMode
 功能描述  : 设置混杂模式
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月10日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
int32_t VOS_SOCK_SetProMiscMode(int32_t lSockfd, char *pcIfName)
{
#if VOS_PLAT_LINUX

    struct ifreq ethreq;
    
    if ( NULL == pcIfName )
    {
        return VOS_ERR;
    }

    strncpy(ethreq.ifr_name,pcIfName,IFNAMSIZ);
    
    if (ioctl(lSockfd,SIOCGIFFLAGS, &ethreq)==-1) {
        return VOS_ERR;
    }
    
    ethreq.ifr_flags |= IFF_PROMISC;
    
    if (ioctl(lSockfd,SIOCSIFFLAGS,&ethreq)==-1) {
        return VOS_ERR;
    }
#endif
    return VOS_OK;
}

/*****************************************************************************
 函 数 名: EQue_API_SetServSocketCreate
 功能描述  : 新建服务端的socket
 输入参数  :   char *pcAddr          
             int32_t iPort           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年4月2日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
int32_t VOS_SOCK_ServCreate(char *pcAddr, int32_t iPort)
{
    int         lSockfd         = SYS_ERR;
    int32_t     lRet            = 0;
    int32_t     lnonBlockflag   = 1;
    int32_t     lSndBufsize     = VOS_SOCK_SNDWINSIZE;
    int32_t     lRcvBufsize     = VOS_SOCK_RCVWINSIZE;
    struct  linger so_linger= {0};
    int32_t     lOn             = 1;
    struct sockaddr_in stServerAddr = {0};
    int         enable = 1;

    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    
    lSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if ( 0 >= lSockfd )
    {
        printf("setsockopt socket error!(errno=%d)\n",errno);
        return SYS_ERR;
    }
        
    lRet = ioctl(lSockfd, FIONBIO, &lnonBlockflag);
    if ( 0 != lRet )
    {
        printf("setsockopt socket FIONBIO error!(errno=%d)\n",errno);
        close(lSockfd);
        return SYS_ERR;
    }

    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_SNDBUF,(const char *)&lSndBufsize, sizeof(int32_t) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_SNDBUF error!(errno=%d)\n",errno);
        close(lSockfd);
        return SYS_ERR;
    }

    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_RCVBUF,(const char *)&lRcvBufsize, sizeof(int32_t) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_RCVBUF error!(errno=%d)\n",errno);
        close(lSockfd);
        return SYS_ERR;
    }
    
    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_REUSEADDR,(void*)&lOn, sizeof(lOn));
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_REUSEADDR error!(errno=%d)\n",errno);
        close(lSockfd);
        return SYS_ERR;
    }

    /*关闭linger*/
    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_LINGER,(const char *)&so_linger, sizeof(so_linger) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_LINGER error!(errno=%d)\n",errno);
        close(lSockfd);
        return SYS_ERR;
    }
    
    lRet = setsockopt(lSockfd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable));
    if ( 0 != lRet )
    {
        printf("setsockopt socket TCP_NODELAY error!(errno=%d:%s)\n",errno, strerror(errno));
        close(lSockfd);
        return SYS_ERR;
    }
    
    stServerAddr.sin_family = AF_INET;
    if ( NULL != pcAddr )
    {
        stServerAddr.sin_addr.s_addr = inet_addr((const char *)pcAddr);
    }
    else
        stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    stServerAddr.sin_port = htons((short)iPort);
    
    if(-1 == bind(lSockfd, (struct sockaddr *)&stServerAddr, sizeof(stServerAddr)))
    {
        printf("bind socket error!(errno=%d),port=%d\n", errno, iPort);
        close(lSockfd);
        return SYS_ERR;
    }
    
    if( -1 == listen(lSockfd, 10240) )
    {
        printf("listen socket error!(errno=%d)\n",errno);
        close(lSockfd);
        return SYS_ERR;
    }

    return lSockfd;
}


/*****************************************************************************
 函 数 名  : EQue_API_ClntSocketCreate
 功能描述  : 直接创建客户端socket
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年4月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
int32_t VOS_SOCK_ClntCreate()
{
    int         lSockfd         = SYS_ERR;
    int32_t     lRet            = 0;
    int32_t     lnonBlockflag   = 1;
    int32_t     lSndBufsize     = VOS_SOCK_SNDWINSIZE;
    int32_t     lRcvBufsize     = VOS_SOCK_RCVWINSIZE;
    int         enable = 1;
    
    lSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if ( 0 >= lSockfd )
    {
        printf("socket error!(errno=%d:%s)\n",errno, strerror(errno));
        return SYS_ERR;
    }
        
    lRet = ioctl(lSockfd, FIONBIO, &lnonBlockflag);
    if ( 0 != lRet )
    {
        printf("setsockopt socket FIONBIO error!(errno=%d:%s)\n",errno, strerror(errno));
        close(lSockfd);
        return SYS_ERR;
    }
    
    lRet = setsockopt(lSockfd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable));
    if ( 0 != lRet )
    {
        printf("setsockopt socket TCP_NODELAY error!(errno=%d:%s)\n",errno, strerror(errno));
        close(lSockfd);
        return SYS_ERR;
    }
    
    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_SNDBUF,(const char *)&lSndBufsize, sizeof(int32_t) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_SNDBUF error!(errno=%d:%s)\n",errno, strerror(errno));
        close(lSockfd);
        return SYS_ERR;
    }

    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_RCVBUF,(const char *)&lRcvBufsize, sizeof(int32_t) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_RCVBUF error!(errno=%d:%s)\n",errno, strerror(errno));
        close(lSockfd);
        return SYS_ERR;
    }
    
    return lSockfd;
}



/*默认都设置为非阻塞， 服务端还有个Reuse*/
int32_t VOS_SOCK_SetOption(int32_t lSockfd)
{
    int32_t lRet =0;
    int32_t lnonBlockflag = 1;
    int32_t lSndBufsize = 65535;//EQUE_SOCKBUF_SIZE;
    int32_t lRcvBufsize = 65535;//EQUE_SOCKBUF_SIZE;
    struct linger so_linger = {0};
    int     enable = 1;
    
    /*不允许字节逗留,直接关闭*/
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    
    int  iKeepAlive = 1;
    /*如果连接在60秒内没有任何数据往来，则进行探测*/
    int  iKeepIdle = 60;
    /*探测法宝的间隔时间为5S*/
    int  iKeepInterval = 5;
    /*探测尝试次数，如果第一次探测包就收到响应，则后2次不发*/
    int  ikeepCount = 3;    
    
    if( -1 == lSockfd )
    {
        return VOS_ERR;
    }

    lRet = ioctl(lSockfd, FIONBIO, &lnonBlockflag);
    if ( 0 != lRet )
    {
        printf("setsockopt socket FIONBIO error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }
    
    lRet = setsockopt(lSockfd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable));
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_SNDBUF error!(errno=%d:%s)\n",errno, strerror(errno));
        close(lSockfd);
        return SYS_ERR;
    }
    
    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_SNDBUF,(const char *)&lSndBufsize, sizeof(int32_t) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_SNDBUF error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }
    
    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_RCVBUF,(const char *)&lRcvBufsize, sizeof(int32_t) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_RCVBUF error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }

    /*关闭linger*/
    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_LINGER,(const char *)&so_linger, sizeof(so_linger) );
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_LINGER error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }
    
    /*开启KeepAlive属性*/
    lRet = setsockopt(lSockfd, SOL_SOCKET, SO_KEEPALIVE, (void*)&iKeepAlive, sizeof(iKeepAlive));
    if ( 0 != lRet )
    {
        printf("setsockopt socket SO_KEEPALIVE error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }
    
    lRet = setsockopt(lSockfd, SOL_TCP, TCP_KEEPIDLE, (void*)&iKeepIdle, sizeof(iKeepIdle));
    if ( 0 != lRet )
    {
        printf("setsockopt socket TCP_KEEPIDLE error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }
    
    lRet = setsockopt(lSockfd, SOL_TCP, TCP_KEEPINTVL, (void*)&iKeepInterval, sizeof(iKeepInterval));
    if ( 0 != lRet )
    {
        printf("setsockopt socket TCP_KEEPINTVL error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }

    lRet = setsockopt(lSockfd, SOL_TCP, TCP_KEEPCNT, (void*)&ikeepCount, sizeof(ikeepCount));
    if ( 0 != lRet )
    {
        printf("setsockopt socket TCP_KEEPCNT error!(errno=%d:%s)\n",errno, strerror(errno));
        return VOS_ERR;
    }
    
#if 0
  int32_t lOn = 1;
  lRet = setsockopt(lSockfd, SOL_SOCKET, SO_REUSEADDR,(void*)&lOn, sizeof(lOn));
  if ( 0 != lRet )
  {
      return VOS_ERR;
  }
#endif
#if 0
    /*关闭接收到send多次，(SO_NOSIGPIPE-IOS/MacOS)linux会受到sigPipe, 忽略
    linux: signal(SIGPIPE, SIG_IGN) */
    lRet = setsockopt(lSockfd, SOL_SOCKET, MSG_NOSIGNAL,(const char *)&lOn, sizeof(int32_t) );
    if ( 0 != lRet )
    {
        return VOS_ERR;
    }
#endif
    
    return VOS_OK;
}



/*默认都设置为非阻塞*/
int32_t VOS_SOCK_UDPSetOption(int32_t lUdpSockfd)
{
  int32_t lRet =0;
  int32_t lnonBlockflag = 1;

  int32_t lSndBufsize = VOS_SOCK_SNDWINSIZE;
  int32_t lRcvBufsize = VOS_SOCK_MSGUDP_RCVWINSIZE;

  lRet = ioctl(lUdpSockfd, FIONBIO, &lnonBlockflag);
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }
  
  lRet = setsockopt(lUdpSockfd, SOL_SOCKET, SO_SNDBUF,(const char *)&lSndBufsize, sizeof(int32_t) );
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }
  
  lRet = setsockopt(lUdpSockfd, SOL_SOCKET, SO_RCVBUF,(const char *)&lRcvBufsize, sizeof(int32_t) );
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }

  return VOS_OK;
}



VOS_IPADDR_T  VOS_SOCK_GetHostIpByName(char *acDomain)
{
    VOS_IPADDR_T iRet                   = VOS_INVALID_VAL;
    struct hostent *    psthostent      = NULL;
    struct in_addr **   ppstaddr_list   = NULL;
    struct in_addr InIp = {0};
    
    if ( NULL == acDomain )
    {
        return VOS_INVALID_VAL;
    }
    
    if ((psthostent = gethostbyname(acDomain)) == NULL) {  
        return VOS_INVALID_VAL;
    }

    ppstaddr_list = (struct in_addr **)psthostent->h_addr_list;

    /*可能存在多个IP*/
    //for(i = 0; ppstaddr_list[i] != NULL; i++) {
        //printf("%s ", inet_ntoa(*ppstaddr_list[i]));
    //}
    
    /*目前就取第一个*/
    if ( NULL != ppstaddr_list[0]  )
    {
        InIp = *ppstaddr_list[0];
        iRet= InIp.s_addr;
    }
        
    return iRet;
}





