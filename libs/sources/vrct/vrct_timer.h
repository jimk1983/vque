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
    VRCT_NETEVT_OPT_S           stQuickOps;                 /** socketfd�������飬�ռ任ʱ��*/
    VOS_DLIST_S                 stQuickList;                /*�춨ʱ����*/
    
    VRCT_NETEVT_OPT_S           stSlowOps;                  /** socketfd�������飬�ռ任ʱ��*/
    VOS_DLIST_S                 stSlowList;                 /*����ʱ����*/
    
    VOS_SPINLOCK_T              stPerfLock;                 /*ִ���߳���:���ڶ�ʱ��ִ������ʱ�����*/
    VOS_DLIST_S                 stPerfList;                 /*ִ�ж�ʱ����*/
};

/*�����ڵ���Ҫ��������Ϣ*/
struct tagVosReactorTimerOpts
{
    VOS_DLIST_S                 stNode;
    INT32_T                     TimerType;                  /** 0: once����, 1:recycleѭ��*/
    UINT32_T                    TimeOut;                    /** ��ʱʱ��*/
    UINT32_T                    TimeStamp;                  /** ʱ���*/
    VRCT_CALLBACK_S             stTimercb;                  /** ���͵��¼�����ص������ͽڵ�ָ��*/
};


#endif
  









