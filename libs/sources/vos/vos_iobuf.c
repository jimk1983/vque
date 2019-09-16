
#include <vos/vos_pub.h>

/*****************************************************************************
 函 数 名  : EQue_IOBuf_malloc
 功能描述  : 申请IO内存，该内存需要系统复用
 输入参数  : uint32_t ulMid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年5月24日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
    /*包的大小，不能超过InLeftSize*/
    pstIobuf->InLeftSize        = pstIobuf->MaxLen;
    pstIobuf->OffsetCurrt       = pstIobuf->OffsetStart;
    pstIobuf->DataSize          = 0;
    pstIobuf->OutLeftSize       = 0;

    return pstIobuf;
}

VOS_IOBUF_S*    VOS_IOBuf_mallocMax(uint32_t ulMid)
{
    VOS_IOBUF_S *pstIobuf   =    NULL;
    /*特别的预留出一个包的大小, 用于分片接收*/
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
    pstIobuf->MaxLen            = VOS_IOBUF_BUFMAX - pstIobuf->OffsetStart - 4; //4字节对齐保证
    pstIobuf->InLeftSize        = pstIobuf->MaxLen;
    pstIobuf->OffsetCurrt       = pstIobuf->OffsetStart;
    pstIobuf->DataSize          = 0;
    pstIobuf->OutLeftSize       = 0;

    return pstIobuf;
}

/*****************************************************************************
 函 数 名  : VOS_IOBuf_free
 功能描述  : IOBUF释放
 输入参数  : VOS_IOBUF_S *pstIobuf  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年5月24日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
void        VOS_IOBuf_free(VOS_IOBUF_S *pstIobuf)
{
    if ( NULL == pstIobuf  )
    {
        return;
    }
    
    free(pstIobuf);
}

