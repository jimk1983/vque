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
 * @brief register the network event
 * @param pvRctor [in] the vos reactor
 * @param fd [in] the socket
 * @param EvtMask [in] the event mask
 * @param pfRecvCb [in] the recv callback
 * @param pfSendCb [in] the send callback
 * @param pvctx [in] the context
 * @author jimk 
 */
INT32_T  VRCT_API_NetworkEvtOptsRegister(     VOID *pvRctor, 
                                                    INT32_T fd,
                                                    INT32_T EvtMask,
                                                    PFVRCT_COMM_CB pfRecvCb, 
                                                    PFVRCT_COMM_CB pfSendCb, 
                                                    VOID* pvCtx)
 {   
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvRctor;
    PVRCT_NETEVT_OPT_S      pstNetOpts  = NULL;
    struct  epoll_event     stEvent     = {0};
    
     if ( NULL == pvRctor
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
     
     VOS_DLIST_INIT(&pstNetOpts->stNode);
    
     VRCT_NETCALLBACK_INIT( pstNetOpts,
                             fd,
                             VRCT_IOTYPE_NET,
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
        PError("[TKD:%02d EID:%02d]=>EPoll-Ctrl:(ADD) error!EpollFd=%d, Fd=%d, errno=%d:%s",  
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
VOID   VRCT_API_NetworkEvtOptsUnRegister(VOID *pvRctor,           INT32_T fd)
{
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvRctor;
    struct epoll_event      stEvent     = {0};

    if ( 0 >= fd
     || fd >= VRCT_FDMAX-1 )
    {
        PError("Param error! fd=%d", fd);
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
 INT32_T   VRCT_API_NetworkEvtOptsCtrl(VOID *pvRctor,         INT32_T fd, INT32_T CtrlOpt)
 {
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvRctor;
    struct epoll_event      stEvent     = {0};
    
    if ( 0 >= fd
     || fd >= VRCT_FDMAX-1 )
    {
        PError("Param error! fd=%d", fd);
        return SYS_ERR_PARAM;
    }

    stEvent.data.fd     = fd;
    stEvent.events      = CtrlOpt;
    
    if ( 0 > epoll_ctl(pstRctor->stInfo.Epollfd, EPOLL_CTL_MOD, fd, &stEvent))
    {
        PError("[TKD:%02d EID:%02d]=>EPoll-Ctrl:(MOD) error! SockFd=%d, errno=%d:%s",  
                    pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                    fd, errno, strerror(errno));
        
        return SYS_ERR;
    }
     
     return SYS_OK;
 }





