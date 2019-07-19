/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_dlist.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年1月10日
  最近修改   :
  功能描述   : VOS的DLIST操作
  函数列表   :
  修改历史   :
  1.日    期   : 2016年1月10日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vos_dlist.h
 * @brief define list operations
 * @author jiangkang 
 * @date 2019-07
 */

#include <vos/vos_pub.h>


/**
 * @brief check list is empty
 * @param list [in] the list head pointer
 */
inline BOOL_T VOS_DList_IsEmpty_s( IN const VOS_DLIST_S *pListHead)
{
    if ( NULL == pListHead )
    {
        return VOS_FALSE;
    }
    
    return (BOOL_T)(pListHead->prev == pListHead);
}

/**
 * @brief remove the entry from list
 * @param list [in] the list entry pointer
 * @retval VOS_TRUE has removed successful.
 */
inline BOOL_T VOS_DList_RemoveEntry_s( IN VOS_DLIST_S *pListEntry)
{
    PVOS_DLIST_S next;
    PVOS_DLIST_S prev;
    
    if ( NULL == pListEntry )
    {
        return VOS_FALSE;
    }
    
    prev = pListEntry->prev;
    next = pListEntry->next;
    next->prev = prev;
    prev->next = next;
    return (BOOL_T)(prev == next);
}


/**
 * @brief check the entry is correct
 * @param list [in] the list entry pointer
 * @retval VOS_TRUE has removed successful.
 */
inline BOOL_T VOS_DList_CheckEntry_s( IN VOS_DLIST_S *pListEntry)
{
    if ( NULL == pListEntry )
    {
        return VOS_FALSE;
    }
    
    if ((((pListEntry->prev)->next) != pListEntry) || (((pListEntry->next)->prev) != pListEntry)) 
    {
        VOS_ASSERT(0);
        return VOS_FALSE;
    }
    
    return VOS_TRUE;
}

/**
 * @brief remove the tail form the list
 * @param list [in] the list head pointer
 * @retval return the tail entry 
 */
inline PVOS_DLIST_S VOS_DList_RemoveTail_s( INOUT PVOS_DLIST_S pListHead )
{
    PVOS_DLIST_S prev;
    PVOS_DLIST_S Entry;

    if ( VOS_FALSE == VOS_DList_CheckEntry_s(pListHead) )
    {
        return NULL;
    }
    
    Entry = pListHead->prev;
    prev = Entry->prev;
    pListHead->prev = prev;
    prev->next = pListHead;
    return Entry;
}


/**
 * @brief remove the tail form the list
 * @param list [in] the list head pointer
 * @retval return the head entry 
 */
inline PVOS_DLIST_S  VOS_DList_RemoveHead_s(      INOUT PVOS_DLIST_S pListHead )
{
    PVOS_DLIST_S Blink;
    PVOS_DLIST_S Entry;
    
    if ( VOS_FALSE == VOS_DList_CheckEntry_s(pListHead) )
    {
        return NULL;
    }
    
    Entry = pListHead->next;
    Blink = Entry->next;
    pListHead->next = Blink;
    Blink->prev = pListHead;
    return Entry;
}


/**
 * @brief insert the entry to the head->next
 * @param list [inout] the list head pointer
 * @param pEntry [inout] the insert entry
 * @retval return none
 */
inline VOID VOS_DList_InsertHead_s( INOUT PVOS_DLIST_S pListHead, INOUT PVOS_DLIST_S pEntry )
{
    PVOS_DLIST_S next;

    if ( VOS_FALSE == VOS_DList_CheckEntry_s(pListHead)
        || VOS_FALSE == VOS_DList_CheckEntry_s(pEntry) )
    {
        return;
    }
    
    next = pListHead->next;
    pEntry->prev = pListHead;
    pEntry->next = next;
    next->prev = pEntry;
    pListHead->next = pEntry;
    return;
}

/**
 * @brief insert the entry to the head->prev
 * @param list [inout] the list head pointer
 * @param pEntry [inout] the insert entry
 * @retval return none
 */
inline VOID VOS_DList_InsertTail_s( INOUT PVOS_DLIST_S pListHead, INOUT PVOS_DLIST_S pEntry )
{
    PVOS_DLIST_S prev;
    
    if ( VOS_FALSE == VOS_DList_CheckEntry_s(pListHead)
        || VOS_FALSE == VOS_DList_CheckEntry_s(pEntry) )
    {
        return;
    }
    
    prev = pListHead->prev;
    pEntry->prev = prev;
    pEntry->next = pListHead;
    prev->next = pEntry;
    pListHead->prev = pEntry;
    return;
}




