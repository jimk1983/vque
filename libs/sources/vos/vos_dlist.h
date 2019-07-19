/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_dlist.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��10��
  ����޸�   :
  ��������   : VOS��DLIST����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��10��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOS_DLIST_H_
#define _VOS_DLIST_H_

typedef struct tagVosDoubleList
{
    struct tagVosDoubleList *prev;
    struct tagVosDoubleList *next;
}VOS_DLIST_S, *PVOS_DLIST_S;

#ifndef container_of
#define container_of(ptr, type, member) \
        (type *)((char *)(ptr) - (char *) &((type *)0)->member)
#endif

/*���ݳ�Ա����������ȡ�ڵ��׵�ַ*/
#define VOS_DLIST_ENTRY(pnode,type,membernode) \
         container_of(pnode,type,membernode)


/*�����������еĽڵ�,�м�ڵ㲻�ܽ��������Ķ���*/
#define VOS_DLIST_FOR_EACH_ENTRY(pos, pstList, type, stnode)\
     for(pos = VOS_DLIST_ENTRY((pstList)->next, type, stnode);\
         &pos->stnode != (pstList);\
         pos =VOS_DLIST_ENTRY(pos->stnode.next, type, stnode))\

/*��ʼ��*/
#define VOS_DLIST_INIT(pstNode) \
do{\
    if ( NULL != (pstNode) )\
    {\
      (pstNode)->prev = (pstNode);\
      (pstNode)->next = (pstNode);\
    }\
}while(0)

/*��ӽڵ㵽����ͷ*/
#define VOS_DLIST_ADD_HEAD(pstList, pstNode)\
{\
    if( (NULL != (pstList)) && (NULL != (pstNode)) )\
    {\
         (pstNode)->next = (pstList)->next;\
         (pstNode)->prev = (pstList);\
         (pstList)->next->prev = (pstNode);\
         (pstList)->next = (pstNode);\
    }\
}

/*��ӽڵ㵽����β*/
#define VOS_DLIST_ADD_TAIL(pstList,pstNode)\
{\
    if( (NULL != (pstList)) && (NULL != (pstNode)) )\
    {\
     (pstNode)->next = (pstList);\
     (pstNode)->prev = (pstList)->prev;\
     (pstList)->prev->next = (pstNode);\
     (pstList)->prev = (pstNode);\
    }\
}

/*��������ժ�����ڵ�*/
#define VOS_DLIST_DEL(pstNode)\
{\
    if(NULL != (pstNode))\
    {\
        (pstNode)->next->prev = (pstNode)->prev;\
        (pstNode)->prev->next = (pstNode)->next;\
        (pstNode)->next = (pstNode);\
        (pstNode)->prev = (pstNode);\
    }\
}

/*�ƶ�����ͷ����������*/
#define VOS_DLIST_MOVE(pstListFrom, pstListTo)\
{\
    if ( (NULL != (pstListFrom)) && (NULL != (pstListTo)) )\
    {\
        (pstListTo)->next = (pstListFrom)->next;\
        (pstListFrom)->next->prev = (pstListTo);\
        (pstListFrom)->prev->next = (pstListTo);\
        (pstListTo)->prev = (pstListFrom)->prev;\
        (pstListFrom)->next = (pstListFrom);\
        (pstListFrom)->prev = (pstListFrom);\
    }\
}

/**
 * @brief check list is empty
 * @param list [in] the list head pointer
 */
CINLINE BOOL_T VOS_DList_IsEmpty( IN const VOS_DLIST_S *pListHead)
{
    return (BOOL_T)(pListHead->prev == pListHead);
}

/**
 * @brief remove the entry from list
 * @param list [in] the list entry pointer
 * @retval VOS_TRUE has removed successful.
 */
CINLINE BOOL_T VOS_DList_RemoveEntry( IN VOS_DLIST_S *pListEntry)
{
    PVOS_DLIST_S next;
    PVOS_DLIST_S prev;

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
CINLINE BOOL_T VOS_DList_CheckEntry( IN VOS_DLIST_S *pListEntry)
{
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
CINLINE PVOS_DLIST_S VOS_DList_RemoveTail( INOUT PVOS_DLIST_S pListHead )
{
    PVOS_DLIST_S prev;
    PVOS_DLIST_S Entry;

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
CINLINE PVOS_DLIST_S  VOS_DList_RemoveHead(      INOUT PVOS_DLIST_S pListHead )
{
    PVOS_DLIST_S Blink;
    PVOS_DLIST_S Entry;

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
CINLINE VOID VOS_DList_InsertHead( INOUT PVOS_DLIST_S pListHead, INOUT PVOS_DLIST_S pEntry )
{
    PVOS_DLIST_S next;
    
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
CINLINE VOID VOS_DList_InsertTail( INOUT PVOS_DLIST_S pListHead, INOUT PVOS_DLIST_S pEntry )
{
    PVOS_DLIST_S prev;
    
    prev = pListHead->prev;
    pEntry->prev = prev;
    pEntry->next = pListHead;
    prev->next = pEntry;
    pListHead->prev = pEntry;
    return;
}


inline BOOL_T VOS_DList_IsEmpty_s( IN const VOS_DLIST_S *pListHead);
inline BOOL_T VOS_DList_RemoveEntry_s( IN VOS_DLIST_S *pListEntry);
inline BOOL_T VOS_DList_CheckEntry_s( IN VOS_DLIST_S *pListEntry);

inline PVOS_DLIST_S VOS_DList_RemoveTail_s( INOUT PVOS_DLIST_S pListHead );
inline PVOS_DLIST_S  VOS_DList_RemoveHead_s(      INOUT PVOS_DLIST_S pListHead );

inline VOID VOS_DList_InsertHead_s( INOUT PVOS_DLIST_S pListHead, INOUT PVOS_DLIST_S pEntry );
inline VOID VOS_DList_InsertTail_s( INOUT PVOS_DLIST_S pListHead, INOUT PVOS_DLIST_S pEntry );


#endif
