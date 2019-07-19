/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_spin.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : VOS��spinlock��װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_SPIN_H_
#define _VOS_SPIN_H_


#if VOS_PLAT_WIN
typedef CRITICAL_SECTION VOS_SPINLOCK_T, *PVOS_SPINLOCK_T;
#define VOS_Spin_Init(pstSpinLock)         (InitializeCriticalSectionEx(pstSpinLock, 81920, CRITICAL_SECTION_NO_DEBUG_INFO) - 1)
#define VOS_Spin_Destroy(pstSpinLock)      DeleteCriticalSection(pstSpinLock)
#define VOS_Spin_Lock(pstSpinLock)         EnterCriticalSection(pstSpinLock)
#define VOS_Spin_Unlock(pstSpinLock)       LeaveCriticalSection(pstSpinLock)
#define VOS_Spin_Trylock(pstSpinLock)      (TryEnterCriticalSection(pstSpinLock) - 1)
#else
typedef pthread_spinlock_t VOS_SPINLOCK_T, *PVOS_SPINLOCK_T;
#define VOS_Spin_Init(pstSpinLock)         pthread_spin_init(spin, PTHREAD_PROCESS_PRIVATE)
#define VOS_Spin_Destroy(pstSpinLock)      pthread_spin_destroy(spin)
#define VOS_Spin_Lock(pstSpinLock)         pthread_spin_lock(spin)
#define VOS_Spin_Unlock(pstSpinLock)       pthread_spin_unlock(spin)
#define VOS_Spin_Trylock(pstSpinLock)      pthread_spin_trylock(spin)
#endif

#endif
