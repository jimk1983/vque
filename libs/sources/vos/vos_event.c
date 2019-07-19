/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_event.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : VOS的事件封装
  函数列表   :
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vos_event.h
 * @brief define event operations
 * @author jiangkang 
 * @date update bye 2019-07
 */
#include <vos/vos_pub.h>


/**
 * @brief init a new event handle
 * @param pstEvent [in] the create event handle
 */
INT32_T  VOS_ThreadEvent_Init(VOS_EVT_T *pstEvent)
{
    INT32_T uiErr = 0;
#if VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    pthread_condattr_t attr;
    
    if ( NULL == pstEvent )
    {
        return VOS_ERR;
    }
    
    pstMutex = &pstEvent->stMutex;
    pstCond = &pstEvent->stCond;
    
    uiErr = pthread_mutex_init(pstMutex, NULL);
    if (uiErr != 0)
    {
        return VOS_ERR;
    }
    uiErr = pthread_condattr_init(&attr);
    if (uiErr != 0)
    {
        pthread_mutex_destroy(pstMutex);
        return VOS_ERR;
    }
    uiErr = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    if (uiErr != 0)
    {
        pthread_mutex_destroy(pstMutex);
        return VOS_ERR;
    }
    
    uiErr = pthread_cond_init(pstCond, &attr);
    if (uiErr != 0)
    {
        pthread_mutex_destroy(pstMutex);
        return VOS_ERR;
    }
#elif VOS_PLAT_WIN


#endif

    return VOS_OK;
}


/**
 * @brief destroy a event handle
 * @param pstEvent [in] the create event handle
 */
void VOS_ThreadEvent_Destroy(VOS_EVT_T *pstEvent)
{

#if VOS_PLAT_LINUX
    if ( NULL != pstEvent )
    {
        pthread_mutex_destroy(&pstEvent->stMutex);
        pthread_cond_destroy(&pstEvent->stCond);
    }
#elif VOS_PLAT_WIN
    
    
#endif
}


/**
 * @brief waitfor a event handle
 * @param pstEvent [in] the event handle
 * @param uiMillTimeout [in] timeout
 */
INT32_T VOS_ThreadEvent_Waitfor(VOS_EVT_T *pstEvent, UINT32_T uiMillTimeout)
{
    INT32_T             uiErr = 0;
    
#if VOS_PLAT_LINUX
    pthread_mutex_t*    pstMutex = NULL;
    pthread_cond_t*     pstCond = NULL;
    VOS_ABS_TIME_S      stAbsTime = {{0}};
    
    pstMutex = &pstEvent->stMutex;
    pstCond = &pstEvent->stCond;
    
    //alway for wait
    if ( uiMillTimeout == 0)
    {
        uiErr = pthread_cond_wait(pstCond, pstMutex);
        if ( uiErr != 0 )
        {
            pthread_mutex_unlock(pstMutex);
            return VOS_ERR;
        }
        else
        {
            //OK
            pthread_mutex_unlock(pstMutex);
            return VOS_OK;
        }
    }
    else
    {
        VOS_GetAbsTime(&stAbsTime,uiMillTimeout);
        uiErr = pthread_cond_timedwait(pstCond, pstMutex, &(stAbsTime.stAbsTime));
        if(uiErr == 0)
        {
            //OK
            pthread_mutex_unlock(pstMutex);
        }
        else
        {
            //OK
            pthread_mutex_unlock(pstMutex);
            return VOS_SYS_TIMEOUT;
        }
    }
#elif VOS_PLAT_WIN
    
    
#endif

    return VOS_ERR;
}



/**
 * @brief set a event
 * @param pstEvent [in] the event handle
 */
INT32_T  VOS_ThreadEvent_Notify(VOS_EVT_T *pstEvent)
{
    INT32_T uiErr = 0;

#if VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    
    pstMutex = &pstEvent->stMutex;
    pstCond = &pstEvent->stCond;
    
    pthread_mutex_lock(pstMutex);
    uiErr = pthread_cond_signal(pstCond);
    if ( uiErr == 0 )
    {
        pthread_mutex_unlock(pstMutex);
        return VOS_OK;
    }
    
    pthread_mutex_unlock(pstMutex);
    
#elif VOS_PLAT_WIN
    
    
#endif
    return VOS_ERR;
}




