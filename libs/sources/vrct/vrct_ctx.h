/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vque_ctx.h
  �� �� ��   : ����
  ��    ��  : jimk
  ��������   : 2019��7��22��
  ����޸�   :
  ��������   : �����Ĺ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/**
 * @vque_api.c
 * @brief vos epoll queue context manager
 * @author jimk 
 * @date 2019-07
 */
#ifndef _VRCT_CTX_H_
#define _VRCT_CTX_H_

/** VRCT��ʼ����Ϣ*/
typedef struct tagVosEventReactorInit
{
    INT32_T                     TaskID;                     /** ����ID*/
    CHAR                        acGuid[SYS_GUID_LEN];       /** ��ReactorΨһGUID*/
    INT32_T                     Epollfd;                    /** ��Reactor��EpollFD*/
    INT32_T                     LogLevel;                   /** ��־����*/
    INT32_T                     MsqSize;                    /** ��Reactor����Ϣ����*/
    INT32_T                     Stop;                       /** ��Reactorֹͣ*/
    PFVRCT_COMM_CB              pfInitCB;                   /** �̳߳�ʼ��*/
    PVOID                       pvInitCtx;                  /** �̳߳�ʼ��������*/
}VRCT_INIT_S, *PVRCT_INIT_S;

/** VOS��Reactor���ṹ*/
struct tagVosEventReactor
{
    PVRCT_REACTOR_S             pstVRctor;
    
    VRCT_INIT_S                 stInfo;                     /** ��ʼ����Ϣ*/
    
    VRCT_MGR_NETEVT_S           stMgrNet;                   /** ����FD�Ĺ�����*/
    //VRCT_MGR_MSEQUE_S           stMgrMsq;                   /** ��Ϣ������*/
    //VRCT_MGR_TIMER_S            stMgrTimer;                 /** ��ʱ��������*/
    //VRCT_MGR_EXPIR_S            stMgrExpirer;               /** �ϻ�������*/
    VOS_EVT_T                   hWaitForStart;              /** ��ʼ���������*/
    VOS_EVT_T                   hWaitForExit;               /** ��ʵ���̰߳�ȫ�˳�*/
};






#endif
