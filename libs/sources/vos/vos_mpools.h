/******************************************************************************

                  版权所有 (C), 2018-2028, 网络有限公司

 ******************************************************************************
  文 件 名   : vos_mpools.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年4月29日
  最近修改   :
  功能描述   : 动态内存池的设计
  函数列表   :
  修改历史   :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#ifndef _VOS_MPOOLS_H_
#define _VOS_MPOOLS_H_

/* 默认创建的队列长度*/ 
#define     VOS_LOOKSIDE_NUMS           512
/*单个池的最大个数*/
#define     VOS_LOOKSIDE_MAXNUMS        102400



/* 内存池的设计 */
typedef struct tagVOSMemoryPoolList
{   
    uint32_t            uiEntrySize;        /*该内池的内存大小*/
    
    VOS_MUTEX_T         stPoolListLock;     /*该内存池的锁*/
    
    VOS_DLIST_S         stPoolList;         /*空闲的存放*/
    
    VINT32_T            uiNums;             /*当前该池中的数量*/
    
}VOS_MemPool_List, *PVOS_MemPool_List;


int32_t    VOS_InitializeVirtualMemoryPool(PVOS_MemPool_List *ppstLookSideList,uint32_t EntrySize);

VOID    VOS_UnInitializeVirtualMemoryPool(PVOS_MemPool_List *ppstLookSideList);

VOID*   VOS_AllcatePoolWithTag(PVOS_MemPool_List pstLookSideList, uint32_t mem_tag);

VOID    VOS_FreePool(VOID *pvEntry);




#endif
