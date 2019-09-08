/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vrct_timer.h
  �� �� ��   : ����
  ��    ��  : jimk
  ��������   : 2019��7��22��
  ����޸�   :
  ��������   : vos reactor�Ķ�ʱ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/**
 * @vrct_timer.c
 * @brief vos reactor timer manager
 * @author jimk 
 * @date 2019-07
 */
#ifndef _VRCT_TIMER_H_
#define _VRCT_TIMER_H_

/*������Ϊ������ʱ�������뼶������ִ��*/
#define     VRCT_TIMER_GRAIN            1

/*�����¼�������: �����¼��������Ļ���*/
struct tagVosReactorTimerManager
{
    PVRCT_REACTOR_S             pstVReactor;                
    VRCT_NETEVT_OPT_S           stQuickOpts;                 /** socketfd�������飬�ռ任ʱ��*/
    VOS_DLIST_S                 stQuickList;                /*�춨ʱ����*/
    
    VRCT_NETEVT_OPT_S           stSlowOpts;                  /** socketfd�������飬�ռ任ʱ��*/
    VOS_DLIST_S                 stSlowList;                 /*����ʱ����*/
    
    VOS_SPINLOCK_T              stPerfLock;                 /*ִ���߳���:���ڶ�ʱ��ִ������ʱ�����*/
    VOS_DLIST_S                 stPerfList;                 /*ִ�ж�ʱ����*/
};


INT32_T     VRCT_TimerCtrlManagerInit(PVRCT_REACTOR_S          pstRctor);
VOID        VRCT_TimerCtrlManagerUnInit(PVRCT_REACTOR_S          pstRctor);

INT32_T VRCT_TimerEvtOptsRegister(PVRCT_REACTOR_S pstRctor, PVRCT_TIMER_OPT_S pstTimeOps);
VOID    VRCT_TimerEvtOptsUnRegister(PVRCT_REACTOR_S pstRctor, PVRCT_TIMER_OPT_S pstTimeOps);





#endif
  









