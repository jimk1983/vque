/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_event.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : VOS���¼���װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_EVENT_H_
#define _VOS_EVENT_H_

#if VOS_PLAT_LINUX
struct tagLinuxEvent
{
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
};
typedef struct tagLinuxEvent VOS_EVT_T, *PVOS_EVT_T;
#elif VOS_PLAT_MAC

#elif VOS_PLAT_WIN
#ifndef HANDLE
#define HANDLE (void *)
#endif
typedef HANDLE  VOS_EVT_T, *PVOS_EVT_T;
#endif


INT32_T  VOS_ThreadEvent_Init(VOS_EVT_T *pstEvent);

void     VOS_ThreadEvent_Destroy(VOS_EVT_T *pstEvent);

INT32_T  VOS_ThreadEvent_Waitfor(VOS_EVT_T *pstEvent, UINT32_T uiMillTimeout);

INT32_T  VOS_ThreadEvent_Notify(VOS_EVT_T *pstEvent);


#endif
