/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_msq.h
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
 * @vque_msq.c
 * @brief vos reactor message queue manager
 * @author jimk 
 * @date 2019-07
 */
  
#ifndef _VRCT_MSQ_H_
#define _VRCT_MSQ_H_

/** 管道过滤器最多支持64级*/
#define     VRCT_MSQPF_NUMS         64

typedef struct tagVosReactorSockPair
{
    INT32_T                     iLocalRxFd;
    INT32_T                     iLocalRxPort;
    struct sockaddr_in          stRxAddr;
    INT32_T                     iLocalTxFd;
    INT32_T                     iLocalTxPort;
}VRCT_SOCKPAIR_S, *PVRCT_SOCKPAIR_S;

/*消息父节点*/
typedef struct tagVosReactorMessageQueueEntry
{
    VOS_DLIST_S                 stNode;
    UINT32_T                    PipeFliterID;                   /** 管道过滤器索引ID*/
    UINT32_T                    MsgCode;                        /** 消息控制码*/
    UINT32_T                    Value;                          /** 简单值传输*/
    UINT32_T                    MsgSize;                        /** 数据传输大小*/
    VOID*                       pvMsgData;                      /** 用户数据保存, 当前采用指针形式*/
}VRCT_MSQ_ENTRY_S, *PVRCT_MSQ_ENTRY_S;


/** TODO:后续尝试修改为无锁队列*/
struct tagVosReactorMessageQueueManager
{
    PVRCT_REACTOR_S             pstRctor;
    UINT32_T                    MaxSize;                        /** 最大消息上限*/
    
    INT32_T                     Eventfd;                        /** 事件FD*/
    VRCT_NETEVT_OPT_S           stMsqNetOpt;                    /** 事件FD对应处理*/
    PVRCT_MSQ_OPT_S             apstMsgOpts[VRCT_MSQPF_NUMS];   /** 管道过滤器注册:0级为四层*/
    
    VOS_DLIST_S                 stIdleList;                     /** 空闲链表*/
    VINT32_T                    iIdleNums;                      /** 空闲链表数量*/
    VOS_MTX_T                   stIdleLock;                     /** 线程锁*/

    VOS_DLIST_S                 stUsedList;                     /** 已使用链表*/
    VINT32_T                    iUsedNums;                      /** 已使用链表数量*/
    VOS_MTX_T                   stUsedLock;                     /** 线程锁*/
    
    INT32_T                     SockPairEnable;
    VRCT_NETEVT_OPT_S           stPairNetOpt;               
    VRCT_SOCKPAIR_S             stSockPair;                 
};

typedef enum
{
    VRCT_MSQCODE_USER =0,
    VRCT_MSQCODE_EXIT,
    
    VRCT_MSGCODE_NUMS
}VRCT_MSGCODE_E;

INT32_T     VRCT_MsgQueOptsRegister(PVRCT_REACTOR_S         pstRctor, PVRCT_MSQ_OPT_S pstMsqOpt);
VOID        VRCT_MsgQueOptsUnRegister(PVRCT_REACTOR_S         pstRctor, PVRCT_MSQ_OPT_S pstMsqOpt);

INT32_T     VRCT_MsgQueManagerInit(PVRCT_REACTOR_S          pstRctor, UINT32_T MaxSize);
VOID        VRCT_MsgQueManagerUnInit(PVRCT_REACTOR_S          pstRctor);
      
#endif
 









