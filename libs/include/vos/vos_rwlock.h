/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_rwlock.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2018��9��25��
  ����޸�   :
  ��������   : VOS��rwlock��װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��9��25��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_RWLOCK_H_
#define _VOS_RWLOCK_H_

#if VOS_PLAT_LINUX
typedef pthread_rwlock_t VOS_RWLOCK_T, *PVOS_RWLOCK_T;

#define VOS_RWLOCK_Init(pstRWLock)              pthread_rwlock_init(pstRWLock, 0)
#define VOS_RWLOCK_Destroy(pstRWLock)           pthread_rwlock_destroy(pstRWLock)
#define VOS_RWLOCK_RdLock(pstRWLock)            pthread_rwlock_rdlock(pstRWLock)
#define VOS_RWLOCK_TryRdLock(pstRWLock)         (pthread_rwlock_tryrdlock(pstRWLock) ? VOS_ERR_EAGAIN : VOS_OK)
#define VOS_RWLOCK_WrLock(pstRWLock)            pthread_rwlock_wrlock(pstRWLock)
#define VOS_RWLOCK_TryWrLock(pstRWLock)         (pthread_rwlock_trywrlock(pstRWLock) ? VOS_ERR_EAGAIN : VOS_OK)
#define VOS_RWLOCK_TimedRdLock(pstRWLock, sec, ret) do{   \
  struct timespec spec;\
  spec.tv_sec = sec;\
  spec.tv_nsec = 0;\
  ret = pthread_rwlock_timedrdlock(pstRWLock, &spec) ? VOS_ERR_EAGAIN : VOS_OK;  \
}while(0)

#define VOS_RWLOCK_TimedWrLock(pstRWLock, sec, ret) do{\
    struct timespec spec;\
    spec.tv_sec = sec;\
    spec.tv_nsec = 900000000;\
    ret = pthread_rwlock_timedwrlock(pstRWLock, &spec) ? VOS_ERR_EAGAIN : VOS_OK;\
}while(0)
#define VOS_RWLOCK_Unlock(pstRWLock, is_read)   pthread_rwlock_unlock(pstRWLock)

#elif VOS_PLAT_WIN

typedef SRWLOCK VOS_RWLOCK_T, *PVOS_RWLOCK_T;

#define VOS_RWLOCK_Init(pstRWLock)              InitializeSRWLock(pstRWLock)
#define VOS_RWLOCK_Destroy(pstRWLock)
#define VOS_RWLOCK_RdLock(pstRWLock)            AcquireSRWLockShared(pstRWLock)
#define VOS_RWLOCK_TryRdLock(pstRWLock)         (TryAcquireSRWLockShared(pstRWLock) ? VOS_OK : VOS_ERR_EAGAIN)
#define VOS_RWLOCK_WrLock(pstRWLock)            AcquireSRWLockExclusive(pstRWLock)
#define VOS_RWLOCK_TryWrLock(pstRWLock)         (TryAcquireSRWLockExclusive(pstRWLock) ? VOS_OK : VOS_ERR_EAGAIN)
#define VOS_RWLOCK_TimedRdLock(pstRWLock, sec, ret) do{\
CONDITION_VARIABLE cv;\
InitializeConditionVariable(&cv);\
ret = SleepConditionVariableSRW(&cv, pstRWLock, sec*1000, CONDITION_VARIABLE_LOCKMODE_SHARED)? VOS_OK : VOS_ERR_EAGAIN;\
}while(0)
#define VOS_RWLOCK_TimedWrLock(pstRWLock, sec, ret) do{\
    CONDITION_VARIABLE cv;\
    InitializeConditionVariable(&cv);\
    ret = SleepConditionVariableSRW(&cv, pstRWLock, sec*1000, 0xffff)? VOS_OK : VOS_ERR_EAGAIN;\
}while(0)

#define VOS_RWLOCK_Unlock(pstRWLock, is_read) (is_read ? ReleaseSRWLockShared(pstRWLock) : ReleaseSRWLockExclusive(pstRWLock))

#endif


#endif
