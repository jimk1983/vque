/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vrct_net.h
  �� �� ��   : ����
  ��    ��  : jimk
  ��������   : 2019��7��22��
  ����޸�   :
  ��������   : vos���������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

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

/*�����¼�������: �����¼��������Ļ���*/
struct tagVosReactorNetEvtManager
{
    PVRCT_REACTOR_S             pstVReactor;                
    VRCT_NETEVT_OPT_S**         apstEpollEvtOps;            /** socketfd�������飬�ռ任ʱ��*/
    UINT32_T                    MaxSize;                    /** ���Size*/
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






