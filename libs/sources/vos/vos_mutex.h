
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
typedef struct tagLinuxMutexLock
{
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
    uint32_t              uiStatus;
    volatile uint32_t     uiLockFlag;
}LINUX_RW_LOCK_S;
typedef LINUX_RW_LOCK_S VOS_MUTEX_T;
#elif VOS_PLAT_WIN
typedef struct tagWinMutexLock
{
    CRITICAL_SECTION stMutex;
    int32_t            stCond;
    uint32_t           uiStatus;
    volatile uint32_t  uiLockFlag;
}WIN_RW_LOCK_S;
typedef WIN_RW_LOCK_S VOS_MUTEX_T;
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



/*初始化线程共享读写锁*/
#define VOS_MUTEX_INIT(_rwlock)         VOS_ThreadMutex_Init(&(_rwlock))

/*释放共享锁*/
#define VOS_MUTEX_FREE(_rwlock)         VOS_ThreadMutex_Destroy(&(_rwlock))

/*加锁*/
#define VOS_MUTEX_LOCK(_rwLock)         VOS_ThreadMutex_RWLock(&(_rwLock))

/*解锁*/
#define VOS_MUTEX_UNLOCK(_rwLock)       VOS_ThreadMutex_RWUnLock(&(_rwLock))

/*加锁*/
#define VOS_MUTEX_PLOCK(_prwLock)       VOS_ThreadMutex_RWLock(_prwLock)

/*解锁*/
#define VOS_MUTEX_UNPLOCK(_prwLock)     VOS_ThreadMutex_RWUnLock(_prwLock)


INT32_T VOS_SM_Init(VOS_SM_T *pstSem);
    
INT32_T VOS_SM_P(VOS_SM_T *pstSem, uint32_t ulTimeOutMiliSec);

INT32_T VOS_SM_V(VOS_SM_T *pstSem);

INT32_T VOS_SM_Destroy(VOS_SM_T *pstSem);


/** reserved for reference */
INT32_T VOS_ThreadMutex_Init(VOS_MUTEX_T *pstMutex);

INT32_T VOS_ThreadMutex_RWLock(VOS_MUTEX_T *pstMutex);

INT32_T VOS_ThreadMutex_RWUnLock(VOS_MUTEX_T *pstMutex);

INT32_T VOS_ThreadMutex_Destroy(VOS_MUTEX_T *pstMutex);



#if VOS_PLAT_WIN

typedef CRITICAL_SECTION VOS_MTX_T;

#define VOS_MTX_INIT(pstMtx)               (InitializeCriticalSectionEx(pstMtx, 81920, CRITICAL_SECTION_NO_DEBUG_INFO) ? VOS_TRUE : VOS_ERR)
#define VOS_MTX_FINI(pstMtx)               DeleteCriticalSection(pstMtx)
#define VOS_MTX_LOCK(pstMtx)               EnterCriticalSection(pstMtx)
#define VOS_MTX_UNLOCK(pstMtx)             LeaveCriticalSection(pstMtx)
#define VPS_MTX_TRYLOCK(pstMtx)            (TryEnterCriticalSection(pstMtx) ? VOS_TRUE : VOS_ERR)

#elif VOS_PLAT_LINUX
typedef pthread_mutex_t VOS_MTX_T;
#define VOS_MTX_INIT(pstMtx)                (pthread_mutex_init(pstMtx, 0) ? VOS_ERR : VOS_OK)
#define VOS_MTX_FINI(pstMtx)                (pthread_mutex_destroy(pstMtx) ? VOS_ERR : VOS_OK)
#define VOS_MTX_LOCK(pstMtx)                pthread_mutex_lock(pstMtx)
#define VOS_MTX_UNLOCK(pstMtx)              pthread_mutex_unlock(pstMtx)
#define VPS_MTX_TRYLOCK(pstMtx)             (pthread_mutex_trylock(pstMtx) ? VOS_ERR : VOS_OK)
#endif 


#endif
