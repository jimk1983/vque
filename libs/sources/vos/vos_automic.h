#ifndef _VOS_AUTOMIC_H_
#define _VOS_AUTOMIC_H_


#if VOS_PLAT_WIN
#ifndef HAVE_INTTYPES
#include "inttypes.h"
#endif
typedef volatile long   vatm32_t;
typedef volatile __int64 vatmv64_t;
typedef long* vatm_t;
typedef __int64* vatm64_t;
#else /* VSYS_LINUX(POSIX) */
typedef volatile int32_t vatm32_t;
typedef volatile int64_t vatmv64_t;
typedef int32_t *vatm_t;
typedef int64_t *vatm64_t;
#endif /* VSYS_WINDOWS */


#if     VOS_PLAT_WIN
#define vmem_align(alignment, size) _aligned_malloc(size,alignment)
#define vmem_align64(size) _aligned_malloc(size, 64)
#define vmem_align_free(ptr) _aligned_free(ptr)
#else // VSYS_POSIX
#define vmem_align(alignment, size) memalign(alignment, size)
#define vmem_align64(size) memalign(64, size)
#define vmem_align_free(ptr) free(ptr)
#endif

#if     VOS_PLAT_LINUX
/* gcc */
#define vatm_alloc(size) memalign(64, size)
#define vatm_alloc_atm() (vatm_t)memalign(64, sizeof(vatm32_t))
#define vatm_alloc_atm64() (vatm64_t)memalign(64, sizeof(vatmv64_t))
#define vatm_free(ptr) free(ptr)

#define vatm_add(ptr, value) __sync_add_and_fetch(ptr, value)
#define vatm_sub(ptr, value) __sync_sub_and_fetch(ptr, value)
#define vatm_inc(ptr) __sync_add_and_fetch(ptr, 1)
#define vatm_dec(ptr) __sync_sub_and_fetch(ptr, 1)
#define vatm_cas(ptr, oldval, newval) __sync_val_compare_and_swap(ptr,oldval,newval)
#define vatm_bcas(ptr, oldval, newval) __sync_bool_compare_and_swap(ptr,oldval,newval)
#define vatm_xchg(ptr, newval) __sync_lock_test_and_set (ptr, newval);
#define vatm_xchg_ptr(ptr, newval) __sync_lock_test_and_set (ptr, newval);

#define vatm_barrier() __sync_synchronize()

#define vatm64_add(ptr, value) __sync_add_and_fetch(ptr, value)
#define vatm64_sub(ptr, value) __sync_sub_and_fetch(ptr, value)
#define vatm64_inc(ptr) __sync_add_and_fetch(ptr, 1)
#define vatm64_dec(ptr) __sync_sub_and_fetch(ptr, 1)
#define vatm64_cas(ptr, oldval, newval) __sync_val_compare_and_swap(type *ptr,oldval,newval)
#define vatm64_xchg(ptr, newval) __sync_lock_test_and_set (ptr, newval);
#define vatm64_xchg_ptr(ptr, newval) __sync_lock_test_and_set (ptr, newval);

#elif VOS_PLAT_WIN

#define vatm_alloc(size) _aligned_malloc(size, 64)
#define vatm_alloc_atm() (vatm_t)_aligned_malloc(sizeof(vatm32_t), 64)
#define vatm_alloc_atm64() (vatm64_t)_aligned_malloc(sizeof(vatmv64_t), 64)
#define vatm_free(ptr) _aligned_free(ptr)
#define vatm_add(ptr, value) InterlockedExchangeAdd(ptr, value)
#define vatm_sub(ptr, value) InterlockedExchangeAdd(ptr, -(value))
#define vatm_inc(ptr) InterlockedIncrement(ptr)
#define vatm_dec(ptr) InterlockedDecrement(ptr)
#define vatm_cas(ptr, oldval, newval) InterlockedCompareExchange(ptr, newval, oldval)
#define vatm_bcas(ptr, oldval, newval) (oldval == InterlockedCompareExchange(ptr, newval, oldval))
#define vatm_xchg(ptr, newval) InterlockedExchange(ptr, newval)
#define vatm_xchg_ptr(ptr, newval) InterlockedExchangePointer((volatile PVOID*)ptr, (PVOID)newval)

#define vatm64_add(ptr, value) InterlockedExchangeAdd64(ptr, value)
#define vatm64_sub(ptr, value) InterlockedExchangeAdd64(ptr, -(value))
#define vatm64_inc(ptr) InterlockedIncrement64(ptr)
#define vatm64_dec(ptr) InterlockedDecrement64(ptr)
#define vatm64_cas(ptr, oldval, newval) InterlockedCompareExchange64(ptr, newval, oldval)
#define vatm64_xchg(ptr, newval) InterlockedExchange64(ptr, newval)
#define vatm_barrier()

#else // some embend code, no threads

#define vatm_alloc(size) cmalloc(1, size)
#define vatm_free(ptr) free(ptr)
#define vatm_add(ptr, value) (*ptr += value)
#define vatm_sub(ptr, value) (*ptr -= value)
#define vatm_inc(ptr) (++(*ptr))
#define vatm_dec(ptr) (--(*ptr))
#define vatm_cas(ptr, oldval, newval) (*ptr == oldval) ? *ptr = newval : oldval;
#define vatm_xchg(ptr, newval) do{ptr ^= newval; newval ^= ptr; ptr ^= newval;}while(0)
#define vatm_xchg_ptr(ptr, newval) do{ptr ^= newval; newval ^= ptr; ptr ^= newval;}while(0)
#define vatm_barrier()
#endif

#endif
