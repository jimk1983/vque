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



/*网络事件处理器: 所有事件触发器的基础*/
struct tagVosReactorNetEvtManager
{
    PVRCT_REACTOR_S             pstVReactor;                /** 总触发器*/
    VRCT_NETEVT_OPT_S**         apstEpollEvtOps;            /** socketfd操作数组，空间换时间*/
};

/*单个节点需要包含的信息*/
struct tagVosReactorNetEvtOpts
{
    VOS_DLIST_S                 stNode;
    INT32_T                     fd;                         /** 网络事件的socket号*/
    INT32_T                     IoType;                     /** IO类型*/
    UINT32_T                    EventMask;                  /** 网络事件关注EPOLL_IN/ EPOLL_OUT*/
    VRCT_CALLBACK_S             stRecv;                     /** 接收的事件处理回调函数和节点指针*/
    VRCT_CALLBACK_S             stSend;                     /** 发送的事件处理回调函数和节点指针*/
};

typedef VOID (*vrct_netevt_recv_cb)(VOID *pvConn);
typedef VOID (*vrct_netevt_send_cb)(VOID *pvConn);

#define VRCT_NETCALLBACK_INIT(pstNetOpts_, fd_, iotype_, EventMask_, pfRecv_, pfSend_, pvConner_) do{\
    (pstNetOpts_)->fd = (fd_);\
    (pstNetOpts_)->IoType = (iotype_);\
    (pstNetOpts_)->EventMask = (EventMask_);\
    VRCT_CALLBACK_INIT(&(pstNetOpts_)->stRecv, pfRecv_, pvConner_);\
    VRCT_CALLBACK_INIT(&(pstNetOpts_)->stSend, pfSend_, pvConner_);\
}while(0);









#endif






