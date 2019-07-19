
#ifndef _VOS_MUTEX_H_
#define _VOS_MUTEX_H_


//SM_P(), SM_V(), the same as EventWait(), EventNotify()
#if VOS_PLAT_LINUX
typedef struct tagLinuxSem
{
    uint32_t V;
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
}LINUX_SEM_S;
typedef LINUX_SEM_S VOS_SM_T;
#elif VOS_PLAT_MAC
typedef struct tagMacOSSem
{
    uint32_t V;
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
}MACOS_SEM_S;
typedef MACOS_SEM_S VOS_SM_T;

#elif VOS_PLAT_WIN
#ifndef HANDLE
#define HANDLE (void *)
#endif
typedef HANDLE  VOS_SM_T;
#endif


#if VOS_PLAT_LINUX
typedef struct tagLinuxRWLock
{
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
    uint32_t              uiStatus;
    volatile uint32_t     uiLockFlag;
}LINUX_RW_LOCK_S;
typedef LINUX_RW_LOCK_S VOS_RW_LOCK_S;
#elif VOS_PLAT_WIN
typedef struct tagWinRWLock
{
    CRITICAL_SECTION stMutex;
    int32_t            stCond;
    uint32_t           uiStatus;
    volatile uint32_t  uiLockFlag;
}WIN_RW_LOCK_S;
typedef WIN_RW_LOCK_S VOS_RW_LOCK_S;
#endif



#if VOS_PLAT_LINUX
#define __xchg_op(x, ptr) \
    ({ \
    \
    __typeof(*(ptr)) __ret = (x); \
    switch (sizeof(*(ptr))) { \
\
    case 1:      \
    asm volatile("xchgb %b0,%1\n"   \
    : "+q" (__ret), "+m" (*(ptr))   \
    ::"memory", "cc");  \
    break;  \
    case 2: \
    asm volatile("xchgw %w0,%1\n"   \
    : "+r" (__ret), "+m" (*(ptr))   \
    :: "memory", "cc");      \
    break;  \
    case 4:     \
    asm volatile("xchgl %k0,%1\n"   \
    : "+r" (__ret), "+m" (*(ptr))   \
    ::"memory", "cc");  \
    break;  \
    case 8:      \
    asm volatile("xchgq %0,%1\n"    \
     : "+r" (__ret), "+m" (*(ptr))  \
    ::"memory", "cc");  \
    break;  \
}   \
__ret;  \
})

#define xchg(ptr,v) __xchg_op((v), (ptr))
#endif


INT32_T VOS_SM_Init(VOS_SM_T *pstSem);
    
INT32_T VOS_SM_P(VOS_SM_T *pstSem, uint32_t ulTimeOutMiliSec);

INT32_T VOS_SM_V(VOS_SM_T *pstSem);

INT32_T VOS_SM_Destroy(VOS_SM_T *pstSem);


/** reserved for reference */
INT32_T VOS_ThreadMutex_Init(VOS_RW_LOCK_S *pstMutex);

INT32_T VOS_ThreadMutex_RWLock(VOS_RW_LOCK_S *pstMutex);

INT32_T VOS_ThreadMutex_RWUnLock(VOS_RW_LOCK_S *pstMutex);

INT32_T VOS_ThreadMutex_Destroy(VOS_RW_LOCK_S *pstMutex);


#endif
