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
    VRCT_NETEVT_OPT_S           stQuickOps;                 /** socketfd操作数组，空间换时间*/
    VOS_DLIST_S                 stQuickList;                /*快定时器堆*/
    
    VRCT_NETEVT_OPT_S           stSlowOps;                  /** socketfd操作数组，空间换时间*/
    VOS_DLIST_S                 stSlowList;                 /*慢定时器堆*/
    
    VOS_SPINLOCK_T              stPerfLock;                 /*执行线程锁:用于定时器执行任务时间过长*/
    VOS_DLIST_S                 stPerfList;                 /*执行定时器堆*/
};

/*单个节点需要包含的信息*/
struct tagVosReactorTimerOpts
{
    VOS_DLIST_S                 stNode;
    INT32_T                     TimerType;                  /** 0: once单次, 1:recycle循环*/
    UINT32_T                    TimeOut;                    /** 超时时间*/
    UINT32_T                    TimeStamp;                  /** 时间戳*/
    VRCT_CALLBACK_S             stTimercb;                  /** 发送的事件处理回调函数和节点指针*/
};


#endif
  









