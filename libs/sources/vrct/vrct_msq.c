/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_msq.c
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : vrct的消息队列
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_msq.c
 * @brief vos reactor message queue manager
 * @author jimk 
 * @date 2019-07
 */
    
#include <vos/vos_pub.h>
#include <vrct/vrct_priv.h>


VOID    VRCT_MsgQueManagerUnInit(PVRCT_REACTOR_S          pstRctor);


/**
 * @brief init the message queue event manager
 * @param fd [in] eventfd
 * @param pvCtx [in] reactor 
 */
VOID    VRCT_MsgQueMainCb(INT32_T fd, VOID *pvCtx)
{
    PVOS_DLIST_S            pstEntry    = NULL;
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvCtx;
    PVRCT_MSQ_ENTRY_S       pstMsgNode  = NULL;

    if ( 0 == pstRctor->stMgrMsQue.iUsedNums )
    {
        return;
    }
    
    while(pstRctor->stMgrMsQue.iUsedNums != 0 )
    {   
        VOS_MTX_LOCK(&pstRctor->stMgrMsQue.stUsedLock);
        pstEntry =VOS_DList_RemoveHead(&pstRctor->stMgrMsQue.stUsedList);
        VOS_MTX_UNLOCK(&pstRctor->stMgrMsQue.stUsedLock);
        pstRctor->stMgrMsQue.iUsedNums--;
        
        pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VRCT_MSQ_ENTRY_S, stNode);
        
        PEvent("[TKD:%02d EID:%02d]=>PipeFliterID=%d, MessageCode=%d, msg-ptr=%p, msg-size=%d!",
                pstRctor->stInfo.TaskID, 
                pstRctor->stInfo.Epollfd, 
                pstMsgNode->PipeFliterID, 
                pstMsgNode->MsgCode,
                pstMsgNode->pvMsgData, 
                pstMsgNode->MsgSize);
        
        if ( pstMsgNode->PipeFliterID < VRCT_MSQPF_NUMS
            && NULL != pstRctor->stMgrMsQue.apstMsgOpts[pstMsgNode->PipeFliterID] )
        {
            ((PFVRCT_MSGCTL_CB)pstRctor->stMgrMsQue.apstMsgOpts[pstMsgNode->PipeFliterID]->stMsgQueCB.pvcbFunc)(pstMsgNode->pvMsgData,
                                                                                                                pstMsgNode->MsgSize,
                                                                                                                pstRctor->stMgrMsQue.apstMsgOpts[pstMsgNode->PipeFliterID]->stMsgQueCB.pvData);
        }
        
        pstMsgNode->pvMsgData = NULL;
        pstMsgNode->MsgCode = 0;
        pstMsgNode->MsgSize = 0;
        /** 队列长度末尾添加, 头部获取即可，或采用无锁化，这里将队列*/
        VOS_MTX_LOCK(&pstRctor->stMgrMsQue.stIdleLock);
        VOS_DLIST_ADD_TAIL(&pstRctor->stMgrMsQue.stIdleList, &pstMsgNode->stNode);
        VOS_MTX_UNLOCK(&pstRctor->stMgrMsQue.stIdleLock);
        pstRctor->stMgrMsQue.iIdleNums++;
   }

   return;
}



/**
 * @brief init the message queue event manager
 * @param pvRctor [in] vos reactor
 * @param Maxsize [in] reactor queue max limited size
 */
INT32_T VRCT_MsgQueManagerInit(PVRCT_REACTOR_S          pstRctor, UINT32_T MaxSize)
{
    INT32_T             uiIndex         = 0; 
    PVRCT_MSQ_ENTRY_S   pstEntry        = NULL;
    PVRCT_NETEVT_OPT_S  pstNetOpts      = NULL;
    INT32_T             lnonBlockflag   = 1;
    INT32_T             lRet            = 0;
    INT32_T             Eventfd         = 0;
    
    if ( NULL == pstRctor )
    {
        return SYS_ERR;
    }
    
    if ( SYS_ERR == VOS_MTX_INIT(&pstRctor->stMgrMsQue.stUsedLock) )
    {
        PError("[TKD:%02d EID:%02d]=>pthread-mutex-lock init error!", pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return SYS_ERR; 
    }
    
    if ( SYS_ERR == VOS_MTX_INIT(&pstRctor->stMgrMsQue.stIdleLock) )
    {
        PError("[TKD:%02d EID:%02d]=>pthread-mutex-lock init error!", pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stUsedLock);
        return SYS_ERR; 
    }

    VOS_DLIST_INIT(&pstRctor->stMgrMsQue.stIdleList);
    pstRctor->stMgrMsQue.iIdleNums = 0;
    VOS_DLIST_INIT(&pstRctor->stMgrMsQue.stUsedList);
    pstRctor->stMgrMsQue.iUsedNums = 0;
    
    Eventfd = eventfd(0, EFD_CLOEXEC);
    if ( Eventfd <= 0 )
    {
        PError("[TKD:%02d EID:%02d]=>Event Fd create failed,errno=%d:%s", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, errno, strerror(errno));
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stIdleLock);
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stUsedLock);
        return SYS_ERR; 
    }

    lRet = ioctl(Eventfd, FIONBIO, &lnonBlockflag);
    if ( 0 != lRet )
    {
        PError("[ETSK] [TKD:%02d EID:%02d]=>Event Fd ctrl nonblock errno=%d:%s", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, errno, strerror(errno));
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stIdleLock);
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stUsedLock);
        close(Eventfd);
        return SYS_ERR;
    }
    
    VRCT_NETCALLBACK_INIT( &pstRctor->stMgrMsQue.stMsqNetOpt,
                             Eventfd,
                             VRCT_IOTYPE_MSG,
                             VRCT_POLL_LTIN,
                             VRCT_MsgQueMainCb,
                             NULL,
                             pstRctor);
    
    if (VOS_ERR == VRCT_NetworkEvtOptsRegister(pstRctor,  &pstRctor->stMgrMsQue.stMsqNetOpt) )
    {
        PError("[TKD:%02d EID:%02d]=>register options error, Eventfd=%d",
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd,
                    Eventfd);
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stIdleLock);
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stUsedLock);
        close(Eventfd);
        return VOS_ERR;
    }
    
    pstRctor->stMgrMsQue.pstRctor = pstRctor;
    pstRctor->stMgrMsQue.MaxSize = MaxSize;
    
    /*开始新建消息节点, 预留并发100用于消息溢出*/
    for ( uiIndex=0; uiIndex < MaxSize + 100; uiIndex++ )
    {
        pstEntry = (PVRCT_MSQ_ENTRY_S)malloc(sizeof(VRCT_MSQ_ENTRY_S));
        if ( NULL == pstEntry )
        {   
            PError("[TKD:%02d EID:%02d]=>malloc failed!", 
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd);
            VRCT_MsgQueManagerUnInit(pstRctor);
            return SYS_ERR;
        }
        memset(pstEntry,0,sizeof(VRCT_MSQ_ENTRY_S));
        VOS_DLIST_INIT(&pstEntry->stNode);
        VOS_DLIST_ADD_TAIL(&pstRctor->stMgrMsQue.stIdleList, &pstEntry->stNode);
        pstRctor->stMgrMsQue.iIdleNums++;
    }

    for ( uiIndex =0; uiIndex < VRCT_MSQPF_NUMS-1; uiIndex++ )
    {
        pstRctor->stMgrMsQue.apstMsgOpts[uiIndex] = NULL;
    }

    PEvent("[TKD:%02d EID:%02d]=>Message Manager Init success, Eventfd=[%d]!", 
        pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, Eventfd);
    
    return VOS_OK;
}



/**
 * @brief un-init the message queue event manager
 * @param pvRctor [in] vos reactor
 */
VOID VRCT_MsgQueManagerUnInit(PVRCT_REACTOR_S          pstRctor)
{
    PVOS_DLIST_S            pthisEntry  = NULL, pNextEntry=NULL, plistHead=NULL;
    PVRCT_MSQ_ENTRY_S       pstMsgNode  = NULL;
    
    if ( NULL != pstRctor )
    {
        VRCT_NetworkEvtOptsUnRegister(pstRctor, &pstRctor->stMgrMsQue.stMsqNetOpt);
        
        if ( VOS_TRUE != VOS_DList_IsEmpty(&pstRctor->stMgrMsQue.stIdleList))
        {
            plistHead = &pstRctor->stMgrMsQue.stIdleList;
            for (pthisEntry = plistHead->next;
                pthisEntry != plistHead;
                pthisEntry = pNextEntry)
            {
                pNextEntry = pthisEntry->next;
                pstMsgNode = VOS_DLIST_ENTRY(pthisEntry, VRCT_MSQ_ENTRY_S, stNode);
                VOS_DLIST_DEL(&pstMsgNode->stNode);
                free(pstMsgNode);
                pthisEntry = plistHead->next;
            }
        }
        
        if ( VOS_TRUE != VOS_DList_IsEmpty(&pstRctor->stMgrMsQue.stUsedList))
        {
            plistHead = &pstRctor->stMgrMsQue.stUsedList;
            for (pthisEntry = plistHead->next;
                pthisEntry != plistHead;
                pthisEntry = pNextEntry)
            {
                pNextEntry = pthisEntry->next;
                pstMsgNode = VOS_DLIST_ENTRY(pthisEntry, VRCT_MSQ_ENTRY_S, stNode);
                VOS_DLIST_DEL(&pstMsgNode->stNode);
                free(pstMsgNode);
                pthisEntry = plistHead->next;
            }
        }
        
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stIdleLock);
        VOS_MTX_FINI(&pstRctor->stMgrMsQue.stUsedLock);
        close(pstRctor->stMgrMsQue.stMsqNetOpt.fd);
    }

}





