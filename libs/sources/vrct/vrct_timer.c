/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_timer.c
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : vos reactor的定时器
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_api.c
 * @brief vos reactor timer manager
 * @author jimk 
 * @date 2019-07
 */


#include <vos/vos_pub.h>
#include <vrct/vrct_priv.h>

#define     VRCT_SLOW_TIMES                      10

/**
 * @brief Timer register the options
 * @param pvRctor [in] vos reactor
 * @param pstTimeOps [in] timer options
 */
CINLINE INT32_T VRCT_TimerEvtOptsRegister(PVRCT_REACTOR_S pstRctor, PVRCT_TIMER_OPT_S pstTimeOps)
{
    
    if ( pstTimeOps->TimeOut > VRCT_SLOW_TIMES  )
    {
        VOS_DLIST_ADD_TAIL(&pstRctor->stMgrTimer.stSlowList, &pstTimeOps->stNode);
    }
    else
    {
        VOS_DLIST_ADD_TAIL(&pstRctor->stMgrTimer.stQuickList, &pstTimeOps->stNode);
    }
    
    return VOS_OK;
}

/**
 * @brief un-register the Timer
 * @param pvRctor [in] vos reactor
 * @param pstTimeOps [in] timer options
 */
CINLINE VOID VRCT_TimerEvtOptsUnRegister(PVRCT_TIMER_OPT_S pstTimeOps)
{
    VOS_DLIST_DEL(&pstTimeOps->stNode);
    
    return;
}

/**
 * @brief Timer Handler CallBack
 * @param pvRctor [in] vos reactor
 */
VOID VRCT_TimerCtrlMainCB(INT32_T fd, PVOID          pvRctor)
{
    PVRCT_REACTOR_S     pstRctor         = (PVRCT_REACTOR_S)pvRctor;
    PVOS_DLIST_S        pstNodeNext     = NULL;
    PVRCT_TIMER_OPT_S   pstTimeNodeTmp  = NULL;
    PVRCT_TIMER_OPT_S   pstTimeNodeNext = NULL;
    
    if ( NULL == pvRctor )
    {
        PError("Param Error");
        return;
    }
    
    PEvent("[TKD:%02d EID:%02d]=>Main Timer Heap Startup!",   pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd);
    
    /*快定时器堆*/
    if ( fd == pstRctor->stMgrTimer.stQuickOpts.fd 
        && VOS_TRUE != VOS_DList_IsEmpty(&pstRctor->stMgrTimer.stQuickList) )
    {
        VOS_DLIST_FOR_EACH_ENTRY(pstTimeNodeTmp, &pstRctor->stMgrTimer.stQuickList, VRCT_TIMER_OPT_S, stNode)
        {
            pstTimeNodeTmp->TimeStamp++;
            
            if ( pstTimeNodeTmp->TimeStamp < pstTimeNodeTmp->TimeOut )
            {
                continue;
            }
            
            ((PFVRCT_COMM_CB)pstTimeNodeTmp->stTimercb.pvcbFunc)(pstTimeNodeTmp->stTimercb.pvData);
            if ( VRCT_TMTYPE_ONE == pstTimeNodeTmp->TimerType )
            {
                pstTimeNodeNext = VOS_DLIST_ENTRY(pstTimeNodeTmp->stNode.next, VRCT_TIMER_OPT_S, stNode);
                
                VOS_DLIST_DEL(&pstTimeNodeTmp->stNode);
                
                pstTimeNodeTmp = pstTimeNodeNext;
                continue;
            }
            
            pstTimeNodeTmp->TimeStamp = 0;
            
            if ( pstTimeNodeTmp->TimeOut > VRCT_SLOW_TIMES )
            {
                pstTimeNodeNext = VOS_DLIST_ENTRY(pstTimeNodeTmp->stNode.next, VRCT_TIMER_OPT_S, stNode);
                
                VOS_DLIST_DEL(&pstTimeNodeTmp->stNode);
                VOS_DLIST_ADD_TAIL(&pstRctor->stMgrTimer.stSlowList, &pstTimeNodeTmp->stNode);
                
                pstTimeNodeTmp = pstTimeNodeNext;
            }
        }
    }
    
    /*慢定时器堆*/
    if ( fd == pstRctor->stMgrTimer.stSlowOpts.fd
        && VOS_TRUE != VOS_DList_IsEmpty(&pstRctor->stMgrTimer.stSlowList) )
    {
        VOS_DLIST_FOR_EACH_ENTRY(pstTimeNodeTmp, &pstRctor->stMgrTimer.stSlowList, VRCT_TIMER_OPT_S, stNode)
        {
            pstTimeNodeTmp->TimeStamp++;
            
            if ( pstTimeNodeTmp->TimeOut - pstTimeNodeTmp->TimeStamp > VRCT_SLOW_TIMES )
            {
                continue;
            }
            
            pstTimeNodeNext = VOS_DLIST_ENTRY(pstTimeNodeTmp->stNode.next, VRCT_TIMER_OPT_S, stNode);
            
            VOS_DLIST_DEL(&pstTimeNodeTmp->stNode);
            VOS_DLIST_ADD_TAIL(&pstRctor->stMgrTimer.stQuickList, &pstTimeNodeTmp->stNode);
            
            pstTimeNodeTmp = pstTimeNodeNext;
        }
    }
    
    return;
}

/**
 * @brief init the timer manager
 * @param pvRctor [in] vos reactor
 */
INT32_T   VRCT_TimerCtrlManagerInit(PVRCT_REACTOR_S          pstRctor)
{
    INT32_T             ret             = 0;
    INT32_T             timerqickfd     = 0;
    INT32_T             timerslowfd     = 0;
    struct itimerspec   new_value       = {0};
    struct itimerspec   old_value       = {0};
    
    new_value.it_value.tv_sec   = 1;
    new_value.it_value.tv_nsec  = 0;
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;
    
    timerqickfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if ( timerqickfd < 0 )
    {
        PError("[TKD:%02d EID:%02d]=>timerfd create error, errno=%d:%s",
                pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, errno, strerror(errno));
        return VOS_ERR;
    }

    ret = timerfd_settime(timerqickfd, 0, &new_value, &old_value);
    if (ret < 0) 
    {
        PError("[TKD:%02d EID:%02d]=>timerfd create error, errno=%d:%s",
                pstRctor->stInfo.TaskID, pstRctor->stInfo.Epollfd, errno, strerror(errno));
        close(timerqickfd);
        return VOS_ERR;
    }
    
    pstRctor->stMgrTimer.pstVReactor            = pstRctor;
    VOS_DLIST_INIT(&pstRctor->stMgrTimer.stQuickList);
    
    VRCT_NETCALLBACK_INIT( &pstRctor->stMgrTimer.stQuickOpts,
                             timerqickfd,
                             VRCT_IOTYPE_TIMER,
                             VRCT_POLL_LTIN,
                             VRCT_TimerCtrlMainCB,
                             NULL,
                             pstRctor);
    
    if (VOS_ERR == VRCT_NetworkEvtOptsRegister(pstRctor,  &pstRctor->stMgrTimer.stQuickOpts) )
    {
        PError("[TKD:%02d EID:%02d]=>register options error, timerfd=%d",
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd,
                    timerqickfd);
        close(timerqickfd);
        return VOS_ERR;
    }
    
    new_value.it_value.tv_sec       = 1;
    new_value.it_value.tv_nsec      = 0;
    new_value.it_interval.tv_sec    = VRCT_SLOW_TIMES;
    new_value.it_interval.tv_nsec   = 0;
    
    timerslowfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if ( timerslowfd < 0 )
    {
        PError("[TKD:%02d EID:%02d]=>timerfd create error, errno=%d:%s",
                pstRctor->stInfo.TaskID, 
                pstRctor->stInfo.Epollfd, 
                errno, strerror(errno));
        VRCT_NetworkEvtOptsUnRegister(pstRctor, &pstRctor->stMgrTimer.stQuickOpts);
        close(timerqickfd);
        return VOS_ERR;
    }

    ret = timerfd_settime(timerslowfd, 0, &new_value, &old_value);
    if (ret < 0) 
    {
        PError("[TKD:%02d EID:%02d]=>timerfd create error, errno=%d:%s",
                pstRctor->stInfo.TaskID, 
                pstRctor->stInfo.Epollfd, 
                errno, strerror(errno));
        VRCT_NetworkEvtOptsUnRegister(pstRctor, &pstRctor->stMgrTimer.stQuickOpts);
        close(timerqickfd);
        close(timerslowfd);
        return VOS_ERR;
    }
    
    VRCT_NETCALLBACK_INIT( &pstRctor->stMgrTimer.stSlowOpts,
                             timerslowfd,
                             VRCT_IOTYPE_TIMER,
                             VRCT_POLL_LTIN,
                             VRCT_TimerCtrlMainCB,
                             NULL,
                             pstRctor);
    
    VOS_DLIST_INIT(&pstRctor->stMgrTimer.stSlowList);
    
    if (VOS_ERR == VRCT_NetworkEvtOptsRegister(pstRctor, &pstRctor->stMgrTimer.stSlowOpts) )
    {
        PError("[TKD:%02d EID:%02d]=>register options error, timerfd=%d",
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd,
                    timerqickfd);
        VRCT_NetworkEvtOptsUnRegister(pstRctor, &pstRctor->stMgrTimer.stQuickOpts);
        close(timerqickfd);
        close(timerslowfd);
        return VOS_ERR;
    }
    
    VOS_DLIST_INIT(&pstRctor->stMgrTimer.stSlowList);
    
    VOS_SPIN_INIT(&pstRctor->stMgrTimer.stPerfLock);
    VOS_DLIST_INIT(&pstRctor->stMgrTimer.stPerfList);
    
    PEvent("[TKD:%02d EID:%02d]=>Timer Manager Init success! quickfd=%d, slowfd=%d",
                    pstRctor->stInfo.TaskID,
                    pstRctor->stInfo.Epollfd,
                    timerqickfd,
                    timerslowfd);
    
    return SYS_OK;
}


/**
 * @brief un-init the timer manager
 * @param pvRctor [in] vos reactor
 */
VOID VRCT_TimerCtrlManagerUnInit(PVRCT_REACTOR_S          pstRctor)
{
    PVOS_DLIST_S       pthisEntry=NULL, pNextEntry=NULL, plistHead=NULL;
    PVRCT_TIMER_OPT_S  pstTimerNode = NULL;
    
    if ( NULL != pstRctor )
    {
        VRCT_NetworkEvtOptsUnRegister(pstRctor, &pstRctor->stMgrTimer.stQuickOpts);
        VRCT_NetworkEvtOptsUnRegister(pstRctor, &pstRctor->stMgrTimer.stSlowOpts);

        plistHead = &pstRctor->stMgrTimer.stQuickList;
        if ( VOS_TRUE != VOS_DList_IsEmpty(plistHead))
        {
            
            for ( pthisEntry = plistHead->prev;
                  pthisEntry != plistHead;
                  pthisEntry = pNextEntry )
            {
                pNextEntry = pthisEntry->prev;
                pstTimerNode = VOS_DLIST_ENTRY(pthisEntry, VRCT_TIMER_OPT_S, stNode);
                VOS_DLIST_DEL(&pstTimerNode->stNode);
                pstTimerNode=NULL;
                pthisEntry = plistHead->prev;
            }
        }
        
        plistHead = &pstRctor->stMgrTimer.stSlowList;
        if ( VOS_TRUE != VOS_DList_IsEmpty(plistHead))
        {
            for ( pthisEntry = plistHead->prev;
                  pthisEntry != plistHead;
                  pthisEntry = pNextEntry )
            {
                pNextEntry = pthisEntry->prev;
                pstTimerNode = VOS_DLIST_ENTRY(pthisEntry, VRCT_TIMER_OPT_S, stNode);
                VOS_DLIST_DEL(&pstTimerNode->stNode);
                pstTimerNode=NULL;
                pthisEntry = plistHead->prev;
            }
        }
        
        close(pstRctor->stMgrTimer.stQuickOpts.fd);
        close(pstRctor->stMgrTimer.stSlowOpts.fd);
    }
}


