/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_time.h
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统时间的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/

#ifndef _VOS_TIME_H_
#define _VOS_TIME_H_

typedef struct tagVosAbsTime
{
    struct timespec stAbsTime;
}VOS_ABS_TIME_T;

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
    UINT32_T  uiMicroSec;
    
    UINT32_T  uiDayOfWeek;
}VOS_SYS_TIME_T;

void    VOS_Sleep(UINT32_T ulSeconds);
void    VOS_MSleep(UINT32_T ulMSleep);
void    VOS_USleep(UINT32_T ulUSleep);
void    VOS_GetAbsTime(VOS_ABS_TIME_T *pstAbsTime, UINT32_T ulTimeMiliSec);
void    VOS_GetLocalSysTime(VOS_SYS_TIME_T *pstSysTime);
UINT32_T VOS_GetSysTimeNow(CHAR *buf);
CHAR*   VOS_GetSysTimeNowStr();


CHAR*   VOS_TimeToString(time_t* tm, CHAR* buf);
CHAR*   VOS_TimeNow(CHAR* buf);
time_t  VOS_TmToTime(struct tm* ptm);
void    VOS_TimeToTm(const time_t* time, struct tm* ptm);


typedef UINT64_T VOS_TSTAMP_T;


/**
 * @brief get the time stamp
 */
CINLINE VOS_TSTAMP_T VOS_GetTimeStampMs()
{
    VOS_TSTAMP_T now = 0;
#ifdef VOS_PLAT_LINUX
    struct timeval tv;
    
    gettimeofday(&tv,NULL);
    if(tv.tv_usec > 1000000){
        tv.tv_usec %= 1000000;
    }
    
    now = (time(NULL) << 10) + (tv.tv_usec >> 10);
#elif VOS_PLAT_WIN


#endif
    return now;
}



#endif

