/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_api.c
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : 对外提供的API使用接口
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_api.c
 * @brief vos reactor application interface
 * @author jimk 
 * @date 2019-07
 */
#include <vos/vos_pub.h>
#include <vrct/vrct_priv.h>


/** 默认的队列大小*/
#define  VRCT_MSQSIZE       1000


/**
 * @brief register the message queue optiion 
 * @param pvRctor [in] vos reactor 
  * @param pstMsgOpts [in] message queue option
 */
INT32_T VRCT_API_MsqOptRegister(PVOID pvRctor, PVRCT_MSQ_OPT_S pstMsqOpts)
{
    PVRCT_REACTOR_S     pstRctor     = (PVRCT_REACTOR_S)pvRctor;
    if ( NULL == pstMsqOpts 
        || NULL == pvRctor )
    {
        PError("Param error!");
        return VOS_ERR;
    }
    
    if ( VOS_ERR == VRCT_MsgQueOptsRegister(pstRctor, pstMsqOpts) )
    {
        PError("Vos reactor register the message queue option error!");
        return VOS_ERR;
    }

    return VOS_OK;
}

/**
 * @brief register the message queue optiion 
 * @param pvRctor [in] vos reactor 
 * @param pstMsgOpts [in] message queue option
 */
VOID    VRCT_API_MsqOptUnRegister(PVOID pvRctor, PVRCT_MSQ_OPT_S pstMsqOpts)
{
    PVRCT_REACTOR_S     pstRctor     = (PVRCT_REACTOR_S)pvRctor;
    
    if ( NULL == pstMsqOpts 
        || NULL == pvRctor )
    {
        return;
    }    

    VRCT_MsgQueOptsUnRegister(pstRctor, pstMsqOpts);
}

/**
 * @brief register the message queue optiion 
 * @param pvRctor [in] vos reactor 
 * @param PipeFilterID [in] get the value on the register option
 * @param pcData [in] message data
 * @param iDataLen [in] message data length
 */
INT32_T VRCT_API_MsqOptPush(PVOID pvRctor, UINT32_T PipeFilterID, UINT32_T Value, CHAR *pcData, UINT32_T DataLen)
{
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvRctor;
    PVRCT_MSQ_ENTRY_S       pstMsgNode  = NULL;
    INT32_T                 Val         = 1;
    
    if ( NULL == pstRctor
        || PipeFilterID >= VRCT_MSQPF_NUMS )
    {
        PError("Param error!");
        return VOS_ERR;
    }
    
    #if 0
    if ( 0 == pstRctor->stMgrMsQue.iIdleNums )
    {
        PError("[TKD:%02d EID:%02d]=>Message queue idle node has zero!",
                pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return VOS_ERR;
    }
    
    VOS_ASSERT(PipeFilterID == 0);
    
    PVOS_DLIST_S            pstEntry    = NULL;
    VOS_MTX_LOCK(&pstRctor->stMgrMsQue.stIdleLock);
    pstEntry=VOS_DList_RemoveHead(&pstRctor->stMgrMsQue.stIdleList);
    VOS_MTX_UNLOCK(&pstRctor->stMgrMsQue.stIdleLock);
    pstRctor->stMgrMsQue.iIdleNums--;
    pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VRCT_MSQ_ENTRY_S, stNode);
    #endif
    
    pstMsgNode = (PVRCT_MSQ_ENTRY_S)malloc(sizeof(VRCT_MSQ_ENTRY_S));
    VOS_DLIST_INIT(&pstMsgNode->stNode);
    
    pstMsgNode->MsgCode   = VRCT_MSQCODE_USER;
    pstMsgNode->PipeFliterID =PipeFilterID;
    pstMsgNode->Value     = Value;
    pstMsgNode->pvMsgData = pcData;
    pstMsgNode->MsgSize   = DataLen;
    
    VOS_MTX_LOCK(&pstRctor->stMgrMsQue.stUsedLock);
    VOS_DLIST_ADD_TAIL(&pstRctor->stMgrMsQue.stUsedList, &pstMsgNode->stNode);
    pstRctor->stMgrMsQue.iUsedNums++;
    VOS_MTX_UNLOCK(&pstRctor->stMgrMsQue.stUsedLock);

    //测试时:发现Used Nums=1的时候,从3s下降到6s,但是每个包的实时性就保证了
    //单核另外考虑
    //if( (pstRctor->stMgrMsQue.iUsedNums & 0x1F) == 0x1F)
    //if( (pstRctor->stMgrMsQue.iUsedNums & 0xFF)  > 0 )
    {
        /*通过eventfd告知*/
        if ( 0 > eventfd_write(pstRctor->stMgrMsQue.Eventfd, Val) )
        {
            PError("[TKD:%02d EID:%02d]=>Event write error,errno=%d:%s",
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd,
                    errno,
                    strerror(errno));
            return VOS_ERR_QUEFULL;
        }
        else
        {
            //printf("Event write success!g_test_cout=%lu\n", g_test_cout);
            PDebug("[TKD:%02d EID:%02d]=>Event write success, EventFd=%d,Value=%d",
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd, 
                    pstRctor->stMgrMsQue.Eventfd,
                    Value);
        }
    }
    
    return VOS_OK;
}

/**
 * @brief register the network optiion 
 * @param pvRctor [in] vos reactor 
 * @param pstNetOpts [in] network option
 */
INT32_T VRCT_API_NetworkOptRegister(PVOID pvRctor, PVRCT_NETEVT_OPT_S pstNetOpts)
{
    PVRCT_REACTOR_S     pstRctor     = (PVRCT_REACTOR_S)pvRctor;
    
    if ( NULL == pvRctor )
    {
        PError("Param error!");
        return VOS_ERR;
    }
    pstNetOpts->IoType = VRCT_IOTYPE_NET;
    
    if ( VOS_ERR == VRCT_NetworkEvtOptsRegister(pstRctor, pstNetOpts) )
    {
        PError("Vos reactor register the network option error!");
        return VOS_ERR;
    }
    
    return VOS_OK;
}

/**
 * @brief register the network event optiion 
 * @param pvRctor [in] vos reactor 
 * @param pstNetOpts [in] network option
 */
VOID    VRCT_API_NetworkOptUnRegister(PVOID pvRctor, PVRCT_NETEVT_OPT_S pstNetOpts)
{
    PVRCT_REACTOR_S     pstRctor     = (PVRCT_REACTOR_S)pvRctor;
    
    if ( NULL == pstNetOpts 
        || NULL == pvRctor )
    {
        return;
    }
    
    VRCT_NetworkEvtOptsUnRegister(pstRctor, pstNetOpts);
}

/**
 * @brief ctrl the netowrk ctrl event option 
 * @param EpollFd [in] vos reactor epoll fd 
 * @param fd [in] fd
 * @param OptCtrl [in] OptCtrl
 */
INT32_T   VRCT_API_NetworkOptCtrl(PVOID pvRctor,  INT32_T fd, INT32_T EvtMask)
{
    struct epoll_event      stEvent     = {0};
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvRctor;
    
    if ( NULL == pvRctor
        || 0 >= fd
        || fd >= VRCT_FDMAX-1 )
    {
        PError("Param error! fd=%d", fd);
        return SYS_ERR_PARAM;
    }
    
    stEvent.data.fd     = fd;
    stEvent.events      = EvtMask;
    
    if ( 0 > epoll_ctl(pstRctor->stInfo.Epollfd, EPOLL_CTL_MOD, fd, &stEvent))
    {
        PError("[EID:%02d]=>EPoll-Ctrl:(MOD) error! SockFd=%d, errno=%d:%s",  
                    pstRctor->stInfo.Epollfd, fd, errno, strerror(errno));
        return SYS_ERR;
    }
    
    return SYS_OK;
}

/**
 * @brief register the timer event optiion 
 * @param pvRctor [in] vos reactor 
 * @param pstTimerOpts [in] timer option
 */
INT32_T VRCT_API_TimerOptRegister(PVOID pvRctor, PVRCT_TIMER_OPT_S pstTimerOpts)
{
    PVRCT_REACTOR_S     pstRctor     = (PVRCT_REACTOR_S)pvRctor;

    if ( NULL == pstTimerOpts 
        || NULL == pvRctor )
    {
        PError("Param error!");
        return VOS_ERR;
    }

    if ( VOS_ERR == VRCT_TimerEvtOptsRegister(pstRctor, pstTimerOpts) )
    {
        PError("Vos reactor register the timer option error!");
        return VOS_ERR;
    }

    return VOS_OK;
}


/**
 * @brief register the timer event optiion 
 * @param pvRctor [in] vos reactor 
 * @param pstTimerOpts [in] timer option
 */
VOID    VRCT_API_TImerOptUnRegister(PVOID pvRctor, PVRCT_TIMER_OPT_S pstTimerOpts)
{
    PVRCT_REACTOR_S     pstRctor     = (PVRCT_REACTOR_S)pvRctor;
    
    if ( NULL == pstTimerOpts 
        || NULL == pvRctor )
    {
        return;
    }    

    VRCT_TimerEvtOptsUnRegister(pstRctor, pstTimerOpts);
}

/**
 * @brief create a new pthread vos reactor task
 * @param TaskID [in] task ID
 * @param MaxSize [in] the message queue max size
 */
VOID    *VRCT_API_Create(INT32_T TaskID, UINT32_T MaxSize)
{
    PVRCT_REACTOR_S     pstRctor     = NULL;
    PVOID               pvRctor      = NULL;
    
    pstRctor = (PVRCT_REACTOR_S)malloc(sizeof(VRCT_REACTOR_S));
    if (NULL == pstRctor)
    {
        PError("[malloc Failed!");
        return NULL;
    }
    
    memset(pstRctor, 0, sizeof(VRCT_REACTOR_S));
    
    VOS_GenUUid(pstRctor->stInfo.acGuid,  SYS_GUID_LEN);
    pstRctor->stInfo.TaskID     = TaskID;
    pstRctor->stInfo.Stop       = VOS_FALSE;
    pstRctor->stInfo.Epollfd    = epoll_create(VRCT_FDMAX);
    if( 0 >= pstRctor->stInfo.Epollfd )
    {
        PError("[TKD:%02d EID:%02d]=>epoll create failed,errno=%d:%s", 
                pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, 
                errno, strerror(errno));
        goto ErrorExit; 
    }
    
    if( SYS_ERR == VOS_ThreadEvent_Init(&pstRctor->hWaitForStart) )
    {   
        PError("[TKD:%02d EID:%02d]=>Thread event init failed", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        goto ErrorExit;   
    }
    
    if( SYS_ERR == VOS_ThreadEvent_Init(&pstRctor->hWaitForExit) )
    {   
        PError("[TKD:%02d EID:%02d]=>Thread event init failed", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        goto ErrorExit;
    }
    
    if ( SYS_ERR == VRCT_NetworkEvtManagerInit(pstRctor, VRCT_FDMAX) )
    {
        PError("[TKD:%02d EID:%02d]=>network fd manager init failed", 
                    pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return NULL; 
    }
    
    if ( SYS_ERR == VRCT_MsgQueManagerInit(pstRctor, MaxSize)  )
    {
        PError("[TKD:%02d EID:%02d]=>message fd manager init failed", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        goto ErrorExit; 
    }
    
    if ( VOS_ERR == VRCT_TimerCtrlManagerInit(pstRctor) )
    {
       PError("[TKD:%02d EID:%02d]=>timer manager init success!", 
                pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
       goto ErrorExit;
    }
    
    pstRctor->pstVRctor = pstRctor;
    
    pvRctor = (VOID *)pstRctor;
    
    PEvent("[TKD:%02d EID:%02d]=>vos reactor create success!", pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
    
    return pvRctor;
    
ErrorExit:
    VOS_ThreadEvent_Destroy(&pstRctor->hWaitForStart);
    VOS_ThreadEvent_Destroy(&pstRctor->hWaitForExit);
    
    VRCT_MsgQueManagerUnInit(pstRctor);
    VRCT_NetworkEvtManagerUnInit(pstRctor);
    
    if ( 0 <  pstRctor->stInfo.Epollfd  )
    {
        close(pstRctor->stInfo.Epollfd);
    }

    if ( NULL != pstRctor )
    {
        free(pstRctor);
    }
    
    return NULL;
}

/**
 * @brief release the pthread vos reactor task
 * @param pvRctor [inout] vos reactor
 */
VOID VRCT_API_Release(PVOID *ppvRctor)
{
    PVRCT_REACTOR_S*    ppstRctor= (PVRCT_REACTOR_S *)ppvRctor;
    PVRCT_REACTOR_S     pstRctor = NULL;    
    
    if ( NULL != ppvRctor )
    {
        pstRctor = (PVRCT_REACTOR_S)*ppstRctor;
        
        if ( NULL == pstRctor )
        {
            return;
        }
        
        pstRctor->stInfo.Stop = 1;
        
        VRCT_API_Stop(pstRctor);
        
        /*等待线程退出*/
        (void)VOS_ThreadEvent_Waitfor(&pstRctor->hWaitForExit, 100);
        
        VOS_ThreadEvent_Destroy(&pstRctor->hWaitForStart);
        VOS_ThreadEvent_Destroy(&pstRctor->hWaitForExit);
        VRCT_MsgQueManagerUnInit(pstRctor);
        VRCT_TimerCtrlManagerUnInit(pstRctor);
        VRCT_NetworkEvtManagerUnInit(pstRctor);
        
        if ( 0 <  pstRctor->stInfo.Epollfd  )
        {
            close(pstRctor->stInfo.Epollfd);
        }
        
        if ( NULL != pstRctor )
        {
            free(pstRctor);
        }
        
        *ppstRctor = NULL;
        
        PEvent("vos reactor release success!");
    }
}


/**
 * @brief start the vos reactor task
 * @param TaskID [in] task ID
 * @param MaxSize [in] the message queue max size
 */
INT32_T VRCT_API_Start(PVOID      pvRctor)
{
    PVRCT_REACTOR_S     pstRctor = (PVRCT_REACTOR_S)pvRctor;
    int                 iRet     = 0;
    pthread_t           handle   = 0;
    
    if ( NULL == pvRctor )
    {
        return VOS_ERR;
    }
    
#if 0
    pthread_attr_t      attr;

    iRet = pthread_attr_init(&attr);
    if (iRet) 
    {
        PError("[TKD:%02d EID:%02d]=>pthread_attr_init() failed", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return VOS_ERR;
    }
    
    /** 会导致线程分离*/
    iRet = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (iRet) 
    {
        PError("[TKD:%02d EID:%02d]=>pthread_attr_setdetachstate() failed", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return VOS_ERR;
    }
    iRet = pthread_create(&handle, &attr, VRCT_MainWorkerCb, (VOID *)pstRctor);
#else
    iRet = pthread_create(&handle, NULL, VRCT_MainWorkerCb, (VOID *)pstRctor);
#endif
    if ( 0 != iRet )
    {
        PError("[TKD:%02d EID:%02d]=>epoll pthread create failed,errno=%d:%s",
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, errno, strerror(errno));
        return VOS_ERR;
    }
    
    iRet = VOS_ThreadEvent_Waitfor(&pstRctor->hWaitForStart, 3000);
    if ( VOS_OK != iRet )
    {
        PError("[TKD:%02d EID:%02d]=>wait for start event exit!iRet=%08x", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, iRet);
        return VOS_ERR;
    }
    
    //(void) pthread_attr_destroy(&attr);
    
    PEvent("[TKD:%02d EID:%02d]=>epoll pthread create successful!",
                pstRctor->stInfo.TaskID, 
                pstRctor->stInfo.Epollfd);
    
    return VOS_OK;
}



/**
 * @brief notify the pthread task exit
 * @param pstRctor [inout] Vos reactor
 */
VOID VRCT_API_Stop(PVOID pvRctor)
{
    PVRCT_REACTOR_S         pstRctor    = (PVRCT_REACTOR_S)pvRctor;
    PVRCT_MSQ_ENTRY_S       pstMsgNode  = NULL;
    PVOS_DLIST_S            pstEntry    = NULL;
    int                     uiVal       = 1;
    int                     iRet        = 0;
    
    if ( NULL == pstRctor )
    {
        return;
    }
    
    if ( 0 == pstRctor->stMgrMsQue.iIdleNums )
    {
        return;
    }
    
    VOS_MTX_LOCK(&pstRctor->stMgrMsQue.stIdleLock);
    pstEntry=VOS_DList_RemoveHead(&pstRctor->stMgrMsQue.stIdleList);
    VOS_MTX_UNLOCK(&pstRctor->stMgrMsQue.stIdleLock);
    pstRctor->stMgrMsQue.iIdleNums--;
    
    pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VRCT_MSQ_ENTRY_S, stNode);
    
    pstMsgNode->MsgCode   = VRCT_MSQCODE_EXIT;
    
    VOS_MTX_LOCK(&pstRctor->stMgrMsQue.stUsedLock);
    VOS_DLIST_ADD_TAIL(&pstRctor->stMgrMsQue.stUsedList, &pstMsgNode->stNode);
    VOS_MTX_UNLOCK(&pstRctor->stMgrMsQue.stUsedLock);
    pstRctor->stMgrMsQue.iUsedNums++;
    
    /*通过eventfd告知*/
    iRet = eventfd_write(pstRctor->stMgrMsQue.Eventfd, uiVal);
    if ( iRet < 0 )
    {
        PError("[TKD:%02d EID:%02d]=>Event write error,errno=%d:%s",
                pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd,
                errno, strerror(errno));
    }
    
    return;
}





