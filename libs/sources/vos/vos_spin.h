/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_spin.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : VOS的spinlock封装
  函数列表   :
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jimk
    修改内容   : 创建文件

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
