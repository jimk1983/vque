/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_time.h
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳʱ���ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef _VOS_TIME_H_
#define _VOS_TIME_H_

typedef struct tagVosAbsTime
{
    struct timespec stAbsTime;
}VOS_ABS_TIME_S;

#define VOS_LOCAL_SYSTIME_BUFLEN        28

typedef struct tagLocalSysTime
{
    UINT32_T  uiYear;
    UINT32_T  uiMoth;
    UINT32_T  uiDay;
    UINT32_T  uiHour;
    UINT32_T  uiMinute;
    UINT32_T  uiSecond;
    UINT32_T  uiMiliSec;    
}VOS_LOCAL_SYSTEM_TIME_S;

void VOS_Sleep(UINT32_T ulSeconds);
void VOS_MSleep(UINT32_T ulMSleep);
void VOS_USleep(UINT32_T ulUSleep);
void VOS_GetAbsTime(VOS_ABS_TIME_S *pstAbsTime, UINT32_T ulTimeMiliSec);
void VOS_GetLocalSysTime(VOS_LOCAL_SYSTEM_TIME_S *pstSysTime);


#endif

