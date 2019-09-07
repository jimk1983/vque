/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_shm.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��7��
  ����޸�   :
  ��������   : �����ڴ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_SHAREMEM_H_
#define _VOS_SHAREMEM_H_


/*�����ڴ��Ĭ�����ݿ��С128K*/
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


