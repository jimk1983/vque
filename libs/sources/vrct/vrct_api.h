/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vrct_api.h
  �� �� ��   : ����
  ��    ��  : jimk
  ��������   : 2019��7��22��
  ����޸�   :
  ��������   : epoll�����ṩ��APIʹ�ýӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/**
 * @vrct_api.h
 * @brief vos reactor application interface
 * @author jimk 
 * @date 2019-07
 */

#ifndef _VRCT_API_H_
#define _VRCT_API_H_

/** ���İ�����*/
#define     VRCT_PACK_MAX           65536

/** Э����ͷ���趨: ����Сֱ�ӹ涨��INT32_T����ʾ*/
typedef struct tagVosReactorSetPacketInfo
{
    INT32_T     MaxSize;                                    /** ����󳤶�*/
    INT32_T     HeadOffset;                                 /** ͷ����Сƫ��*/
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


/** IO�Ĺ���ģʽ*/
typedef enum
{
    VRCT_IOTYPE_NET = 0,                                    /** ����IO*/
    VRCT_IOTYPE_MSG,                                        /** ��ϢIO*/
    VRCT_IOTYPE_TIMER,                                      /** ��ʱ��IO*/
    VRCT_IOTYPE_SPAIR,                                      /** ��Ϣ��IO*/
    
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
    INT32_T                     fd;                         /** �����¼���socket��*/
    INT32_T                     IoType;                     /** IO����*/
    UINT32_T                    EventMask;                  /** �����¼���עEPOLL_IN/ EPOLL_OUT*/
    VRCT_CALLBACK_S             stRecv;                     /** ���յ��¼�����ص������ͽڵ�ָ��*/
    VRCT_CALLBACK_S             stSend;                     /** ���͵��¼�����ص������ͽڵ�ָ��*/
}VRCT_NETEVT_OPT_S,*PVRCT_NETEVT_OPT_S;

/** ��ʱ��������*/
typedef enum
{
    VRCT_TMTYPE_ONE = 0,                                    /** һ���Զ�ʱ��*/
    VRCT_TMTYPE_RECYLE,                                     /** ѭ����ʱ��*/
    
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
    INT32_T                     TimerType;                  /** 0: once����, 1:recycleѭ��*/
    UINT32_T                    TimeOut;                    /** ��ʱʱ��*/
    UINT32_T                    TimeStamp;                  /** ʱ���*/
    VRCT_CALLBACK_S             stTimercb;                  /** ���͵��¼�����ص������ͽڵ�ָ��*/
}VRCT_TIMER_OPT_S,*PVRCT_TIMER_OPT_S;


#define VRCT_MSQOPT_INIT(pstMsqOpts_, PipeFliterID_, pfMsqHandler_, pvConner_) do{\
    VOS_DLIST_INIT(&(pstMsqOpts_)->stNode);\
    (pstMsqOpts_)->PipeFliterID = (PipeFliterID_);\
    VRCT_CALLBACK_INIT(&(pstMsqOpts_)->stMsgQueCB, pfMsqHandler_, pvConner_);\
}while(0);

typedef struct tagVosReactorMessageQueueOption
{
    VOS_DLIST_S                 stNode;
    UINT32_T                    PipeFliterID;               /** �ܵ�������ģ��ID*/
    VRCT_CALLBACK_S             stMsgQueCB;                 /** ��Ϣ����*/
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

