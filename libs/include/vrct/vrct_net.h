/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_net.h
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : vos的网络机制
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_api.c
 * @brief vos reactor network manager
 * @author jimk 
 * @date 2019-07
 */
#ifndef _VRCT_NET_H_
#define _VRCT_NET_H_

#define     VRCT_TIMER_QUIK     0
#define     VRCT_TIMER_SLOW     1

/*网络事件处理器: 所有事件触发器的基础*/
struct tagVosReactorNetEvtManager
{
    PVRCT_REACTOR_S             pstVReactor;                
    VRCT_NETEVT_OPT_S**         apstEpollEvtOps;            /** socketfd操作数组，空间换时间*/
    UINT32_T                    MaxSize;                    /** 最大Size*/
};

#define VRCT_NETCALLBACK_INIT(pstNetOpts_, fd_, iotype_, EventMask_, pfRecv_, pfSend_, pvConner_) do{\
    VOS_DLIST_INIT(&(pstNetOpts_)->stNode);\
    (pstNetOpts_)->fd = (fd_);\
    (pstNetOpts_)->IoType = (iotype_);\
    (pstNetOpts_)->EventMask = (EventMask_);\
    VRCT_CALLBACK_INIT(&(pstNetOpts_)->stRecv, pfRecv_, pvConner_);\
    VRCT_CALLBACK_INIT(&(pstNetOpts_)->stSend, pfSend_, pvConner_);\
}while(0);




INT32_T     VRCT_NetworkEvtManagerInit(PVRCT_REACTOR_S           pstRctor, UINT32_T MaxSize);
VOID        VRCT_NetworkEvtManagerUnInit(PVRCT_REACTOR_S            pstRctor);
INT32_T     VRCT_NetworkEvtRegister(PVRCT_REACTOR_S pstRctor,
                                                    INT32_T fd,
                                                    INT32_T IoType,
                                                    INT32_T EvtMask,
                                                    PFVRCT_NETEVT_CB pfRecvCb, 
                                                    PFVRCT_NETEVT_CB pfSendCb, 
                                                    VOID* pvCtx);
VOID        VRCT_NetworkEvtUnRegister(PVRCT_REACTOR_S          pstRctor, INT32_T fd);
INT32_T     VRCT_NetworkEvtOptsRegister(     PVRCT_REACTOR_S pstRctor,     PVRCT_NETEVT_OPT_S pstNetOpt);
VOID        VRCT_NetworkEvtOptsUnRegister(PVRCT_REACTOR_S             pstRctor,PVRCT_NETEVT_OPT_S pstNetOpt);
INT32_T     VRCT_NetworkEvtCtrl(PVRCT_REACTOR_S         pstRctor,INT32_T fd, INT32_T OptMask);






#endif






