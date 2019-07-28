/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_time.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统时间的适配
  函数列表   :
              VOS_GetAbsTime
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

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
void VOS_GetAbsTime(VOS_ABS_TIME_T *pstAbsTime, UINT32_T ulTimeMiliSec)
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
        
        ulNSec = (UINT32_T)currentTime.tv_nsec;
        ulSum = (ulTimeMiliSec / 1000);
        ulNSec += ulSum * 1000000;
        pstAbsT->tv_nsec = ulNSec % 1000000000;
        ulSec = (UINT32_T)currentTime.tv_sec;
        ulSec += ulTimeMiliSec / 1000;        
        ulSec += ulNSec /1000000000;
        pstAbsT->tv_sec = (UINT32_T)ulSec;
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
void VOS_GetLocalSysTime(VOS_SYS_TIME_T *pstSysTime)
{
    struct tm *pstTime = NULL;
    struct timeval tv;;
    time_t t=time(NULL);
    
    if ( NULL == pstSysTime )
    {
        return;
    }
    
#if VOS_PLAT_LINUX
    memset((char *)pstSysTime, 0, sizeof(VOS_SYS_TIME_T));

    /*获取年月日,时分秒*/
    pstTime  = localtime(&t);


    pstSysTime->uiYear       =  pstTime->tm_year + 1900;
    pstSysTime->uiMoth      =  pstTime->tm_mon + 1;
    pstSysTime->uiDay        =  pstTime->tm_mday;

    pstSysTime->uiHour      =  pstTime->tm_hour;
    pstSysTime->uiMinute   =  pstTime->tm_min;
    pstSysTime->uiSecond   =  pstTime->tm_sec;
    
    /*获取毫秒时间*/
    gettimeofday(&tv, NULL);
    
    pstSysTime->uiMiliSec   =  tv.tv_usec;
#elif VOS_PLAT_WIN
    SYSTEMTIME st;
    
    GetLocalTime(&st);

    pstSysTime->uiYear = st.wYear;
    pstSysTime->uiMoth = st.wMonth;
    pstSysTime->uiDay  = st.wDay;
    pstSysTime->uiHour = st.wHour;
    pstSysTime->uiMinute = st.wMinute;
    pstSysTime->uiSecond = st.wSecond;
    
#endif

    return;
}


/**
 * @brief get the system-time now
 * @param buf [inout] time buffer
 */
UINT32_T VOS_GetSysTimeNow(CHAR *buf){
    VOS_SYS_TIME_T  vtm = {0};
    UINT32_T        ret = 0;
    
#ifdef VOS_PLAT_LINUX
    struct timeval tv;
    struct tm now;
    ret = gettimeofday(&tv,NULL);
    if( VOS_OK != ret )
    {
        return 0;
    }
    localtime_r(&tv.tv_sec, &now);

    vtm.uiYear = now.tm_year + 1900;
    vtm.uiMoth = now.tm_mon + 1;
    vtm.uiDay = now.tm_mday;
    vtm.uiHour = now.tm_hour;
    vtm.uiMinute = now.tm_min;
    vtm.uiSecond = now.tm_sec;
    vtm.uiMiliSec = tv.tv_usec/1000;
    vtm.uiMicroSec = tv.tv_usec%1000;
    
    vtm.uiDayOfWeek = now.tm_wday;
#else /* VSYS_WINDOWS */
    SYSTEMTIME st;
    ret = VOS_OK;
    GetLocalTime(&st);

    vtm.uiYear = st.wYear;
    vtm.uiMoth = st.wMonth;
    vtm.uiDay = st.wDay;
    vtm.uiHour = st.wHour;
    vtm.uiMinute = st.wMinute;
    vtm.uiSecond = st.wSecond;
    vtm.uiMiliSec = st.wMilliseconds;
    vtm.uiMicroSec = 0;
    
    vtm.uiDayOfWeek = st.wDayOfWeek;
#endif

    ret = sprintf((char *)buf, "%d-%02d-%02d %02d:%02d:%02d.%03d",
                      vtm.uiYear, vtm.uiMoth, vtm.uiDay, vtm.uiHour,
                      vtm.uiMinute, vtm.uiSecond, vtm.uiMiliSec);

    return ret;
}


/**
 * @brief get the system-time now
 * @param buf [inout] time buffer
 */
static __thread CHAR     g_th_timebuf[32];
CHAR* VOS_GetSysTimeNowStr()
{
    VOS_SYS_TIME_T  vtm = {0};
    UINT32_T        ret = 0;
    
    memset(g_th_timebuf,0,32);
#ifdef VOS_PLAT_LINUX
    struct timeval tv;
    struct tm now;
    ret = gettimeofday(&tv,NULL);
    if( VOS_OK != ret )
    {
        return 0;
    }
    localtime_r(&tv.tv_sec, &now);

    vtm.uiYear = now.tm_year + 1900;
    vtm.uiMoth = now.tm_mon + 1;
    vtm.uiDay = now.tm_mday;
    vtm.uiHour = now.tm_hour;
    vtm.uiMinute = now.tm_min;
    vtm.uiSecond = now.tm_sec;
    vtm.uiMiliSec = tv.tv_usec/1000;
    vtm.uiMicroSec = tv.tv_usec%1000;
    
    vtm.uiDayOfWeek = now.tm_wday;
#else /* VSYS_WINDOWS */
    SYSTEMTIME st;
    ret = VOS_OK;
    GetLocalTime(&st);

    vtm.uiYear = st.wYear;
    vtm.uiMoth = st.wMonth;
    vtm.uiDay = st.wDay;
    vtm.uiHour = st.wHour;
    vtm.uiMinute = st.wMinute;
    vtm.uiSecond = st.wSecond;
    vtm.uiMiliSec = st.wMilliseconds;
    vtm.uiMicroSec = 0;
    
    vtm.uiDayOfWeek = st.wDayOfWeek;
#endif

    ret = sprintf((char *)g_th_timebuf, "[%d-%02d-%02d %02d:%02d:%02d.%03d]",
                      vtm.uiYear, vtm.uiMoth, vtm.uiDay, vtm.uiHour,
                      vtm.uiMinute, vtm.uiSecond, vtm.uiMiliSec);
    
    return g_th_timebuf;
}


/**
 * @brief time to string
 * @param buf [inout] time string
 */
CHAR* VOS_TimeToString(time_t* tm, CHAR* buf)
{
    struct tm* tmnow = localtime(tm);
    if (NULL != buf)
    {
        sprintf((char *)buf, "%d-%02d-%02d %02d:%02d:%02d", tmnow->tm_year + 1900, 
                                                    tmnow->tm_mon + 1, 
                                                    tmnow->tm_mday, 
                                                    tmnow->tm_hour, 
                                                    tmnow->tm_min, 
                                                    tmnow->tm_sec);
    }
    return buf;
}



/**
 * @brief time string
 * @param buf [inout] time now
 */
CHAR* VOS_TimeNow(CHAR* buf)
{
    time_t now;
    struct tm* tmnow;
    time(&now);
    tmnow = localtime(&now);
    sprintf((char *)buf, "%d-%02d-%02d %02d:%02d:%02d", tmnow->tm_year + 1900, 
                                                tmnow->tm_mon + 1, 
                                                tmnow->tm_mday, 
                                                tmnow->tm_hour, 
                                                tmnow->tm_min, 
                                                tmnow->tm_sec);
    return buf;
}



/**
 * @brief tm to time_t
 * @param ptm [in] tm time
 */
time_t VOS_TmToTime(struct tm* ptm)
{
    ptm->tm_year -= 1900;
    ptm->tm_mon -= 1;
    ptm->tm_isdst = -1;
    return mktime(ptm);
}

/**
 * @brief time_t to tm
 * @param ptm [in] tm time
 */
void VOS_TimeToTm(const time_t* time, struct tm* ptm)
{
    struct tm* ptime;
    
    if (NULL == time || NULL == ptm)
    {
        return;
    }
    
    ptime = localtime(time);
    ptm->tm_year = ptime->tm_year + 1900;
    ptm->tm_mon  = ptm->tm_mon + 1;
    ptm->tm_mday = ptime->tm_mday;
    ptm->tm_hour = ptime->tm_hour;
    ptm->tm_min  = ptime->tm_min;
    ptm->tm_sec  = ptime->tm_sec;
}



