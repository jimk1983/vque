/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_ctx.c
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : 上下文管理
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_ctx.c
 * @brief vos reactor context manager
 * @author jimk 
 * @date 2019-07
 */
    
#include <vos/vos_pub.h>
#include <vrct/vrct_priv.h>

 
 /**
  * @brief pthread main dispatch 
  * @param pvArgv [in] argv
  */
 INT32_T VRCT_MainDispatch(VOID* pvArgv)
 {
     int                     lRet        = 0;
     UINT64_T                iVal        = 0;
     int                     i           = 0;
     int                     fd          = 0;
     PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvArgv;
     PVRCT_NETEVT_OPT_S      pstNetOpts  = NULL;
     INT32_T                 iEpollFd    = 0;
     INT32_T                 nums        = 0;
     struct epoll_event*     event       = NULL;
     struct                  sockaddr_in stCliaddr;
     socklen_t               len         =sizeof(struct sockaddr_in);
     
     if ( NULL == pvArgv )
     {
         return VOS_ERR;
     }
 
     iEpollFd = pstRctor->stInfo.Epollfd;
 
     event= (struct epoll_event *)calloc(VRCT_EVTMAX, sizeof(struct epoll_event));
     if ( NULL == event )
     {
         PError("[TKD:%02d EID:%02d]=>events calloc error, errno=%d:%s", 
                 pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, 
                 errno, strerror(errno));
         return VOS_ERR;
     }
     
     PEvent("[TKD:%02d EID:%02d]=>epoll wait for event!",
                                 pstRctor->stInfo.TaskID, 
                                 pstRctor->stInfo.Epollfd);
     while(!pstRctor->stInfo.Stop)
     {
         nums = epoll_wait(iEpollFd, event, VRCT_EVTMAX, -1);
         PDebug("[TKD:%02d EID:%02d]=>epoll_wait nums=%d!",
                                 pstRctor->stInfo.TaskID, 
                                 pstRctor->stInfo.Epollfd, 
                                 nums);
         if (0 < nums )
         {
             for(i=0; i < nums; ++i)
             {
                 fd = event[i].data.fd;
                 
                 pstNetOpts = pstRctor->stMgrNet.apstEpollEvtOps[fd];
                 if ( NULL == pstNetOpts )
                 {
                    /*需要保护in/out，可能在netopts中会release相关的fd节点信息*/
                     continue;
                 }
                 
                 switch(pstNetOpts->IoType)
                 {
                     case VRCT_IOTYPE_TIMER:
                         lRet = read(fd, &iVal, sizeof(UINT64_T));
                         if ( lRet < 0 )
                         {
                             PError("[TKD:%02d EID:%02d]=>timer-fd=%d, read error, errno=%d:%s!",
                                 pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, 
                                 fd, errno, strerror(errno));
                             return VOS_ERR;
                         }
                         
                         if ( fd == pstRctor->stMgrTimer.stSlowOpts.fd )
                         {
                             ((PFVRCT_NETEVT_CB)(pstRctor->stMgrTimer.stSlowOpts.stRecv.pvcbFunc))(fd, pstRctor->stMgrTimer.stSlowOpts.stRecv.pvData);
                         }
                         else
                            ((PFVRCT_NETEVT_CB)(pstRctor->stMgrTimer.stQuickOpts.stRecv.pvcbFunc))(fd, pstRctor->stMgrTimer.stQuickOpts.stRecv.pvData);
                         
                         break;
                     case VRCT_IOTYPE_MSG:
                         lRet = eventfd_read(fd, &iVal);
                         if ( lRet < 0 )
                         {
                             PError("[TKD:%02d EID:%02d]=>message-fd=%d, read error, errno=%d:%s!", 
                                 pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                                 fd, errno, strerror(errno));
                             return VOS_ERR;
                         }
                        ((PFVRCT_NETEVT_CB)(pstRctor->stMgrMsQue.stMsqNetOpt.stRecv.pvcbFunc))(fd, pstRctor->stMgrMsQue.stMsqNetOpt.stRecv.pvData);
                         break;
                     case VRCT_IOTYPE_SPAIR:
                         
                         lRet = recvfrom(fd, &iVal, sizeof(UINT64_T), 0, (struct sockaddr*)&stCliaddr, &len);
                         if ( lRet < 0 )
                         {
                             PError("[TKD:%02d EID:%02d]=>message-fd=%d, recvfrom error, errno=%d:%s!", 
                                 pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                                 fd, errno, strerror(errno));
                             return VOS_ERR;
                         }
                         
                         ((PFVRCT_NETEVT_CB)(pstRctor->stMgrMsQue.stPairNetOpt.stRecv.pvcbFunc))(fd, pstRctor->stMgrMsQue.stPairNetOpt.stRecv.pvData);
                         
                         break;
                         
                     case VRCT_IOTYPE_NET:
                             if ( event[i].events & EPOLLIN )
                             {
                                ((PFVRCT_NETEVT_CB)(pstNetOpts->stRecv.pvcbFunc))(fd, pstNetOpts->stRecv.pvData);
                             }
                             
                             if ( event[i].events & EPOLLOUT  )
                             { 
                                 ((PFVRCT_NETEVT_CB)(pstNetOpts->stSend.pvcbFunc))(fd, pstNetOpts->stSend.pvData);
                             }
                         break;
                     default:
                         PError("[TKD:%02d EID:%02d]=>unknow type=%d!", 
                            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                            pstNetOpts->IoType);
                         break;
                 }
             }
         }
     }
     #if VOS_PLAT_LINUX
     pthread_exit(0);
     #endif
     
     return VOS_OK;
 }
 
 
 /**
  * @brief pthread main work pthread
  * @param pvArgv [in] argv
  */
 VOID *VRCT_MainWorkerCb(VOID *pvArgv)
 {
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvArgv;
    
    if ( NULL == pvArgv )
    {
        return NULL;
    }
     
     //semaphore mutex=1;
     //nice(10);
    
    PEvent("[TKD:%02d EID:%02d]=>EPoll [TID=%08x] Start to work!",
                pstRctor->stInfo.TaskID, 
                pstRctor->stInfo.Epollfd, 
                VOS_GetSelfTId());
    
    VOS_ThreadEvent_Notify(&pstRctor->hWaitForStart);
    if ( SYS_ERR == VRCT_MainDispatch(pstRctor) )
    {
        PError("[TKD:%02d EID:%02d]=>EPoll task dispatch exit!", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
    }
    
    VOS_ThreadEvent_Notify(&pstRctor->hWaitForExit);
     
    return NULL;
 }
 




