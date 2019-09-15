/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_mpools.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��4��29��
  ����޸�   :
  ��������   : ��̬�ڴ�ص����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos/vos_pub.h>


/*****************************************************************************
 �� �� ��  : VOS_InitializeVirtualMemoryPool
 ��������  : ��ʼ��һ���ڴ��,Ĭ�ϴ���100���ڴ�
 �������  : VOS_PLookSideList **ppstLookSideList  
             UINT32 uiEntrySize                    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

    /*����VOS���ڴ���*/
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
    
    /*��ʼ�½��ڴ��*/
    for(uiIndex=0; uiIndex < VOS_LOOKSIDE_NUMS; uiIndex++ )
    {
        /*ÿ��ͷ���ض���VOS_DLIST_NODE_S*/
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
 �� �� ��  : VOS_UnInitializeVirtualMemoryPool
 ��������  : �ڴ�ص��ͷ�
 �������  : VOS_PLookSideList *pstLookSideList  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_AllcateFromLookSideList
 ��������  : �����ڴ�
 �������  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 �������  : ��
 �� �� ֵ  : VOID
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

    /*ÿ��ͷ���ض���VOS_DLIST_NODE_S*/
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
 �� �� ��  : VOS_FreeToLookSideList
 ��������  : �ͷ��ڴ�
 �������  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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




