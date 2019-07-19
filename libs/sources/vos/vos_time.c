/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_time.c
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳʱ�������
  �����б�   :
              VOS_GetAbsTime
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos/vos_pub.h>


/**
 * @brief sleep
 * @param ulSeconds [in] sec-sleep times
 */
void VOS_Sleep(UINT32_T ulSeconds)
{
    
#if VOS_PLAT_LINUX
    sleep(ulSeconds);
#elif VOS_PLAT_WIN
    
#endif

    return;
}


/**
 * @brief sleep
 * @param ulMSleep [in] ms-sleep times
 */
void VOS_MSleep(UINT32_T ulMSleep)
{
    
#if VOS_PLAT_LINUX

    usleep(ulMSleep*1000);
#elif VOS_PLAT_WIN
    
#endif

    return;
}


/**
 * @brief sleep
 * @param ulUSleep [in] us-sleep times
 */
void VOS_USleep(UINT32_T ulUSleep)
{

#if VOS_PLAT_LINUX
    usleep(ulUSleep);
#elif VOS_PLAT_WIN
    
#endif
    return;
}


/**
 * @brief get the system-time abs
 * @param pstEvent [in] the event handle
 * @param uiMillTimeout [in] timeout
 */
void VOS_GetAbsTime(VOS_ABS_TIME_S *pstAbsTime, UINT32_T ulTimeMiliSec)
{
    UINT32_T ulSec     =   0;
    UINT32_T ulNSec    =   0;
    UINT32_T ulSum     =   0;
    struct timespec *pstAbsT = NULL;
    
    if ( NULL == pstAbsTime )
    {
        return;
    }
    
#if VOS_PLAT_LINUX
    pstAbsT = &(pstAbsTime->stAbsTime);
    #if 1
        struct timespec currentTime;
        (void)clock_gettime(CLOCK_MONOTONIC, &currentTime);
        
        ulNSec = (uint32_t)currentTime.tv_nsec;
        ulSum = (ulTimeMiliSec / 1000);
        ulNSec += ulSum * 1000000;
        pstAbsT->tv_nsec = ulNSec % 1000000000;
        ulSec = (uint32_t)currentTime.tv_sec;
        ulSec += ulTimeMiliSec / 1000;        
        ulSec += ulNSec /1000000000;
        pstAbsT->tv_sec = (int32_t)ulSec;
    #else
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        ulNSec = (uint32_t)currentTime.tv_usec * 1000;
        ulNSec += (ulTimeMiliSec%1000) * 1000000;
        pstAbsT->tv_nsec = ulNSec / 1000000000;
        ulSec = (uint32_t)currentTime.tv_sec;
        ulSec += ulTimeMiliSec %1000;
        ulSec += ulNSec /1000000000;
        pstAbsT->tv_sec = (int32_t)ulSec;
    #endif
#elif VOS_PLAT_WIN

#endif

    return;
}


/**
 * @brief get the system-time formats
 * @param pstSysTime [in] the formats system-times
 */
void VOS_GetLocalSysTime(VOS_LOCAL_SYSTEM_TIME_S *pstSysTime)
{
    struct tm *pstTime = NULL;
    struct timeval tv;;
    time_t t=time(NULL);
    
    if ( NULL == pstSysTime )
    {
        return;
    }
    
#if VOS_PLAT_LINUX
    memset((char *)pstSysTime, 0, sizeof(VOS_LOCAL_SYSTEM_TIME_S));

    /*��ȡ������,ʱ����*/
    pstTime  = localtime(&t);


    pstSysTime->uiYear       =  pstTime->tm_year + 1900;
    pstSysTime->uiMoth      =  pstTime->tm_mon + 1;
    pstSysTime->uiDay        =  pstTime->tm_mday;

    pstSysTime->uiHour      =  pstTime->tm_hour;
    pstSysTime->uiMinute   =  pstTime->tm_min;
    pstSysTime->uiSecond   =  pstTime->tm_sec;
    
    /*��ȡ����ʱ��*/
    gettimeofday(&tv, NULL);
    
    pstSysTime->uiMiliSec   =  tv.tv_usec;
#elif VOS_PLAT_WIN
        
#endif

    return;
}

