/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vque_ctx.h
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : 上下文管理
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vque_api.c
 * @brief vos epoll queue context manager
 * @author jimk 
 * @date 2019-07
 */
#ifndef _VRCT_CTX_H_
#define _VRCT_CTX_H_

/** VRCT初始化信息*/
typedef struct tagVosEventReactorInit
{
    INT32_T                     TaskID;                     /** 所属ID*/
    CHAR                        acGuid[SYS_GUID_LEN];       /** 该Reactor唯一GUID*/
    INT32_T                     Epollfd;                    /** 该Reactor的EpollFD*/
    INT32_T                     LogLevel;                   /** 日志级别*/
    INT32_T                     Stop;                       /** 该Reactor停止*/
    PFVRCT_COMM_CB              pfInitCB;                   /** 线程初始化*/
    PVOID                       pvInitCtx;                  /** 线程初始化上下文*/
}VRCT_INIT_S, *PVRCT_INIT_S;


/** VOS的Reactor主结构*/
struct tagVosEventReactor
{
    PVRCT_REACTOR_S             pstVRctor;
    VRCT_INIT_S                 stInfo;                     /** 初始化信息*/
    VRCT_NETEVT_MGR_S           stMgrNet;                   /** 所有FD的管理器*/
    VRCT_TIMER_MGR_S            stMgrTimer;                 /** 定时器管理器*/
    VRCT_MSQUE_MGR_S            stMgrMsQue;                 /** 消息控制管理*/
    VOS_EVT_T                   hWaitForStart;              /** 初始化启动完成*/
    VOS_EVT_T                   hWaitForExit;               /** 本实例线程安全退出*/
};






#endif

