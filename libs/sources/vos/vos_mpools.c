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
#include <vos/vos_pub.h>


/*****************************************************************************
 函 数 名  : VOS_InitializeVirtualMemoryPool
 功能描述  : 初始化一个内存池,默认创建100个内存
 输入参数  : VOS_PLookSideList **ppstLookSideList  
             UINT32 uiEntrySize                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
int32_t VOS_InitializeVirtualMemoryPool(PVOS_MemPool_List *ppstLookSideList,uint32_t EntrySize)
{
    PVOS_MemPool_List   pstLooksidelist = NULL;
    VOS_DLIST_S*        pstEntry        = NULL;
    uint32_t            uiIndex         = 0;
    
    if ( NULL == ppstLookSideList )
    {
        return VOS_ERR;
    }

    /*不在VOS的内存中*/
    pstLooksidelist = (PVOS_MemPool_List)malloc(sizeof(VOS_MemPool_List));
    if ( NULL == pstLooksidelist )
    {
        PError("vos lookside list malloc error!\n");
        return VOS_ERR;
    }
    memset(pstLooksidelist,0, sizeof(VOS_MemPool_List));
    
    VOS_MUTEX_INIT(pstLooksidelist->stPoolListLock);
    VOS_DLIST_INIT(&pstLooksidelist->stPoolList);
    
    pstLooksidelist->uiEntrySize = EntrySize;
    
    /*开始新建内存池*/
    for(uiIndex=0; uiIndex < VOS_LOOKSIDE_NUMS; uiIndex++ )
    {
        /*每个头部必定是VOS_DLIST_NODE_S*/
        pstEntry = (VOS_DLIST_S *)malloc(sizeof(VOS_DLIST_S) + EntrySize);
        if ( NULL == pstEntry )
        {   
            PError("vos lookside entry malloc error!\n");
            goto ErrHandle;
        }
        
        VOS_DLIST_INIT(pstEntry);
        VOS_DLIST_ADD_TAIL(&pstLooksidelist->stPoolList, pstEntry);
        pstLooksidelist->uiNums++;
    }

    *ppstLookSideList = pstLooksidelist;

    return VOS_OK;
    
ErrHandle:

    VOS_UnInitializeVirtualMemoryPool(&pstLooksidelist);
    return VOS_ERR;
}


/*****************************************************************************
 函 数 名  : VOS_UnInitializeVirtualMemoryPool
 功能描述  : 内存池的释放
 输入参数  : VOS_PLookSideList *pstLookSideList  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_UnInitializeVirtualMemoryPool(PVOS_MemPool_List *ppstLookSideList)
{
    PVOS_DLIST_S        thisEntry, nextEntry, listHead;
    PVOS_MemPool_List   pstLookSideList = *ppstLookSideList;
    
    if ( NULL != pstLookSideList )
    {
        listHead = &pstLookSideList->stPoolList;
        for (   thisEntry = listHead->prev;
                thisEntry != listHead ;
                thisEntry = nextEntry)
        {
            nextEntry = thisEntry->prev;
            free(thisEntry);
            pstLookSideList->uiNums--;
        }

        free(pstLookSideList);
        pstLookSideList = NULL;
    }
    
    PEvent("vos memory pool nums=%d", pstLookSideList->uiNums);
    
    return;
}

/*****************************************************************************
 函 数 名  : VOS_AllcateFromLookSideList
 功能描述  : 申请内存
 输入参数  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 输出参数  : 无
 返 回 值  : VOID
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID *VOS_AllcatePoolWithTag(PVOS_MemPool_List pstLookSideList, uint32_t mem_tag)
{
    VOS_DLIST_S *pstEntry  = NULL;
    VOID             *pvMem     = NULL;

    VOS_MUTEX_LOCK(pstLookSideList->stPoolListLock);
    if ( VOS_OK != VOS_DList_IsEmpty(&pstLookSideList->stPoolList) )
    {
        pstEntry =  pstLookSideList->stPoolList.prev;
        VOS_DLIST_DEL(pstEntry);
        VOS_MUTEX_UNLOCK(pstLookSideList->stPoolListLock);
        
        pvMem = (CHAR *)pstEntry + sizeof(VOS_DLIST_S);
        return pvMem;
    }
    VOS_MUTEX_UNLOCK(pstLookSideList->stPoolListLock);
    
    if ( VOS_LOOKSIDE_MAXNUMS >= pstLookSideList->uiNums )
    {
        PError("vos pool exceed max nums!");
        return NULL;
    }

    /*每个头部必定是VOS_DLIST_NODE_S*/
    pstEntry = (VOS_DLIST_S *)malloc(sizeof(VOS_DLIST_S) + pstLookSideList->uiEntrySize);
    if ( NULL == pstEntry )
    {
        PError("vos entry malloc error!");
        return NULL;
    }
    
    memset(pstEntry,0, sizeof(VOS_DLIST_S) + pstLookSideList->uiEntrySize);
    
    VOS_DLIST_INIT(pstEntry);
    
    pvMem = (CHAR *)pstEntry + sizeof(VOS_DLIST_S);
    pstLookSideList->uiNums++;
    
    return pvMem;
}



/*****************************************************************************
 函 数 名  : VOS_FreeToLookSideList
 功能描述  : 释放内存
 输入参数  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID  VOS_FreePool(PVOS_MemPool_List pstLookSideList, VOID *pvEntry)
{
    VOS_DLIST_S*    pstEntry  = NULL;

    if ( NULL != pvEntry )
    {
          pstEntry = (VOS_DLIST_S *)((CHAR *)pvEntry - sizeof(VOS_DLIST_S));
          VOS_DLIST_INIT(pstEntry);
          VOS_MUTEX_LOCK(pstLookSideList->stPoolListLock);
          VOS_DLIST_ADD_TAIL(&pstLookSideList->stPoolList, pstEntry);
          VOS_MUTEX_UNLOCK(pstLookSideList->stPoolListLock);
    }
    
    return;
}




