/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_macro.h
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : vos的queue宏定义
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_macro.c
 * @brief vos reactor macro define
 * @author jimk 
 * @date 2019-07
 */
#ifndef _VRCT_MACRO_H_
#define _VRCT_MACRO_H_
     

/*最大的FD数*/
#define VRCT_FDMAX                          102400
/*最大的EVENT数*/
#define VRCT_EVTMAX                         10240
/*最大定时器数*/
#define VRCT_FDTIMEMAX                      VRCT_FDMAX

/*系统最大的队列线程数*/
#define VRCT_SYS_MAX                        1024
/*流控低水位*/
#define VRCT_BUFLO_LEVEL                    10
/*流控高水位*/
#define VRCT_BUFHI_LEVEL                    1000


typedef struct tagVosReactor                    VRCT_REACTOR_S, *PVRCT_REACTOR_S;
typedef struct tagVosReactorNetEvtManager       VRCT_NETEVT_MGR_S,*PVRCT_NETEVT_MGR_S;
typedef struct tagVosReactorTimerManager        VRCT_TIMER_MGR_S,*PVRCT_TIMER_MGR_S;
typedef struct tagVosReactorMessageQueueManager VRCT_MSQUE_MGR_S,*PVRCT_MSQUE_MGR_S;


//typedef struct tagVosReactorExpireManager       VRCT_EXPIRE_MGR_S,*PVRCT_EXPIRE_MGR_S;
//typedef struct tagVosReactorExpireOpts          VRCT_EXPIRE_OPT_S,*PVRCT_EXPIRE_OPT_S;

#endif






