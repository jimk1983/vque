/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_net.c
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : vrct的网络机制
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_net.c
 * @brief vos reactor network manager
 * @author jimk 
 * @date 2019-07
 */
  
#include <vos/vos_pub.h>
#include <vrct/vrct_priv.h>


/**
 * @brief init the net work event manager
 * @param pvRctor [in] vos reactor
 * @param Maxsize [in] reactor fd max size
 */
INT32_T VRCT_NetworkEvtManagerInit(PVRCT_REACTOR_S          pstRctor, UINT32_T MaxSize)
{
    if ( NULL == pstRctor )
    {
        PError("param error!");
        return VOS_ERR;
    }
    
    pstRctor->stMgrNet.apstEpollEvtOps= (PVRCT_NETEVT_OPT_S *)malloc(sizeof(PVRCT_NETEVT_OPT_S) * MaxSize);
    if ( NULL == pstRctor->stMgrNet.apstEpollEvtOps  )
    {
        PError("[TKD:%02d EID:%02d]=>Malloc failed! MaxSize=%d",
                    pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, MaxSize);
        return SYS_ERR;
    }
    
    memset(pstRctor->stMgrNet.apstEpollEvtOps,0,sizeof(sizeof(PVRCT_NETEVT_OPT_S) * MaxSize));
    
    for(UINT32_T iIndex=0; iIndex < MaxSize; iIndex++)
    {
        pstRctor->stMgrNet.apstEpollEvtOps[iIndex] =NULL;
    }
    
    pstRctor->stMgrNet.MaxSize = MaxSize;

    PEvent("[TKD:%02d EID:%02d]=>Network Manager Init success!",
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd );
    
    return VOS_OK;
}


/**
 * @brief UnInit the net work event manager
 * @param pvRctor [in] vos reactor
 */
VOID VRCT_NetworkEvtManagerUnInit(PVRCT_REACTOR_S          pstRctor)
{
    UINT32_T         iIndex = 0;
    
    if ( NULL != pstRctor )
    {
        for(iIndex=0; iIndex < pstRctor->stMgrNet.MaxSize; iIndex++)
        {
            if ( NULL != pstRctor->stMgrNet.apstEpollEvtOps[iIndex] )
            {
                //PEvent("[TKD:%02d EID:%02d]=>network memory free! index= %d",
                //        pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, iIndex);
                
                /*网络的资源是需要自己释放的*/
                if ( pstRctor->stMgrNet.apstEpollEvtOps[iIndex]->IoType == VRCT_IOTYPE_NET )
                {
                    //free(pstRctor->stMgrNet.apstEpollEvtOps[iIndex]);
                    pstRctor->stMgrNet.apstEpollEvtOps[iIndex] =NULL;
                }
            }
        }
        free(pstRctor->stMgrNet.apstEpollEvtOps);
        pstRctor->stMgrNet.apstEpollEvtOps = NULL;
        
    }
}

/**
 * @brief register the network event
 * @param pvRctor [in] the vos reactor
 * @param PVRCT_NETEVT_OPT_S [in] the socket network option
 * @author jimk 
 */
INT32_T  VRCT_NetworkEvtOptsRegister(     PVRCT_REACTOR_S pstRctor,     PVRCT_NETEVT_OPT_S pstNetOpt)
 {   
    struct  epoll_event     stEvent     = {0};
    
    if ( NULL == pstRctor
        || NULL == pstNetOpt )
    {
        PError("param error!");
        return SYS_ERR_PARAM;
    }
        
    if ( NULL != pstRctor->stMgrNet.apstEpollEvtOps[pstNetOpt->fd])
    {
        PError("[TKD:%02d EID:%02d]=>System error! Found the exist Socket=%d", 
                                                                    pstRctor->stInfo.TaskID,  
                                                                    pstRctor->stInfo.Epollfd,
                                                                    pstNetOpt->fd);
        return SYS_ERR;
    }
    
    stEvent.data.fd = pstNetOpt->fd;
    stEvent.events  = pstNetOpt->EventMask;
    
    if ( 0 > epoll_ctl(pstRctor->stInfo.Epollfd, EPOLL_CTL_ADD, pstNetOpt->fd, &stEvent))
    {
        PError("[TKD:%02d EID:%02d]=>EPoll-Ctrl:(ADD) error!Fd=%d, errno=%d:%s",  
                 pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, pstNetOpt->fd, errno, strerror(errno));
        return SYS_ERR;
    }
    else
    {
        PDebug("[TKD:%02d EID:%02d]=>network register success!fd=%d,Mask=%08x",
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, pstNetOpt->fd, pstNetOpt->EventMask);
    }
    
    pstRctor->stMgrNet.apstEpollEvtOps[pstNetOpt->fd] = pstNetOpt;
    
    return SYS_OK;
}

                                                   
/**
* @brief unregister the network event
* @param pvRctor [in] the vos reactor
* @param fd [in] the socket
* @author jimk 
*/
VOID   VRCT_NetworkEvtOptsUnRegister(PVRCT_REACTOR_S             pstRctor,  PVRCT_NETEVT_OPT_S pstNetOpt)
{
    struct epoll_event      stEvent     = {0};
    INT32_T                 fd          = 0;

    if ( NULL == pstNetOpt ) 
    {
        PError("Param error!");
        return;
    }
    
    fd = pstNetOpt->fd;
    if (fd >= VRCT_FDMAX-1 ) 
    {
        PError("Param error!fd=%d", fd);
        return;
    }
    
    stEvent.data.fd = fd;  
    stEvent.events  = VRCT_POLL_LTINOUT;
    
    if ( pstRctor->stMgrNet.apstEpollEvtOps[fd] != NULL )
    { 
        pstRctor->stMgrNet.apstEpollEvtOps[fd] = NULL;
    }
    
    /*不能在close(fd)之后调用，错误码为0, success*/
    if ( 0 > epoll_ctl(pstRctor->stInfo.Epollfd, EPOLL_CTL_DEL, fd, &stEvent))
    {
        PError("[TKD:%02d EID:%02d]=>EPoll-Ctrl:(DEL) error!Fd=%d, errno=%d:%s",  
                    pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                    fd, errno, strerror(errno));
        return;
    }
}


/**
 * @brief register the network event
 * @param pvRctor [in] the vos reactor
 * @param fd [in] the socket
 * @param EvtMask [in] the event mask
 * @param pfRecvCb [in] the recv callback
 * @param pfSendCb [in] the send callback
 * @param pvctx [in] the context
 * @author jimk 
 */
INT32_T  VRCT_NetworkEvtRegister(     PVRCT_REACTOR_S pstRctor,
                                                    INT32_T fd,
                                                    INT32_T IoType,
                                                    INT32_T EvtMask,
                                                    PFVRCT_NETEVT_CB pfRecvCb, 
                                                    PFVRCT_NETEVT_CB pfSendCb, 
                                                    VOID* pvCtx)
 {   
    PVRCT_NETEVT_OPT_S      pstNetOpts  = NULL;
    struct  epoll_event     stEvent     = {0};
    
     if ( NULL == pstRctor
         || 0 >= fd
         || VRCT_FDMAX-1 < fd )
     {
         PError("param error!");
         return SYS_ERR_PARAM;
     }
 
     pstNetOpts = (PVRCT_NETEVT_OPT_S)malloc(sizeof(VRCT_NETEVT_OPT_S));
     if ( NULL == pstNetOpts )
     {
         PError("[TKD:%02d EID:%02d]=>malloc failed.fd=%d", pstRctor->stInfo.TaskID,  
                                                            pstRctor->stInfo.Epollfd,
                                                            fd);
         return SYS_ERR;
     }
     
     memset(pstNetOpts,0,sizeof(VRCT_NETEVT_OPT_S));
     
     VRCT_NETCALLBACK_INIT( pstNetOpts,
                             fd,
                             IoType,
                             EvtMask,
                             pfRecvCb,
                             pfSendCb,
                             pvCtx);
    
    if ( NULL != pstRctor->stMgrNet.apstEpollEvtOps[fd])
    {
        PError("[TKD:%02d EID:%02d]=>System error! Found the exist Socket=%d", 
                                                                    pstRctor->stInfo.TaskID,  
                                                                    pstRctor->stInfo.Epollfd,
                                                                    fd);
        free(pstNetOpts);
        return SYS_ERR;
    }
    
    stEvent.data.fd = pstNetOpts->fd;
    stEvent.events  = pstNetOpts->EventMask;
    
    if ( 0 > epoll_ctl(pstRctor->stInfo.Epollfd, EPOLL_CTL_ADD, pstNetOpts->fd, &stEvent))
    {
        PError("[TKD:%02d EID:%02d]=>EPoll-Ctrl:(ADD) error!Fd=%d, errno=%d:%s",  
                 pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, fd, errno, strerror(errno));
        free(pstNetOpts);
        return SYS_ERR;
    }
    
    pstRctor->stMgrNet.apstEpollEvtOps[fd] = pstNetOpts;
    
    return SYS_OK;
}

                                                   
/**
* @brief unregister the network event
* @param pvRctor [in] the vos reactor
* @param fd [in] the socket
* @author jimk 
*/
VOID   VRCT_NetworkEvtUnRegister(PVRCT_REACTOR_S             pstRctor,INT32_T fd)
{
    struct epoll_event      stEvent     = {0};

    if ( 0 >= fd
     || fd >= VRCT_FDMAX-1 )
    {
        PError("Param error!fd=%d", fd);
        return;
    }
    
    stEvent.data.fd = fd;  
    stEvent.events  = VRCT_POLL_LTINOUT;
    
    if ( pstRctor->stMgrNet.apstEpollEvtOps[fd] != NULL )
    { 
        free(pstRctor->stMgrNet.apstEpollEvtOps[fd]);
        pstRctor->stMgrNet.apstEpollEvtOps[fd] = NULL;
    }
    
    /*不能在close(fd)之后调用，错误码为0, success*/
    if ( 0 > epoll_ctl(pstRctor->stInfo.Epollfd, EPOLL_CTL_DEL, fd, &stEvent))
    {
        PError("[TKD:%02d EID:%02d]=>EPoll-Ctrl:(DEL) error!Fd=%d, errno=%d:%s",  
                    pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                    fd, errno, strerror(errno));
        return;
    }
}
 
                
/**
* @brief ctrl the network event
* @param pvRctor [in] the vos reactor
* @param fd [in] the socket
* @param ctrlopt [in] the socket
* @author jimk 
*/
 INT32_T   VRCT_NetworkEvtCtrl(PVRCT_REACTOR_S         pstRctor, INT32_T fd, INT32_T OptMask)
 {
    struct epoll_event      stEvent     = {0};
    
    if ( 0 >= fd
     || fd >= VRCT_FDMAX-1 )
    {
        PError("Param error! fd=%d", fd);
        return SYS_ERR_PARAM;
    }
    
    stEvent.data.fd     = fd;
    stEvent.events      = OptMask;
    
    if ( 0 > epoll_ctl(pstRctor->stInfo.Epollfd, EPOLL_CTL_MOD, fd, &stEvent))
    {
        PError("[TKD:%02d EID:%02d]=>EPoll-Ctrl:(MOD) error! SockFd=%d, errno=%d:%s",  
                    pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                    fd, errno, strerror(errno));
        
        return SYS_ERR;
    }
     
     return SYS_OK;
 }





