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
#ifndef _VOS_MPOOLS_H_
#define _VOS_MPOOLS_H_

/* Ĭ�ϴ����Ķ��г���*/ 
#define     VOS_LOOKSIDE_NUMS           512
/*�����ص�������*/
#define     VOS_LOOKSIDE_MAXNUMS        102400



/* �ڴ�ص���� */
typedef struct tagVOSMemoryPoolList
{   
    uint32_t            uiEntrySize;        /*���ڳص��ڴ��С*/
    
    VOS_MUTEX_T         stPoolListLock;     /*���ڴ�ص���*/
    
    VOS_DLIST_S         stPoolList;         /*���еĴ��*/
    
    VINT32_T            uiNums;             /*��ǰ�ó��е�����*/
    
}VOS_MemPool_List, *PVOS_MemPool_List;


int32_t    VOS_InitializeVirtualMemoryPool(PVOS_MemPool_List *ppstLookSideList,uint32_t EntrySize);

VOID    VOS_UnInitializeVirtualMemoryPool(PVOS_MemPool_List *ppstLookSideList);

VOID*   VOS_AllcatePoolWithTag(PVOS_MemPool_List pstLookSideList, uint32_t mem_tag);

VOID    VOS_FreePool(VOID *pvEntry);




#endif
