/******************************************************************************

                  版权所有 (C), 2018-2028, 技术有限公司

 ******************************************************************************
  文 件 名   : vos_shm.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月7日
  最近修改   :
  功能描述   : 共享内存的适配
  函数列表   :
  修改历史   :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#include <vos/vos_pub.h>

void *VOS_ShareMem_Create(VOS_SHM_T *pstHandle, INT8_T *pcKeyString, UINT32_T uiSize)
{
    void*           pvMem = NULL;
    
    if ( NULL == pstHandle
        || NULL == pcKeyString 
        || uiSize > VOS_SHMEM_SIZE )
    {
        return NULL;
    }

#if VOS_PLAT_LINUX
    pthread_mutexattr_t attr;
     
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    
    pstHandle->key = (key_t)VOS_HashStr_Normal((const CHAR *)pcKeyString);
    
    pstHandle->shmid = shmget(pstHandle->key, sizeof(LINUX_MEM_UNIT), IPC_CREAT | IPC_EXCL | 0666);
    if ( -1 == pstHandle->shmid )
    {
        PError("shmget error=%d:%s", errno, strerror(errno));
        return NULL;
    }

    pvMem = shmat(pstHandle->shmid, NULL, 0);
    if ( pvMem == (void *)-1 )
    {
        (void)shmctl(pstHandle->shmid, IPC_RMID, NULL);
        return NULL;
    }
    
    pstHandle->pstMem = (PLINUX_MEM_UNIT)pvMem;
    
    pthread_mutex_init(&pstHandle->pstMem->stProcMutex, &attr);
    memset(pstHandle->pstMem->acMapMem, 0, VOS_SHMEM_SIZE);
#elif VOS_PLAT_WIN
    
#endif

    return pstHandle->pstMem->acMapMem;
}

void *VOS_ShareMem_Open(VOS_SHM_T *pstHandle, INT8_T *pcKeyString)
{
    void*           pvMem = NULL;
    
    if ( NULL == pstHandle 
        || NULL == pcKeyString )
    {
        return NULL;
    }
        
#if VOS_PLAT_LINUX

    pstHandle->key = (key_t)VOS_HashStr_Normal((const CHAR *)pcKeyString);

    pstHandle->shmid = shmget(pstHandle->key, 0, 0);
    if ( -1 == pstHandle->shmid  )
    {
        return NULL;
    }
    
    pvMem = shmat(pstHandle->shmid, NULL, 0);
    if ( pvMem == (void *)-1 )
    {
        return NULL;
    }
    
    pstHandle->pstMem = (PLINUX_MEM_UNIT)pvMem;
    
#elif VOS_PLAT_WIN
    
#endif

    return pstHandle->pstMem->acMapMem;
}

void VOS_ShareMem_Lock(VOS_SHM_T *pstHandle)
{
    if ( NULL == pstHandle 
        || NULL == pstHandle->pstMem )
    {
        return;
    }
#if VOS_PLAT_LINUX
    
    pthread_mutex_lock(&pstHandle->pstMem->stProcMutex);
    
#elif VOS_PLAT_WIN
    
#endif

}

void VOS_ShareMem_UnLock(VOS_SHM_T *pstHandle)
{
    if ( NULL == pstHandle 
        || NULL == pstHandle->pstMem )
    {
        return;
    }
#if VOS_PLAT_LINUX
    pthread_mutex_unlock(&pstHandle->pstMem->stProcMutex);
#elif VOS_PLAT_WIN
    
#endif
}

void VOS_ShareMem_Close(VOS_SHM_T *pstHandle)
{
#if VOS_PLAT_LINUX
    if ( NULL != pstHandle )
    {
        (void)pthread_mutex_destroy(&pstHandle->pstMem->stProcMutex);
            
        (void)shmctl(pstHandle->shmid, IPC_RMID, NULL);
    }
#elif VOS_PLAT_WIN

#endif

}



