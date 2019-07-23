/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vrct_macro.h
  �� �� ��   : ����
  ��    ��  : jimk
  ��������   : 2019��7��22��
  ����޸�   :
  ��������   : vos��queue�궨��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/**
 * @vrct_macro.c
 * @brief vos reactor macro define
 * @author jimk 
 * @date 2019-07
 */
#ifndef _VRCT_MACRO_H_
#define _VRCT_MACRO_H_
     

/*����FD��*/
#define VRCT_FDMAX                          102400
/*����EVENT��*/
#define VRCT_EVTMAX                         10240
/*���ʱ����*/
#define VRCT_FDTIMEMAX                      VRCT_FDMAX

/*ϵͳ���Ķ����߳���*/
#define VRCT_SYS_MAX                        1024
/*���ص�ˮλ*/
#define VRCT_BUFLO_LEVEL                    10
/*���ظ�ˮλ*/
#define VRCT_BUFHI_LEVEL                    1000


/** ͨ�ýṹ����*/
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


typedef struct tagVosEventReactor               VRCT_REACTOR_S, *PVRCT_REACTOR_S;
typedef struct tagVosReactorNetEvtManager       VRCT_MGR_NETEVT_S,*PVRCT_MGR_NETEVT_S;
typedef struct tagVosReactorNetEvtOpts          VRCT_NETEVT_OPT_S,*PVRCT_NETEVT_OPT_S;




#endif






