/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_timer.h
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
 * @vrct_timer.c
 * @brief vos reactor timer manager
 * @author jimk 
 * @date 2019-07
 */
#ifndef _VRCT_TIMER_H_
#define _VRCT_TIMER_H_

/*以网络为主，定时器按照秒级的粒度执行*/
#define     VRCT_TIMER_GRAIN            1

/*网络事件处理器: 所有事件触发器的基础*/
struct tagVosReactorTimerManager
{
    PVRCT_REACTOR_S             pstVReactor;                
    VRCT_NETEVT_OPT_S           stQuickOpts;                 /** socketfd操作数组，空间换时间*/
    VOS_DLIST_S                 stQuickList;                /*快定时器堆*/
    
    VRCT_NETEVT_OPT_S           stSlowOpts;                  /** socketfd操作数组，空间换时间*/
    VOS_DLIST_S                 stSlowList;                 /*慢定时器堆*/
    
    VOS_SPINLOCK_T              stPerfLock;                 /*执行线程锁:用于定时器执行任务时间过长*/
    VOS_DLIST_S                 stPerfList;                 /*执行定时器堆*/
};


INT32_T     VRCT_TimerCtrlManagerInit(PVRCT_REACTOR_S          pstRctor);
VOID        VRCT_TimerCtrlManagerUnInit(PVRCT_REACTOR_S          pstRctor);

INT32_T VRCT_TimerEvtOptsRegister(PVRCT_REACTOR_S pstRctor, PVRCT_TIMER_OPT_S pstTimeOps);
VOID    VRCT_TimerEvtOptsUnRegister(PVRCT_REACTOR_S pstRctor, PVRCT_TIMER_OPT_S pstTimeOps);





#endif
  









