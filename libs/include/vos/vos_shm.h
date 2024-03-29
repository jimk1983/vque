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
#ifndef _VOS_SHAREMEM_H_
#define _VOS_SHAREMEM_H_


/*共享内存的默认数据块大小128K*/
#define VOS_SHMEM_SIZE  131072

#if VOS_PLAT_LINUX
typedef struct tagLinuxMemUnit
{
    pthread_mutex_t stProcMutex;
    char            acMapMem[VOS_SHMEM_SIZE];   
}LINUX_MEM_UNIT, *PLINUX_MEM_UNIT;

typedef struct tagLinuxSHareMem
{
    INT32_T shmid;
    key_t   key;
    PLINUX_MEM_UNIT pstMem;
}LINUX_SHAREMEM_S;
typedef LINUX_SHAREMEM_S VOS_SHM_T, *PVOS_SHM_T;
#elif VOS_PLAT_MAC

#elif VOS_PLAT_WIN

#endif


void*   VOS_ShareMem_Create(VOS_SHM_T *pstHandle, INT8_T *pcKeyString, UINT32_T uiSize);

void*   VOS_ShareMem_Open(VOS_SHM_T *pstHandle, INT8_T *pcKeyString);

void    VOS_ShareMem_Lock(VOS_SHM_T *pstHandle);

void    VOS_ShareMem_UnLock(VOS_SHM_T *pstHandle);

void    VOS_ShareMem_Close(VOS_SHM_T *pstHandle);




#endif


