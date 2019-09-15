/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_mm.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��17��
  ����޸�   :
  ��������   : �ڴ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��17��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#if 0
/*Ŀǰҵ��Ͷ�����2K,4K,8K,16K��������չ*/
#define VOS_MEM_CONNNUM          1500

#define VOS_MEM_32_NUM           VOS_MEM_CONNNUM
#define VOS_MEM_64_NUM           VOS_MEM_CONNNUM
#define VOS_MEM_128_NUM          VOS_MEM_CONNNUM
#define VOS_MEM_256_NUM          VOS_MEM_CONNNUM
#define VOS_MEM_512_NUM          VOS_MEM_CONNNUM
#define VOS_MEM_1K_NUM           VOS_MEM_CONNNUM
#define VOS_MEM_2K_NUM           10                 /*��ǰҵ���ϲ����õ�*/
#define VOS_MEM_4K_NUM           10                 /*��ǰҵ���ϲ����õ�*/
#define VOS_MEM_8K_NUM           10                 /*��ǰҵ���ϲ����õ�*/
#define VOS_MEM_16K_NUM          10000               /*ҵ�����ʽ��16K��Ϊ��, ÿ���û����20����������, ���200������*/
#define VOS_MEM_32K_NUM          10               
#define VOS_MEM_64K_NUM          10
#define VOS_MEM_128K_NUM         10


#define VOS_CHAR_SIZE                   sizeof(CHAR) 

#define VOS_FILEPATH_MAXLEN       256

#define VOS_MEM_FREED       0
#define VOS_MEM_USED         1

#define VOS_Malloc(ulMid, ulSize)  VOS_MemCreate_X(ulMid, ulSize,__FILE__,__LINE__)

#define VOS_Free(pcMemBuf)    VOS_MemFree_X(pcMemBuf)

INT32 VOS_MemMgmt_Init();

VOID  VOS_MemMgmt_UnInit();

VOID* VOS_MemCreate_X(ULONG ulMid, UINT32 ulBufSize, const CHAR *pcFileName, INT32 ulLine);

VOID  VOS_MemFree_X(CHAR *pcMemBuf);

VOID  VOS_MM_MidTypeShow(UINT32 uiType,  UINT32 uiMid);

VOID  VOS_MM_TypeShow(UINT32 uiType);

VOID  VOS_MM_Show();

#endif
