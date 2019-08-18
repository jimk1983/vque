/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_api.h
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : epoll对外提供的API使用接口
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_api.h
 * @brief vos reactor application interface
 * @author jimk 
 * @date 2019-07
 */

#ifndef _VRCT_API_H_
#define _VRCT_API_H_

/** 最大的包长度*/
#define     VRCT_PACK_MAX           65536

/** 协议框架头部设定: 包大小直接规定用INT32_T来表示*/
typedef struct tagVosReactorSetPacketInfo
{
    INT32_T     MaxSize;                                    /** 包最大长度*/
    INT32_T     HeadOffset;                                 /** 头部大小偏移*/
}VRCT_PACK_INFO_S, *PVRCT_PACK_INFO_S;


typedef VOID    (*PFVRCT_COMM_CB)(VOID *pvCtx);
typedef VOID    (*PFVRCT_NETEVT_CB)(INT32_T fd, VOID *pvConn);
typedef VOID    (*PFVRCT_MSGCTL_CB)(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx);

typedef struct tagVRctCommCallback
{
      VOID* pvcbFunc;
      VOID* pvData;
}VRCT_CALLBACK_S, *PVRCT_CALLBACK_S;

#define VRCT_CALLBACK_INIT(pstCallBack_, pfFunc_, pvUserData_)\
do{\
    (pstCallBack_)->pvcbFunc = (VOID *)pfFunc_;\
    (pstCallBack_)->pvData   = (VOID *)pvUserData_;\
}while(0);


/** IO的工作模式*/
typedef enum
{
    VRCT_IOTYPE_NET = 0,                                    /** 网络IO*/
    VRCT_IOTYPE_MSG,                                        /** 消息IO*/
    VRCT_IOTYPE_TIMER,                                      /** 定时器IO*/
    VRCT_IOTYPE_SPAIR,                                      /** 消息对IO*/
    
    VRCT_IOTYPE_NUMS
}VRCT_IOTYPE_E;

#define VRCT_POLL_ETIN          (EPOLLIN  | EPOLLET)
#define VRCT_POLL_ETOUT         (EPOLLOUT | EPOLLET)
#define VRCT_POLL_ETINOUT       (EPOLLOUT | EPOLLIN | EPOLLET)

#define VRCT_POLL_LTIN          (EPOLLIN)
#define VRCT_POLL_LTOUT         (EPOLLOUT)
#define VRCT_POLL_LTINOUT       (EPOLLOUT | EPOLLIN)

#define VRCT_POLL_ADD           EPOLL_CTL_ADD
#define VRCT_POLL_MOD           EPOLL_CTL_MOD
#define VRCT_POLL_DEL           EPOLL_CTL_DEL


#define VRCT_NETOPT_INIT(pstNetOpts_, fd_, EventMask_, pfRecv_, pfSend_, pvConner_) do{\
    VOS_DLIST_INIT(&(pstNetOpts_)->stNode);\
    (pstNetOpts_)->fd = (fd_);\
    (pstNetOpts_)->IoType = VRCT_IOTYPE_NET;\
    (pstNetOpts_)->EventMask = (EventMask_);\
    VRCT_CALLBACK_INIT(&(pstNetOpts_)->stRecv, pfRecv_, pvConner_);\
    VRCT_CALLBACK_INIT(&(pstNetOpts_)->stSend, pfSend_, pvConner_);\
}while(0);

typedef struct tagVosReactorNetEvtOption
{
    VOS_DLIST_S                 stNode;
    INT32_T                     fd;                         /** 网络事件的socket号*/
    INT32_T                     IoType;                     /** IO类型*/
    UINT32_T                    EventMask;                  /** 网络事件关注EPOLL_IN/ EPOLL_OUT*/
    VRCT_CALLBACK_S             stRecv;                     /** 接收的事件处理回调函数和节点指针*/
    VRCT_CALLBACK_S             stSend;                     /** 发送的事件处理回调函数和节点指针*/
}VRCT_NETEVT_OPT_S,*PVRCT_NETEVT_OPT_S;

/** 定时器的类型*/
typedef enum
{
    VRCT_TMTYPE_ONE = 0,                                    /** 一次性定时器*/
    VRCT_TMTYPE_RECYLE,                                     /** 循环定时器*/
    
    VRCT_TMTYPE_NUMS
}VRCT_TMTYPE_E;
    
#define VRCT_TIMEROPT_INIT(pstTimerOpts_, TimerType_, TimerOut_, pfTimer_, pvConner_) do{\
    VOS_DLIST_INIT(&(pstTimerOpts_)->stNode);\
    (pstTimerOpts_)->TimerType = (TimerType_);\
    (pstTimerOpts_)->TimeOut = (TimerOut_);\
    (pstTimerOpts_)->TimeStamp = 0;\
    VRCT_CALLBACK_INIT(&(pstTimerOpts_)->stTimercb, pfTimer_, pvConner_);\
}while(0);

typedef struct tagVosReactorTimerOption
{
    VOS_DLIST_S                 stNode;
    INT32_T                     TimerType;                  /** 0: once单次, 1:recycle循环*/
    UINT32_T                    TimeOut;                    /** 超时时间*/
    UINT32_T                    TimeStamp;                  /** 时间戳*/
    VRCT_CALLBACK_S             stTimercb;                  /** 发送的事件处理回调函数和节点指针*/
}VRCT_TIMER_OPT_S,*PVRCT_TIMER_OPT_S;


#define VRCT_MSQOPT_INIT(pstMsqOpts_, PipeFliterID_, pfMsqHandler_, pvConner_) do{\
    VOS_DLIST_INIT(&(pstMsqOpts_)->stNode);\
    (pstMsqOpts_)->PipeFliterID = (PipeFliterID_);\
    VRCT_CALLBACK_INIT(&(pstMsqOpts_)->stMsgQueCB, pfMsqHandler_, pvConner_);\
}while(0);

typedef struct tagVosReactorMessageQueueOption
{
    VOS_DLIST_S                 stNode;
    UINT32_T                    PipeFliterID;               /** 管道过滤器模块ID*/
    VRCT_CALLBACK_S             stMsgQueCB;                 /** 消息队列*/
}VRCT_MSQ_OPT_S,*PVRCT_MSQ_OPT_S;


VOID*       VRCT_API_Create(INT32_T TaskID, UINT32_T MaxSize);
VOID        VRCT_API_Release(PVOID *ppvRctor);
INT32_T     VRCT_API_Start(PVOID pvRctor);
VOID        VRCT_API_Stop(PVOID pvRctor);

INT32_T     VRCT_API_NetworkOptRegister(PVOID pvRctor, PVRCT_NETEVT_OPT_S pstNetOpts);
VOID        VRCT_API_NetworkOptUnRegister(PVOID pvRctor, PVRCT_NETEVT_OPT_S pstNetOpts);
INT32_T     VRCT_API_NetworkOptCtrl(PVOID pvRctor,  INT32_T fd, INT32_T EvtMask);

INT32_T     VRCT_API_TimerOptRegister(PVOID pvRctor, PVRCT_TIMER_OPT_S pstTimerOpts);
VOID        VRCT_API_TImerOptUnRegister(PVOID pvRctor, PVRCT_TIMER_OPT_S pstTimerOpts);

INT32_T     VRCT_API_MsqOptRegister(PVOID pvRctor, PVRCT_MSQ_OPT_S pstMsqOpts);
VOID        VRCT_API_MsqOptUnRegister(PVOID pvRctor, PVRCT_MSQ_OPT_S pstMsqOpts);
INT32_T     VRCT_API_MsqOptPush(PVOID pvRctor, UINT32_T PipeFilterID,UINT32_T Value, CHAR *pcData, UINT32_T DataLen);


#endif

