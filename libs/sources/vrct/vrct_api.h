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
    INT32_T     MaxSize;            /** 包最大长度*/
    INT32_T     HeadOffset;         /** 头部大小偏移*/
}VRCT_PACK_INFO_S, *PVRCT_PACK_INFO_S;

typedef VOID    (*PFVRCT_COMM_CB)(VOID *pvCtx);
typedef VOID    (*PFVRCT_NETEVT_CB)(INT32_T fd, VOID *pvConn);


/** IO的工作模式*/
typedef enum
{
    VRCT_IOTYPE_NET = 0,            /** 网络IO*/
    VRCT_IOTYPE_MSG,                /** 消息IO*/
    VRCT_IOTYPE_TIMER,              /** 定时器IO*/
    VRCT_IOTYPE_SPAIR,              /** 消息对IO*/
    
    VRCT_IOTYPE_NUMS
}VRCT_IOTYPE_E;

#define VRCT_POLL_ETIN      (EPOLLIN  | EPOLLET)
#define VRCT_POLL_ETOUT     (EPOLLOUT | EPOLLET)
#define VRCT_POLL_ETINOUT   (EPOLLOUT | EPOLLIN | EPOLLET)

#define VRCT_POLL_LTIN      (EPOLLIN)
#define VRCT_POLL_LTOUT     (EPOLLOUT)
#define VRCT_POLL_LTINOUT   (EPOLLOUT | EPOLLIN)

#define VRCT_POLL_ADD       EPOLL_CTL_ADD
#define VRCT_POLL_MOD       EPOLL_CTL_MOD
#define VRCT_POLL_DEL       EPOLL_CTL_DEL


/** 定时器的类型*/
typedef enum
{
    VRCT_TMTYPE_ONE = 0,              /** 一次性定时器*/
    VRCT_TMTYPE_RECYLE,               /** 循环定时器*/
    
    VRCT_TMTYPE_NUMS
}VRCT_TMTYPE_E;



#endif

