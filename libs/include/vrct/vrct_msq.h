/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vrct_msq.h
  �� �� ��   : ����
  ��    ��  : jimk
  ��������   : 2019��7��22��
  ����޸�   :
  ��������   : vrct����Ϣ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/**
 * @vque_msq.c
 * @brief vos reactor message queue manager
 * @author jimk 
 * @date 2019-07
 */
  
#ifndef _VRCT_MSQ_H_
#define _VRCT_MSQ_H_

/** �ܵ����������֧��64��*/
#define     VRCT_MSQPF_NUMS         64

typedef struct tagVosReactorSockPair
{
    INT32_T                     iLocalRxFd;
    INT32_T                     iLocalRxPort;
    struct sockaddr_in          stRxAddr;
    INT32_T                     iLocalTxFd;
    INT32_T                     iLocalTxPort;
}VRCT_SOCKPAIR_S, *PVRCT_SOCKPAIR_S;

/*��Ϣ���ڵ�*/
typedef struct tagVosReactorMessageQueueEntry
{
    VOS_DLIST_S                 stNode;
    UINT32_T                    PipeFliterID;                   /** �ܵ�����������ID*/
    UINT32_T                    MsgCode;                        /** ��Ϣ������*/
    UINT32_T                    Value;                          /** ��ֵ����*/
    UINT32_T                    MsgSize;                        /** ���ݴ����С*/
    VOID*                       pvMsgData;                      /** �û����ݱ���, ��ǰ����ָ����ʽ*/
}VRCT_MSQ_ENTRY_S, *PVRCT_MSQ_ENTRY_S;


/** TODO:���������޸�Ϊ��������*/
struct tagVosReactorMessageQueueManager
{
    PVRCT_REACTOR_S             pstRctor;
    UINT32_T                    MaxSize;                        /** �����Ϣ����*/
    
    INT32_T                     Eventfd;                        /** �¼�FD*/
    VRCT_NETEVT_OPT_S           stMsqNetOpt;                    /** �¼�FD��Ӧ����*/
    PVRCT_MSQ_OPT_S             apstMsgOpts[VRCT_MSQPF_NUMS];   /** �ܵ�������ע��:0��Ϊ�Ĳ�*/
    
    VOS_DLIST_S                 stIdleList;                     /** ��������*/
    VINT32_T                    iIdleNums;                      /** ������������*/
    VOS_MTX_T                   stIdleLock;                     /** �߳���*/

    VOS_DLIST_S                 stUsedList;                     /** ��ʹ������*/
    VINT32_T                    iUsedNums;                      /** ��ʹ����������*/
    VOS_MTX_T                   stUsedLock;                     /** �߳���*/
    
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
 









