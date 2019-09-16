
#include <vos/vos_pub.h>

/*****************************************************************************
 �� �� ��  : EQue_IOBuf_malloc
 ��������  : ����IO�ڴ棬���ڴ���Ҫϵͳ����
 �������  : uint32_t ulMid  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��5��24��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_IOBUF_S*    VOS_IOBuf_malloc(uint32_t ulMid)
{
    VOS_IOBUF_S*    pstIobuf    =    NULL;
    uint32_t        ulBufSize   =    VOS_IOBUF_MAXSIZE;

    pstIobuf = (VOS_IOBUF_S *)malloc(ulBufSize);
    if ( NULL == pstIobuf )
    {
        PError("system malloc error!");
        return NULL;
    }

    memset(pstIobuf,0,ulBufSize);
    VOS_DLIST_INIT(&pstIobuf->stNode);
    pstIobuf->Magic             = VOS_IOBUF_MAGIC;
    pstIobuf->OffsetStart       = sizeof(VOS_IOBUF_S) + VOS_PREHEAD_SIZE;
    pstIobuf->MaxLen            = VOS_IOBUF_MAXSIZE-pstIobuf->OffsetStart;
    /*���Ĵ�С�����ܳ���InLeftSize*/
    pstIobuf->InLeftSize        = pstIobuf->MaxLen;
    pstIobuf->OffsetCurrt       = pstIobuf->OffsetStart;
    pstIobuf->DataSize          = 0;
    pstIobuf->OutLeftSize       = 0;

    return pstIobuf;
}

VOS_IOBUF_S*    VOS_IOBuf_mallocMax(uint32_t ulMid)
{
    VOS_IOBUF_S *pstIobuf   =    NULL;
    /*�ر��Ԥ����һ�����Ĵ�С, ���ڷ�Ƭ����*/
    uint32_t ulBufSize      =    VOS_IOBUF_BUFMAX;

    pstIobuf = (VOS_IOBUF_S *)malloc(ulBufSize);
    if ( NULL == pstIobuf )
    {
        PError("system malloc error!");
        return NULL;
    }

    memset(pstIobuf,0,ulBufSize);
    VOS_DLIST_INIT(&pstIobuf->stNode);
    pstIobuf->Magic             = VOS_IOBUF_MAGIC;
    pstIobuf->OffsetStart       = sizeof(VOS_IOBUF_S) + VOS_IOBUF_MAXSIZE;
    pstIobuf->MaxLen            = VOS_IOBUF_BUFMAX - pstIobuf->OffsetStart - 4; //4�ֽڶ��뱣֤
    pstIobuf->InLeftSize        = pstIobuf->MaxLen;
    pstIobuf->OffsetCurrt       = pstIobuf->OffsetStart;
    pstIobuf->DataSize          = 0;
    pstIobuf->OutLeftSize       = 0;

    return pstIobuf;
}

/*****************************************************************************
 �� �� ��  : VOS_IOBuf_free
 ��������  : IOBUF�ͷ�
 �������  : VOS_IOBUF_S *pstIobuf  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��5��24��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
void        VOS_IOBuf_free(VOS_IOBUF_S *pstIobuf)
{
    if ( NULL == pstIobuf  )
    {
        return;
    }
    
    free(pstIobuf);
}

