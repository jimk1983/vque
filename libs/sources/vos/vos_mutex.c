/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_pub.h
  �� �� ��   : ����
  ��    ��  : ����
  ��������   : 
  ����޸�   :
  ��������   : VOS����ͳһ�ṩ�Ĵ�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos/vos_pub.h>



/**
 * @brief init a new rwlock handle
 * @param pstMutex [in] the create mutex handle
 */
INT32_T VOS_ThreadMutex_Init(VOS_MUTEX_T *pstMutex)
{
#if VOS_PLAT_WIN
    CRITICAL_SECTION *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;

    InitializeCriticalSection(&pstMutexLock);

#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;

    pthread_mutex_init(pstMutexLock,NULL);
    
#elif VOS_PLAT_MAC


#endif
    __xchg_op(0, &pstMutex->uiLockFlag);

    return VOS_OK;
}

/**
 * @brief rwlock option
 * @param pstMutex [in] the mutex handle
 */
INT32_T VOS_ThreadMutex_RWLock(VOS_MUTEX_T *pstMutex)
{
#if VOS_PLAT_WIN
        CRITICAL_SECTION *pstMutexLock = NULL;

        pstMutexLock = &pstMutex->stMutex;
        
        (void)EnterCriticalSection(pstMutexLock);
        
#elif VOS_PLAT_LINUX
        pthread_mutex_t *pstMutexLock = NULL;

        pstMutexLock = &pstMutex->stMutex;
        
        __xchg_op(1, &pstMutex->uiLockFlag);
        
        (void)pthread_mutex_lock(pstMutexLock);
#elif VOS_PLAT_MAC
    
#endif
    
    return VOS_OK;
}

/**
 * @brief rwlock option
 * @param pstMutex [in] the mutex handle
 */
INT32_T VOS_ThreadMutex_RWUnLock(VOS_MUTEX_T *pstMutex)
{
#if VOS_PLAT_WIN
    CRITICAL_SECTION *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;
    
    (void)LeaveCriticalSection(pstMutexLock);
#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;
    
    (void)pthread_mutex_unlock(pstMutexLock);
            
#elif VOS_PLAT_MAC
        
#endif
            
    __xchg_op(0, &pstMutex->uiLockFlag);


    return VOS_OK;
}


/**
 * @brief rwlock option
 * @param pstMutex [in] the mutex handle
 */
INT32_T VOS_ThreadMutex_RLock(VOS_MUTEX_T *pstMutex)
{
#if VOS_PLAT_WIN
        CRITICAL_SECTION *pstMutexLock = NULL;

        pstMutexLock = &pstMutex->stMutex;

        if ( 0 == pstMutex->uiLockFlag )
        {
            return VOS_OK;
        }
        
        (void)EnterCriticalSection(pstMutexLock);
#elif VOS_PLAT_LINUX
        pthread_mutex_t *pstMutexLock = NULL;

        pstMutexLock = &pstMutex->stMutex;
    
        __xchg_op(1, &pstMutex->uiLockFlag);
        
        (void)pthread_mutex_lock(pstMutexLock);
        
#elif VOS_PLAT_MAC
    
#endif

    return VOS_OK;
}

/**
 * @brief unlock option
 * @param pstMutex [in] the mutex handle
 */
INT32_T VOS_ThreadMutex_RUnLock(VOS_MUTEX_T *pstMutex)
{
#if VOS_PLAT_WIN
            CRITICAL_SECTION *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            
            (void)LeaveCriticalSection(pstMutexLock);
#elif VOS_PLAT_LINUX
            pthread_mutex_t *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            
            (void)pthread_mutex_unlock(pstMutexLock);
#elif VOS_PLAT_MAC
        
#endif
            
    __xchg_op(0, &pstMutex->uiLockFlag);
    return VOS_OK;
}


/**
 * @brief destroy option
 * @param pstMutex [in] the mutex handle
 */
INT32_T VOS_ThreadMutex_Destroy(VOS_MUTEX_T *pstMutex)
{
#if VOS_PLAT_WIN
            CRITICAL_SECTION *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            (void)DeleteCriticalSection(pstMutexLock);
#elif VOS_PLAT_LINUX
            pthread_mutex_t *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            (void)pthread_mutex_destroy(pstMutexLock);
#elif VOS_PLAT_MAC
           //TODO: 
#endif

    return VOS_OK;
}



/**
 * @brief init the sem option
 * @param pstMutex [in] the sem handle
 */
INT32_T VOS_SM_Init(VOS_SM_T *pstSem)
{
    INT32_T uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }

    
#if VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    pthread_condattr_t attr;
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;
    
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
   // CreateSemaphore();
#elif VOS_PLAT_MAC
             
#endif
    return VOS_OK;
}


/**
 * @brief  sem P option
 * @param pstMutex [in] the sem handle
 */
INT32_T VOS_SM_P(VOS_SM_T *pstSem, UINT32_T ulTimeOutMiliSec)
{
    INT32_T uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
#if VOS_PLAT_WIN
    
#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    VOS_ABS_TIME_T stAbsTime = {{0}};
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;
    //alway for wait
    if ( ulTimeOutMiliSec == 0)
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
        VOS_GetAbsTime(&stAbsTime,ulTimeOutMiliSec);
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
            return VOS_ERR_TIMEOUT;
        }

    }
#elif VOS_PLAT_MAC
                 
#endif
    return VOS_OK;
}


/**
 * @brief  the sem V option
 * @param pstMutex [in] the sem handle
 */
INT32_T VOS_SM_V(VOS_SM_T *pstSem)
{
    INT32_T uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
#if VOS_PLAT_WIN
    
#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;
    
    pthread_mutex_lock(pstMutex);
    uiErr = pthread_cond_signal(pstCond);
    if ( uiErr == 0 )
    {
        pthread_mutex_unlock(pstMutex);
    }
#elif VOS_PLAT_MAC
                 
#endif
    return VOS_OK;
}



/**
 * @brief  destroy the sem 
 * @param pstMutex [in] the sem handle
 */
INT32_T VOS_SM_Destroy(VOS_SM_T *pstSem)
{
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
    
#if VOS_PLAT_WIN
    
#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;

    pthread_mutex_destroy(pstMutex);
    pthread_cond_destroy(pstCond);
#elif VOS_PLAT_MAC
                 
#endif

    return VOS_OK;
}


