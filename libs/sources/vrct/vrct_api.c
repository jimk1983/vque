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
 * @brief notify the pthread task exit
 * @param pstRctor [inout] Vos reactor
 */
VOID VRCT_API_ExitNotify(PVRCT_REACTOR_S pstRctor)
{
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
    
    pstRctor->pstVRctor = pstRctor;
    
    pvRctor = (VOID *)pstRctor;
    
    PEvent("[TKD:%02d EID:%02d]=>Epoll create success!", pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
    
    return pvRctor;
    
ErrorExit:
    VOS_ThreadEvent_Destroy(&pstRctor->hWaitForStart);
    VOS_ThreadEvent_Destroy(&pstRctor->hWaitForStart);
    
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


INT32_T VRCT_API_Start(VOID*     pvRctor)
{
    PVRCT_REACTOR_S     pstRctor = (PVRCT_REACTOR_S)pvRctor;
    int                 iRet     = 0;
    pthread_t           handle   = 0;
    pthread_attr_t      attr;
    
    if ( NULL == pvRctor )
    {
        return VOS_ERR;
    }
    
    iRet = pthread_attr_init(&attr);
    if (iRet) 
    {
        PError("[TKD:%02d EID:%02d]=>pthread_attr_init() failed", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return VOS_ERR;
    }

    iRet = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (iRet) 
    {
        PError("[ETSK] [TKD:%02d EID:%02d]=>pthread_attr_setdetachstate() failed", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return VOS_ERR;
    }
    
    iRet = pthread_create(&handle, &attr, VRCT_MainWorkerCb, (VOID *)pstRctor);
    if ( 0 != iRet )
    {
        PError("[ETSK] [TKD:%02d EID:%02d]=>epoll pthread create failed,errno=%d:%s",
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return VOS_ERR;
    }
    
    (void) pthread_attr_destroy(&attr);
    
    iRet = VOS_ThreadEvent_Waitfor(&pstRctor->hWaitForStart, 3000);
    if ( VOS_OK != iRet)
    {
        PError("[ETSK] [TKD:%02d EID:%02d]=>wait for start event exit!", 
            pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
        return VOS_ERR;
    }
    
    return VOS_OK;
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
        
        VRCT_API_ExitNotify(pstRctor);
        
        /*等待线程退出*/
        (void)VOS_ThreadEvent_Waitfor(&pstRctor->hWaitForExit, 100);
        #if VOS_PLAT_LINUX
        pthread_exit(0);
        #endif
        
        VOS_ThreadEvent_Destroy(&pstRctor->hWaitForStart);
        VOS_ThreadEvent_Destroy(&pstRctor->hWaitForExit);
        VRCT_MsgQueManagerUnInit(pstRctor);
        VRCT_NetworkEvtManagerUnInit(pstRctor);
        VRCT_TimerCtrlManagerUnInit(pstRctor);
        
        if ( 0 <  pstRctor->stInfo.Epollfd  )
        {
            close(pstRctor->stInfo.Epollfd);
        }

        if ( NULL != pstRctor )
        {
            free(pstRctor);
        }
        
        *ppstRctor = NULL;
    }
}



