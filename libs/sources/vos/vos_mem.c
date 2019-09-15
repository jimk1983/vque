/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_mm.c
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
#include <vos_pub.h>

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
    
/*�ڴ浥Ԫ�Ĵ�С*/
#define VOS_MEM_UNIT_SIZE       sizeof(VOS_MEMBUF_UNIT_S)
    
    /*���ݽڵ��ȡ��Ա��ַ*/
#define VOS_MEM_UNIT_ENTRY(plist,type,membernode) \
        container_of(plist,type,membernode)

/*�ڴ������*/
typedef struct tagVOSMemMgmt    VOS_MEMBUF_MGMT_S;

typedef struct tagVosMemUnit      VOS_MEMBUF_UNIT_S;

/*���ڴ���ħ��*/
#define VOS_MGMT_MAGIC_HEAD1        0xAACCBBDD
#define VOS_MGMT_MAGIC_HEAD2        0xEEBBFFAA

#define VOS_MEM_TYPE_32        1
#define VOS_MEM_TYPE_64        2
#define VOS_MEM_TYPE_128       3
#define VOS_MEM_TYPE_256       4
#define VOS_MEM_TYPE_512       5
#define VOS_MEM_TYPE_1K        6
#define VOS_MEM_TYPE_2K        7
#define VOS_MEM_TYPE_4K        8
#define VOS_MEM_TYPE_8K        9
#define VOS_MEM_TYPE_16K       10
#define VOS_MEM_TYPE_32K       11
#define VOS_MEM_TYPE_64K       12
#define VOS_MEM_TYPE_128K      13
#define VOS_MEM_TYPE_256K      14
#define VOS_MEM_TYPE_512K      15

#define VOS_MEM_TYPE_SYS       16


#define VOS_MEM_SIZE_32         32
#define VOS_MEM_SIZE_64         64
#define VOS_MEM_SIZE_128        128
#define VOS_MEM_SIZE_256        256
#define VOS_MEM_SIZE_512        512
#define VOS_MEM_SIZE_1K         1024
#define VOS_MEM_SIZE_2K         2048
#define VOS_MEM_SIZE_4K         4096
#define VOS_MEM_SIZE_8K         8192    
#define VOS_MEM_SIZE_16K        16384+256
#define VOS_MEM_SIZE_32K        32768
#define VOS_MEM_SIZE_64K        65536
#define VOS_MEM_SIZE_128K       131072
#define VOS_MEM_SIZE_256K       262144
#define VOS_MEM_SIZE_512K       524288



/*���Է����������ڴ����й���*/
struct tagVOSMemMgmt
{
    /*��ʼ���ڴ��*/
    CHAR *pcBlk1;

    /*��ʼ���ڴ��1*/
    CHAR *pcBlk2;

    /*�ڴ���߳���*/
    VOID *pvThreadLock;

    /*����32��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_32;
    /*����32��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_32;
    UINT32 ulFNum_32;    /*������е��ڴ���*/
    UINT32 ulUNum_32;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����64��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_64;
    /*����64��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_64;
    UINT32 ulFNum_64;    /*������е��ڴ���*/
    UINT32 ulUNum_64;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����128��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_128;
    /*����128��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_128;
    UINT32 ulFNum_128;    /*������е��ڴ���*/
    UINT32 ulUNum_128;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����256��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_256;
    /*����256��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_256;
    UINT32 ulFNum_256;    /*������е��ڴ���*/
    UINT32 ulUNum_256;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����512��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_512;
    /*����512��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_512;
    UINT32 ulFNum_512;    /*������е��ڴ���*/
    UINT32 ulUNum_512;    /*�����Ѿ�ʹ�õ��ڴ���*/


    /*����1024��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_1K;
    /*����1024��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_1K;
    UINT32 ulFNum_1K;    /*������е��ڴ���*/
    UINT32 ulUNum_1K;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����2048��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_2K;
    /*����2048��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_2K;
    UINT32 ulFNum_2K;    /*������е��ڴ���*/
    UINT32 ulUNum_2K;    /*�����Ѿ�ʹ�õ��ڴ���*/


    /*����4096��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_4K;
    /*����4096��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_4K;
    UINT32 ulFNum_4K;    /*������е��ڴ���*/
    UINT32 ulUNum_4K;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����8192��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_8K;
    /*����8192��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_8K;
    UINT32 ulFNum_8K;    /*������е��ڴ���*/
    UINT32 ulUNum_8K;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����16384��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_16K;
    /*����16384��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_16K;
    UINT32 ulFNum_16K;    /*������е��ڴ���*/
    UINT32 ulUNum_16K;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����32K��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_32K;
    /*����32K��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_32K;
    UINT32 ulFNum_32K;    /*������е��ڴ���*/
    UINT32 ulUNum_32K;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����64K��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_64K;
    /*����64K��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_64K;
    UINT32 ulFNum_64K;    /*������е��ڴ���*/
    UINT32 ulUNum_64K;    /*�����Ѿ�ʹ�õ��ڴ���*/
    
    /*����128K��С�Ŀ�������*/
    VOS_DLIST_NODE_S stMMListF_128K;
    /*����128K��С��ʹ������*/
    VOS_DLIST_NODE_S stMMListU_128K;
    UINT32 ulFNum_128K;    /*������е��ڴ���*/
    UINT32 ulUNum_128K;    /*�����Ѿ�ʹ�õ��ڴ���*/

    /*����128K��ϵͳ����, ʹ��ϵͳ��malloc������*/
    VOS_DLIST_NODE_S stMMList_system;
    UINT32 ulUNum_system;    /*�����Ѿ�ʹ�õ��ڴ���*/
};

/*�����Ľṹ��*/
struct tagVosMemUnit
{
    VOS_DLIST_NODE_S stNode;
    UINT32 ulMagicHead1;          /*ħ��ͷ1����ֹ�ڴ�Խ��*/
    UINT32 ulMagicHead2;          /*ħ��ͷ2����ֹ�ڴ�Խ��*/
    UINT32 ulMid;                      /*ģ���module ID�ţ���������ڴ�й¶/ʹ�������λ*/
    UINT32 ulIdle;                      /*�Ƿ��Ѿ����ڿ���,0:����1:�ǿ���*/
    CHAR    acFileName[VOS_FILEPATH_MAXLEN];            /*���ڴ������ļ�*/
    UINT32 ulLineNum;              /*���ڴ����������ļ��к�*/
    UINT32 ulBufType;              /*�ͷ��ڴ��ʱ����Ҫ�õ�����������*/
    CHAR    ucmmbuf[0];           /*���ڴ����ݵ��׵�ַ*/
};

/*�����ڴ������*/
static VOS_MEMBUF_MGMT_S  g_stmmMgmt;


/*�����ڴ�������Ϊ�Ǵ�ͬһ���ڴ����������ڴ�*/
VOS_RW_LOCK_S g_stmmLock32;
VOS_RW_LOCK_S g_stmmLock64;
VOS_RW_LOCK_S g_stmmLock128;
VOS_RW_LOCK_S g_stmmLock256;
VOS_RW_LOCK_S g_stmmLock512;
VOS_RW_LOCK_S g_stmmLock1K;
VOS_RW_LOCK_S g_stmmLock2K;
VOS_RW_LOCK_S g_stmmLock4K;
VOS_RW_LOCK_S g_stmmLock8K;
VOS_RW_LOCK_S g_stmmLock16K;
VOS_RW_LOCK_S g_stmmLock32K;
VOS_RW_LOCK_S g_stmmLock64K;
VOS_RW_LOCK_S g_stmmLock128K;

/*���Ᵽ����1M��С���ڴ�,��ֹ��ʼ���ڴ����*/
#define VOS_MEMORY_RESV      1024 * 1024

/*�ڴ��������ʼ��*/
INT32 VOS_MemMgmt_Init()
{  
    /*���������һ����ڴ�Ҳ���ԣ�����Ļ�����֤�����༴��
     ��Ҫ������һ�����������ڴ���ܻ�ʧ��*/
    UINT32 ulBufSum1    = 0;
    UINT32 ulBufSum2    = 0;
    CHAR   *pcMmOffset  = NULL;
    UINT32 ulIndex        = 0;
    UINT32 ulSizeChar     = VOS_CHAR_SIZE;
    //CHAR   *pcTmpOff    = NULL;
    VOS_MEMBUF_UNIT_S *pstMmEntry = NULL;

    /*С�ֽ������Ƚ϶�, ���ڵ�һ���ڴ���*/
    ulBufSum1  = VOS_MEM_32_NUM  * ( (VOS_MEM_SIZE_32+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_64_NUM  * ( (VOS_MEM_SIZE_64+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_128_NUM * ( (VOS_MEM_SIZE_128+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_256_NUM * ( (VOS_MEM_SIZE_256+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_512_NUM * ( (VOS_MEM_SIZE_512+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);

    /*1K�Ժ�ķ�������һ��*/
    ulBufSum2 =   VOS_MEM_1K_NUM  * ( (VOS_MEM_SIZE_1K+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_2K_NUM  * ( (VOS_MEM_SIZE_2K+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_4K_NUM  * ( (VOS_MEM_SIZE_4K+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_8K_NUM  * ( (VOS_MEM_SIZE_8K+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_16K_NUM * ( (VOS_MEM_SIZE_16K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_32K_NUM * ( (VOS_MEM_SIZE_32K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_64K_NUM * ( (VOS_MEM_SIZE_64K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_128K_NUM* ( (VOS_MEM_SIZE_128K+1)* VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);

    
    /*���߳��ڴ�����ʼ��*/
    VOS_ThreadMutex_Init(&g_stmmLock32);
    VOS_ThreadMutex_Init(&g_stmmLock64);
    VOS_ThreadMutex_Init(&g_stmmLock128);
    VOS_ThreadMutex_Init(&g_stmmLock256);
    VOS_ThreadMutex_Init(&g_stmmLock512);
    VOS_ThreadMutex_Init(&g_stmmLock1K);
    VOS_ThreadMutex_Init(&g_stmmLock2K);
    VOS_ThreadMutex_Init(&g_stmmLock4K);
    VOS_ThreadMutex_Init(&g_stmmLock8K);
    VOS_ThreadMutex_Init(&g_stmmLock16K);
    VOS_ThreadMutex_Init(&g_stmmLock32K);
    VOS_ThreadMutex_Init(&g_stmmLock64K);
    VOS_ThreadMutex_Init(&g_stmmLock128K);
    
    /*�ڴ������*/
    VOS_Mem_Zero((CHAR *)&g_stmmMgmt, sizeof(g_stmmMgmt));
    
    /*1.�����ڴ�*/
    g_stmmMgmt.pcBlk1 = (CHAR *)VOS_Memory(ulBufSum1 + VOS_MEMORY_RESV);
    if ( NULL == g_stmmMgmt.pcBlk1 )
    {
        VOS_Printf("g_stmmMgmt.ucBlk1 malloc error!ulBufSum1=%d\n", ulBufSum1);
        goto ErrHandle;
    }
    else
    {
        VOS_Mem_Zero(g_stmmMgmt.pcBlk1, ulBufSum1 );
    }
    
    g_stmmMgmt.pcBlk2 = (CHAR *)VOS_Memory(ulBufSum2 + VOS_MEMORY_RESV);
    if ( NULL == g_stmmMgmt.pcBlk2 )
    {
        VOS_Printf("g_stmmMgmt.ucBlk2  malloc error!\n");
        goto ErrHandle;
    }
    else
    {
        VOS_Mem_Zero(g_stmmMgmt.pcBlk2, ulBufSum2 );
    }

    /*�����ڴ������ʼ��*/
    VOS_Node_Init(&g_stmmMgmt.stMMListF_32);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_32);
    g_stmmMgmt.ulFNum_32 = 0;
    g_stmmMgmt.ulUNum_32 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_64);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_64);
    g_stmmMgmt.ulFNum_64 = 0;
    g_stmmMgmt.ulUNum_64 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_128);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_128);
    g_stmmMgmt.ulFNum_128 = 0;
    g_stmmMgmt.ulUNum_128 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_256);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_256);
    g_stmmMgmt.ulFNum_256 = 0;
    g_stmmMgmt.ulUNum_256 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_512);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_512);
    g_stmmMgmt.ulFNum_512 = 0;
    g_stmmMgmt.ulUNum_512 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_1K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_1K);
    g_stmmMgmt.ulFNum_1K = 0;
    g_stmmMgmt.ulUNum_1K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_2K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_2K);
    g_stmmMgmt.ulFNum_2K = 0;
    g_stmmMgmt.ulUNum_2K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_4K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_4K);
    g_stmmMgmt.ulFNum_4K = 0;
    g_stmmMgmt.ulUNum_4K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_8K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_8K);
    g_stmmMgmt.ulFNum_8K = 0;
    g_stmmMgmt.ulUNum_8K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_16K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_16K);
    g_stmmMgmt.ulFNum_16K = 0;
    g_stmmMgmt.ulUNum_16K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_32K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_32K);
    g_stmmMgmt.ulFNum_32K = 0;
    g_stmmMgmt.ulUNum_32K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_64K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_64K);
    g_stmmMgmt.ulFNum_64K = 0;
    g_stmmMgmt.ulUNum_64K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_128K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_128K);
    g_stmmMgmt.ulFNum_128K = 0;
    g_stmmMgmt.ulUNum_128K = 0;


    /*С�ֽڵ��ڴ��1���и�ֵ*/
    pcMmOffset = g_stmmMgmt.pcBlk1;
    /*1. 32�ֽڵ��ڴ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_32_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_32;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_32, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_32++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_32 + ulSizeChar;
    }


    /*64�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_64_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_64;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_64, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_64++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_64 + ulSizeChar;
    }

    
    /*128�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_128_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_128;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
       //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_128, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_128++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_128 + ulSizeChar;
    }

    /*256�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_256_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_256;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_256, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_256++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_256 + ulSizeChar;
    }

    /*512�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_512_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_512;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_512, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_512++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_512 + ulSizeChar;
    }


    /*2. ���ֽڵ��ڴ��1���и�ֵ*/
    pcMmOffset = g_stmmMgmt.pcBlk2;
    /*. 1K�ֽڵ��ڴ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_1K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 =  VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 =  VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum    =  0;
        pstMmEntry->ulBufType    =  VOS_MEM_TYPE_1K;
        pstMmEntry->ulIdle       =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_1K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_1K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��1024�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_1K + ulSizeChar;
    }


    /*2K�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_2K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_2K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_2K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_2K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��2048�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_2K + ulSizeChar;
    }

    
    /*4096�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_4K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_4K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_4K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_4K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_4K + ulSizeChar;
    }

    /*8192�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_8K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_8K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_8K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_8K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_8K + ulSizeChar;
    }

    /*16384�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_16K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_16K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_16K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_16K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��16384�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_16K + ulSizeChar;
    }

    /*32768�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_32K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_32K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_32K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_32K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32768�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_32K + ulSizeChar;
    }

    /*65536�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_64K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_64K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_64K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_64K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32768�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_64K + ulSizeChar;
    }

    /*131072�ֽ�*/
    for(ulIndex = 0;ulIndex < VOS_MEM_128K_NUM;ulIndex++)
    {
        /*��ֵ�ڴ��ƫ�Ƶ�ַ*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*�����ڴ���и�ֵ*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_128K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*���һ�����ڴ��ʹ�õ�ַ��
           �ӳ�ʼ��ַƫ�Ƽ���*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*�����ڴ濪ʼ�ĵط�*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*���ڴ���뵽���е�������*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_128K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_128K++;
        /*�л�����һƬ���ڴ�, ƫ�ƴ�С��32768�ֽ�+ 1����λ
           ��ֹ����Խ��*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + VOS_MEM_SIZE_128K + ulSizeChar;
    }
    
    return VOS_OK;
    
ErrHandle:
    VOS_MemFree(g_stmmMgmt.pcBlk1);
    VOS_MemFree(g_stmmMgmt.pcBlk2);
    
    VOS_ThreadMutex_Destroy(&g_stmmLock32);
    VOS_ThreadMutex_Destroy(&g_stmmLock64);
    VOS_ThreadMutex_Destroy(&g_stmmLock128);
    VOS_ThreadMutex_Destroy(&g_stmmLock256);
    VOS_ThreadMutex_Destroy(&g_stmmLock512);
    VOS_ThreadMutex_Destroy(&g_stmmLock1K);
    VOS_ThreadMutex_Destroy(&g_stmmLock2K);
    VOS_ThreadMutex_Destroy(&g_stmmLock4K);
    VOS_ThreadMutex_Destroy(&g_stmmLock8K);
    VOS_ThreadMutex_Destroy(&g_stmmLock16K);
    VOS_ThreadMutex_Destroy(&g_stmmLock32K);
    VOS_ThreadMutex_Destroy(&g_stmmLock64K);
    VOS_ThreadMutex_Destroy(&g_stmmLock128K);
    
    return VOS_ERR;
}

/*�ڴ������ȥ��ʼ��*/
VOID VOS_MemMgmt_UnInit()
{  
    
    VOS_MemFree(g_stmmMgmt.pcBlk1);

    VOS_MemFree(g_stmmMgmt.pcBlk2);

    VOS_ThreadMutex_Destroy(&g_stmmLock32);
    VOS_ThreadMutex_Destroy(&g_stmmLock64);
    VOS_ThreadMutex_Destroy(&g_stmmLock128);
    VOS_ThreadMutex_Destroy(&g_stmmLock256);
    VOS_ThreadMutex_Destroy(&g_stmmLock512);
    VOS_ThreadMutex_Destroy(&g_stmmLock1K);
    VOS_ThreadMutex_Destroy(&g_stmmLock2K);
    VOS_ThreadMutex_Destroy(&g_stmmLock4K);
    VOS_ThreadMutex_Destroy(&g_stmmLock8K);
    VOS_ThreadMutex_Destroy(&g_stmmLock16K);
    VOS_ThreadMutex_Destroy(&g_stmmLock32K);
    VOS_ThreadMutex_Destroy(&g_stmmLock64K);
    VOS_ThreadMutex_Destroy(&g_stmmLock128K);
    
    return;
}

/*****************************************************************************
 �� �� ��  : VOS_MemCreate_X
 ��������  : �����ڴ棬��¼�˸��ڴ������ļ����к���Ϣ
 �������  :   ULONG ulMid       ---ģ����Ϣ
                            ULONG ulBufSize     ---�����ڴ��С         
                            CHAR *pcFileName    ---�����ڴ���ļ�����
                            ULONG ulLine        ---�����ڴ���к�
 �������  : ��
 �� �� ֵ  : CHAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��26��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID *VOS_MemCreate_X(ULONG ulMid, UINT32 ulBufSize, const CHAR *pcFileName, INT32 ulLine)
{   
    ULONG ulSysBufSize       = 0;
    VOID *pcMemBuf          = NULL;
    VOS_MEMBUF_UNIT_S *pstMem = NULL;
    VOS_DLIST_NODE_S   *pstNode = NULL;
    
    /*֧�ֶ��߳��ڴ���䣬��Ҫ����߳���*/
    if ( ulBufSize <= VOS_MEM_SIZE_32 )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32);
         /*������Ҫ���ǲ��ǿյģ��Ӳ�Ϊ�յĻ�ȡ
           *(VOS_TRUE != VOS_Node_IsEmpty(&g_stmmMgmt.stMMListF_32) )
           *���Ч��,Ŀǰ���Ը������ж�,����Ŀ�����ж�*/
        if ( g_stmmMgmt.ulFNum_32 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_32,&pstNode);

            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock32);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_32--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_32, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_32++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock32);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_8K ) && ( ulBufSize <= VOS_MEM_SIZE_16K ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock16K);
        
        if ( g_stmmMgmt.ulFNum_16K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_16K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock16K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_16K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_16K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_16K++;
            }                
        }
        
        VOS_RWLOCK_UNLOCK(g_stmmLock16K);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_32 ) && ( ulBufSize <= VOS_MEM_SIZE_64 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock64);
        
        if ( g_stmmMgmt.ulFNum_64 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_64,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock64);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_64--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_64, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_64++;
           }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock64);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_64 ) && ( ulBufSize <= VOS_MEM_SIZE_128 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock128);
        
        if ( g_stmmMgmt.ulFNum_128 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_128, &pstNode);
            if ( NULL != pstNode )
                {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock128);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_128--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_128, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_128++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock128);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_128 ) && ( ulBufSize <= VOS_MEM_SIZE_256 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock256);
        
        if ( g_stmmMgmt.ulFNum_256 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_256,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock256);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_256--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_256, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_256++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock256);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_256 ) && ( ulBufSize <= VOS_MEM_SIZE_512 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock512);
        
        if ( g_stmmMgmt.ulFNum_512 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_512,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock512);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_512--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_512, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_512++;
           }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock512);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_512 ) && ( ulBufSize <= VOS_MEM_SIZE_1K ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock1K);
        
        if ( g_stmmMgmt.ulFNum_1K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_1K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock1K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_1K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_1K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_1K++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock1K);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_1K ) && ( ulBufSize <= VOS_MEM_SIZE_2K ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock2K);
        
        if ( g_stmmMgmt.ulFNum_2K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_2K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock2K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_2K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_2K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_2K++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock2K);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_2K ) && ( ulBufSize <= VOS_MEM_SIZE_4K ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock4K);
        
        if ( g_stmmMgmt.ulFNum_4K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_4K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock4K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_4K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_4K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_4K++;
            }                
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock4K);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_4K ) && ( ulBufSize <= VOS_MEM_SIZE_8K ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock8K);
        
        if ( g_stmmMgmt.ulFNum_8K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_8K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode, VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock8K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_8K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_8K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_8K++;
                }                
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock8K);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_16K ) && ( ulBufSize <= VOS_MEM_SIZE_32K ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32K);
        
        if ( g_stmmMgmt.ulFNum_32K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_32K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock32K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_32K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_32K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_32K++;
            }                
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock32K);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_32K ) && ( ulBufSize <= VOS_MEM_SIZE_64K ) )
    {
       VOS_RWLOCK_LOCK(g_stmmLock64K);
       
       if ( g_stmmMgmt.ulFNum_64K >0 )
       {
           VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_64K, &pstNode);
           if ( NULL != pstNode )
           {
               pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
               if ( NULL == pstMem )
               {
                   VOS_RWLOCK_UNLOCK(g_stmmLock64K);
                   return NULL;
               }
               
               VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
               pstMem->ulLineNum =  ulLine;
               pstMem->ulIdle        =  VOS_MEM_USED;
               pstMem->ulMid        =  ulMid;
               pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
               
               g_stmmMgmt.ulFNum_64K--;
               VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_64K, &(pstMem->stNode));
               g_stmmMgmt.ulUNum_64K++;
            }               
       }
       VOS_RWLOCK_UNLOCK(g_stmmLock64K);
    }
    else if ( ( ulBufSize > VOS_MEM_SIZE_64K ) && ( ulBufSize <= VOS_MEM_SIZE_128K ) )
    {
       VOS_RWLOCK_LOCK(g_stmmLock128K);
       
       if ( g_stmmMgmt.ulFNum_128K >0 )
       {
           VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_128K,&pstNode);
            if ( NULL != pstNode )
            {
               pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
               if ( NULL == pstMem )
               {
                   VOS_RWLOCK_UNLOCK(g_stmmLock128K);
                   return NULL;
               }
               
               VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName));
               pstMem->ulLineNum =  ulLine;
               pstMem->ulIdle        =  VOS_MEM_USED;
               pstMem->ulMid        =  ulMid;
               pcMemBuf                =  (VOID *)pstMem->ucmmbuf;
               
               g_stmmMgmt.ulFNum_128K--;
               VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_128K, &(pstMem->stNode));
               g_stmmMgmt.ulUNum_128K++;
            }               
       }
       VOS_RWLOCK_UNLOCK(g_stmmLock128K);
    }
    else
    {       
            /*�ֽڶ���??*/
            ulSysBufSize = sizeof(VOS_MEMBUF_UNIT_S) + ulBufSize;
            pcMemBuf = malloc(ulSysBufSize);
            pstMem = (VOS_MEMBUF_UNIT_S *)pcMemBuf;
            VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, (VOID *)pcFileName, VOS_StrLen((CHAR *)pcFileName)); 
            pstMem->ulLineNum   =  ulLine;
            pstMem->ulIdle      =  VOS_MEM_USED;
            pstMem->ulMid       =  ulMid;
            pstMem->ulBufType   =  VOS_MEM_TYPE_SYS;
            pcMemBuf            =  (VOID *)pstMem->ucmmbuf;

            VOS_Printf("/****BigMemory Warning!:VOS_Malloc() used the system malloc(), [%s:%d]=[%p], memsize=%d",
                pcFileName, ulLine, pstMem, ulBufSize);
    }

    return pcMemBuf;
}


/*�黹�ڴ�������*/
VOID VOS_MemFree_X(CHAR *pcMemBuf)
{   
    VOS_MEMBUF_UNIT_S *pstMem = NULL;

    if ( NULL == pcMemBuf )
    {
        return;
    }

    pstMem = VOS_MEM_UNIT_ENTRY(pcMemBuf, VOS_MEMBUF_UNIT_S, ucmmbuf);
    if ( NULL == pstMem )
    {
        return;
    }
    
    VOS_Mem_Zero(pstMem->acFileName, VOS_FILEPATH_MAXLEN);
    pstMem->ulIdle = VOS_MEM_FREED;
    pstMem->ulMid  = 0;
    pstMem->ulLineNum = 0;
    
    if ( VOS_MEM_TYPE_32 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32);
        
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_32--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_32, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_32++;
        VOS_RWLOCK_UNLOCK(g_stmmLock32);
    }
    else if ( VOS_MEM_TYPE_16K == pstMem->ulBufType )   /*�����Ż�*/
    {
        VOS_RWLOCK_LOCK(g_stmmLock16K);        
        VOS_Node_Remove(&pstMem->stNode);        
        g_stmmMgmt.ulUNum_16K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_16K, &(pstMem->stNode));        
        g_stmmMgmt.ulFNum_16K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock16K);
    }
    else if ( VOS_MEM_TYPE_64 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock64);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_64--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_64, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_64++;
        VOS_RWLOCK_UNLOCK(g_stmmLock64);
    }
    else if ( VOS_MEM_TYPE_128 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock128);
        
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_128--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_128, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_128++;
        VOS_RWLOCK_UNLOCK(g_stmmLock128);
    }
    else if ( VOS_MEM_TYPE_256 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock256);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_256--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_256, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_256++;
        VOS_RWLOCK_UNLOCK(g_stmmLock256);
    }
    else if ( VOS_MEM_TYPE_512 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock512);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_512--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_512, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_512++;
        VOS_RWLOCK_UNLOCK(g_stmmLock512);
    }
    else if ( VOS_MEM_TYPE_1K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock1K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_1K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_1K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_1K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock1K);
    }
    else if ( VOS_MEM_TYPE_2K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock2K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_2K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_2K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_2K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock2K);
    }
    else if ( VOS_MEM_TYPE_4K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock4K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_4K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_4K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_4K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock4K);
    }
    else if ( VOS_MEM_TYPE_8K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock8K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_8K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_8K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_8K++;            
        VOS_RWLOCK_UNLOCK(g_stmmLock8K);
    }
    else if ( VOS_MEM_TYPE_32K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32K);        
        VOS_Node_Remove(&pstMem->stNode);        
        g_stmmMgmt.ulUNum_32K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_32K, &(pstMem->stNode));        
        g_stmmMgmt.ulFNum_32K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock32K);
    }
    else if ( VOS_MEM_TYPE_64K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock64K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_64K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_64K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_64K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock64K);
    }
    else if ( VOS_MEM_TYPE_128K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock128K);        
        VOS_Node_Remove(&pstMem->stNode);        
        g_stmmMgmt.ulUNum_128K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_128K, &(pstMem->stNode));        
        g_stmmMgmt.ulFNum_128K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock128K);
    }
    else
    {
        VOS_Printf("/****BigMemory Warning!:VOS_MemFree() used the system free(), [%s:%d]=[%p]",
                pstMem->acFileName, pstMem->ulLineNum, pstMem);
        free(pstMem);
    }
    
    return;
}

/*****************************************************************************
 �� �� ��  : VOS_MM_TypeShow
 ��������  : �ڴ���ʾ
 �������  : UINT32 uiType  
             UINT32 uiMid   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��26��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_MM_MidTypeShow(UINT32 uiType,  UINT32 uiMid)
{
    VOS_DLIST_NODE_S   *pstDList     = NULL;
    VOS_RW_LOCK_S       *pstMutex   = NULL;
    VOS_MEMBUF_UNIT_S *pstMmBuf  = NULL;   
    ULONG                       ulCount      = 0;
    ULONG                       ulFreedNum= 0 ;
    ULONG                       ulUsedNum = 0;

    switch(uiType)
    {
        case VOS_MEM_TYPE_32:
            pstDList = &g_stmmMgmt.stMMListU_32;
            pstMutex = &g_stmmLock32;
            ulFreedNum = g_stmmMgmt.ulFNum_32;
            ulUsedNum = g_stmmMgmt.ulUNum_32;
            break;
        case VOS_MEM_TYPE_64:
            pstDList = &g_stmmMgmt.stMMListU_64;
            pstMutex = &g_stmmLock64;
            ulFreedNum = g_stmmMgmt.ulFNum_64;
            ulUsedNum = g_stmmMgmt.ulUNum_64;
            break;
        case VOS_MEM_TYPE_128:
            pstDList = &g_stmmMgmt.stMMListU_128;
            pstMutex = &g_stmmLock128;
            ulFreedNum = g_stmmMgmt.ulFNum_128;
            ulUsedNum = g_stmmMgmt.ulUNum_128;
            break;
        case VOS_MEM_TYPE_256:
            pstDList = &g_stmmMgmt.stMMListU_256;
            pstMutex = &g_stmmLock256;
            ulFreedNum = g_stmmMgmt.ulFNum_256;
            ulUsedNum = g_stmmMgmt.ulUNum_256;
            break;
        case VOS_MEM_TYPE_512:
            pstDList = &g_stmmMgmt.stMMListU_512;
            pstMutex = &g_stmmLock512;
            ulFreedNum = g_stmmMgmt.ulFNum_512;
            ulUsedNum = g_stmmMgmt.ulUNum_512;
            break;
        case VOS_MEM_TYPE_1K:
            pstDList = &g_stmmMgmt.stMMListU_1K;
            pstMutex = &g_stmmLock1K;
            ulFreedNum = g_stmmMgmt.ulFNum_1K;
            ulUsedNum = g_stmmMgmt.ulUNum_1K;
            break;
        case VOS_MEM_TYPE_2K:
            pstDList = &g_stmmMgmt.stMMListU_2K;
            pstMutex = &g_stmmLock2K;
            ulFreedNum = g_stmmMgmt.ulFNum_2K;
            ulUsedNum = g_stmmMgmt.ulUNum_2K;
            break;
        case VOS_MEM_TYPE_4K:
            pstDList = &g_stmmMgmt.stMMListU_4K;
            pstMutex = &g_stmmLock4K;
            ulFreedNum = g_stmmMgmt.ulFNum_4K;
            ulUsedNum = g_stmmMgmt.ulUNum_4K;
            break;
        case VOS_MEM_TYPE_8K:
            pstDList = &g_stmmMgmt.stMMListU_8K;
            pstMutex = &g_stmmLock8K;
            ulFreedNum = g_stmmMgmt.ulFNum_8K;
            ulUsedNum = g_stmmMgmt.ulUNum_8K;
            break;
        case VOS_MEM_TYPE_16K:
            pstDList = &g_stmmMgmt.stMMListU_16K;
            pstMutex = &g_stmmLock16K;
            ulFreedNum = g_stmmMgmt.ulFNum_16K;
            ulUsedNum = g_stmmMgmt.ulUNum_16K;
            break;
        case VOS_MEM_TYPE_32K:
            pstDList = &g_stmmMgmt.stMMListU_32K;
            pstMutex = &g_stmmLock32K;
            ulFreedNum = g_stmmMgmt.ulFNum_32K;
            ulUsedNum = g_stmmMgmt.ulUNum_32K;
            break;
        case VOS_MEM_TYPE_64K:
            pstDList = &g_stmmMgmt.stMMListU_64K;
            pstMutex = &g_stmmLock64K;
            ulFreedNum = g_stmmMgmt.ulFNum_64K;
            ulUsedNum = g_stmmMgmt.ulUNum_64K;
            break;
        case VOS_MEM_TYPE_128K:
            pstDList = &g_stmmMgmt.stMMListU_128K;
            pstMutex = &g_stmmLock128K;
            ulFreedNum = g_stmmMgmt.ulFNum_128K;
            ulUsedNum = g_stmmMgmt.ulUNum_128K;
            break;
        default:
            return;
    }

    if ( NULL == pstDList
        || NULL == pstMutex )
    {
        return;
    }

    
    VOS_RWLOCK_PLOCK(pstMutex);
    
    if ( VOS_OK != VOS_Node_IsEmpty(pstDList)  )
    {
        /* 50Line */
        VOS_Printf("########### Type:[%08x],FreeNum=%d,UsedNum=%d##########",
                    uiType, ulFreedNum, ulUsedNum);

        pstMmBuf = VOS_DLIST_ENTRY(pstDList->next, VOS_MEMBUF_UNIT_S, stNode);
        /*ƥ�����0:��ʾȫ����ʾ*/
        if ( uiMid == pstMmBuf->ulMid )
        {
            ulCount++;
            VOS_Printf("***********MID:[%08x],File:[%s],Line:[%d]", 
                pstMmBuf->ulMid,pstMmBuf->acFileName, pstMmBuf->ulLineNum);
        }
        
        for(;pstMmBuf->stNode.next != pstDList;\
            pstMmBuf=VOS_DLIST_ENTRY(pstMmBuf->stNode.next,VOS_MEMBUF_UNIT_S, stNode))
        {
            ulCount++;
            
            if ( NULL == pstMmBuf )
            {
                break;
            }
            if ( uiMid == pstMmBuf->ulMid )
            {
                VOS_Printf("***********MID:[%08x],File:[%s],Line:[%d]", 
                    pstMmBuf->ulMid,pstMmBuf->acFileName, pstMmBuf->ulLineNum );
            }
        }
        
        VOS_Printf("################Count=%d#####################", ulCount);
    }

    VOS_RWLOCK_UNPLOCK(pstMutex);
        
   return;
}


/*****************************************************************************
 �� �� ��  : VOS_MM_TypeShow
 ��������  : ��ʾĳһ���͵��ڴ�
 �������  : UINT32 uiType  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��26��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_MM_TypeShow(UINT32 uiType)
{
    VOS_DLIST_NODE_S   *pstDList     = NULL;
    VOS_RW_LOCK_S       *pstMutex   = NULL;
    VOS_MEMBUF_UNIT_S *pstMmBuf  = NULL;   
    ULONG                       ulCount      = 0;
    ULONG                       ulFreedNum= 0 ;
    ULONG                       ulUsedNum = 0;

    switch(uiType)
    {
        case VOS_MEM_TYPE_32:
            pstDList = &g_stmmMgmt.stMMListU_32;
            pstMutex = &g_stmmLock32;
            ulFreedNum = g_stmmMgmt.ulFNum_32;
            ulUsedNum = g_stmmMgmt.ulUNum_32;
            break;
        case VOS_MEM_TYPE_64:
            pstDList = &g_stmmMgmt.stMMListU_64;
            pstMutex = &g_stmmLock64;
            ulFreedNum = g_stmmMgmt.ulFNum_64;
            ulUsedNum = g_stmmMgmt.ulUNum_64;
            break;
        case VOS_MEM_TYPE_128:
            pstDList = &g_stmmMgmt.stMMListU_128;
            pstMutex = &g_stmmLock128;
            ulFreedNum = g_stmmMgmt.ulFNum_128;
            ulUsedNum = g_stmmMgmt.ulUNum_128;
            break;
        case VOS_MEM_TYPE_256:
            pstDList = &g_stmmMgmt.stMMListU_256;
            pstMutex = &g_stmmLock256;
            ulFreedNum = g_stmmMgmt.ulFNum_256;
            ulUsedNum = g_stmmMgmt.ulUNum_256;
            break;
        case VOS_MEM_TYPE_512:
            pstDList = &g_stmmMgmt.stMMListU_512;
            pstMutex = &g_stmmLock512;
            ulFreedNum = g_stmmMgmt.ulFNum_512;
            ulUsedNum = g_stmmMgmt.ulUNum_512;
            break;
        case VOS_MEM_TYPE_1K:
            pstDList = &g_stmmMgmt.stMMListU_1K;
            pstMutex = &g_stmmLock1K;
            ulFreedNum = g_stmmMgmt.ulFNum_1K;
            ulUsedNum = g_stmmMgmt.ulUNum_1K;
            break;
        case VOS_MEM_TYPE_2K:
            pstDList = &g_stmmMgmt.stMMListU_2K;
            pstMutex = &g_stmmLock2K;
            ulFreedNum = g_stmmMgmt.ulFNum_2K;
            ulUsedNum = g_stmmMgmt.ulUNum_2K;
            break;
        case VOS_MEM_TYPE_4K:
            pstDList = &g_stmmMgmt.stMMListU_4K;
            pstMutex = &g_stmmLock4K;
            ulFreedNum = g_stmmMgmt.ulFNum_4K;
            ulUsedNum = g_stmmMgmt.ulUNum_4K;
            break;
        case VOS_MEM_TYPE_8K:
            pstDList = &g_stmmMgmt.stMMListU_8K;
            pstMutex = &g_stmmLock8K;
            ulFreedNum = g_stmmMgmt.ulFNum_8K;
            ulUsedNum = g_stmmMgmt.ulUNum_8K;
            break;
        case VOS_MEM_TYPE_16K:
            pstDList = &g_stmmMgmt.stMMListU_16K;
            pstMutex = &g_stmmLock16K;
            ulFreedNum = g_stmmMgmt.ulFNum_16K;
            ulUsedNum = g_stmmMgmt.ulUNum_16K;
            break;
        case VOS_MEM_TYPE_32K:
            pstDList = &g_stmmMgmt.stMMListU_32K;
            pstMutex = &g_stmmLock32K;
            ulFreedNum = g_stmmMgmt.ulFNum_32K;
            ulUsedNum = g_stmmMgmt.ulUNum_32K;
            break;
        case VOS_MEM_TYPE_64K:
            pstDList = &g_stmmMgmt.stMMListU_64K;
            pstMutex = &g_stmmLock64K;
            ulFreedNum = g_stmmMgmt.ulFNum_64K;
            ulUsedNum = g_stmmMgmt.ulUNum_64K;
            break;
        case VOS_MEM_TYPE_128K:
            pstDList = &g_stmmMgmt.stMMListU_128K;
            pstMutex = &g_stmmLock128K;
            ulFreedNum = g_stmmMgmt.ulFNum_128K;
            ulUsedNum = g_stmmMgmt.ulUNum_128K;
            break;
        default:
            return;
    }

    if ( NULL == pstDList
        || NULL == pstMutex )
    {
        return;
    }
    
    VOS_RWLOCK_PLOCK(pstMutex);
    
    if ( VOS_OK != VOS_Node_IsEmpty(pstDList)  )
    {
        VOS_Printf("########### Type:[%08x],FreeNum=%d,UsedNum=%d##########",
                    uiType, ulFreedNum, ulUsedNum);

        pstMmBuf = VOS_DLIST_ENTRY(pstDList->next, VOS_MEMBUF_UNIT_S, stNode);

        ulCount++;
        VOS_Printf("***********MID:[%08x], File:[%s],Line:[%d]", 
                pstMmBuf->ulMid, pstMmBuf->acFileName, pstMmBuf->ulLineNum);
        
        
        for(;pstMmBuf->stNode.next != pstDList;\
            pstMmBuf=VOS_DLIST_ENTRY(pstMmBuf->stNode.next,VOS_MEMBUF_UNIT_S, stNode))
        {
            ulCount++;
            
            if ( NULL == pstMmBuf )
            {
                break;
            }

            VOS_Printf("***********MID:[%08x], File:[%s],Line:[%d]", 
                    pstMmBuf->ulMid, pstMmBuf->acFileName, pstMmBuf->ulLineNum);
            
        }
        
        VOS_Printf("################Count=%d#####################", ulCount);
    }

    VOS_RWLOCK_UNPLOCK(pstMutex);
        
   return;
}



VOID VOS_MM_Show()
{
    UINT32 ulIndex = 0;

    for (ulIndex = VOS_MEM_TYPE_32; ulIndex <= VOS_MEM_TYPE_128K; ulIndex++)
    {
        VOS_MM_TypeShow(ulIndex);
    }
    
    return;
}
#endif
