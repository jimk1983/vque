#ifndef _VOS_IOBUF_H_
#define _VOS_IOBUF_H_


#define     VOS_PREHEAD_SIZE    256

#define     VOS_IOBUF_MAXSIZE   2048

#define     VOS_IOBUF_MAGIC     0xAABBCCDD


#define     VOS_IOBUF_BUFMAX    67584 //65536+2048


/*定义的通用的Iobuf块*/
typedef struct tagVosIobuf
{
    VOS_DLIST_S             stNode;             /*挂链用的节点*/
    uint32_t                Magic;              /*本节点最大的数据长度*/
    int32_t                 MaxLen;             /*接收包的数据最大的长度*/  
    int32_t                 InLeftSize;         /*接收剩余大小*/ 
    uint32_t                OutLeftSize;        /*发送剩余大小*/ 
    uint32_t                OffsetStart;        /*有效业务数据*/
    uint32_t                OffsetCurrt;        /*当前偏移*/
    uint32_t                DataSize;           /*本次的接收到的包的大小或者待发送的大小*/
    char                    Data[0];            /*柔性数组*/
}VOS_IOBUF_S, *PVOS_IOBUF_S;


/*获取有效的数据大小*/
#define VOS_IOBUF_GETCURT_SIZE(_pstIobuf)     ((_pstIobuf)->DataSize)
/*获取剩余空闲空间地址*/
#define VOS_IOBUF_GETCURT_PTR(_pstIobuf)      (char *)(&(_pstIobuf)->Data[0] +(_pstIobuf)->OffsetCurrt)
/*获取有效的数据起始地址*/
#define COM_IOBUF_GETORIG_PTR(_pstIobuf)      (char *)(&(_pstIobuf)->Data[0] + (_pstIobuf)->OffsetStart)
/*获取剩余空闲空间长度*/
#define VOS_IOBUF_RCVLEFT_SIZE(_pstIobuf)     ((_pstIobuf)->InLeftSize)
/*设置已经接收的数据长度，并更新剩余数据空间长度*/
#define VOS_IOBUF_RCVUPDATE_SIZE(_pstIobuf, ulInputLen) \
do{\
    (_pstIobuf)->DataSize   += ulInputLen;\
    (_pstIobuf)->OffsetCurrt+=ulInputLen;\
    (_pstIobuf)->InLeftSize = (_pstIobuf)->MaxLen-(_pstIobuf)->DataSize;\
}while(0);

/*获取剩余空闲空间长度*/
#define VOS_IOBUF_SNDLEFT_SIZE(_pstIobuf)     ((_pstIobuf)->OutLeftSize)

/*更新有效数据处理后剩余长度*/
#define VOS_IOBUF_SNDUPDATE_SIZE(_pstIobuf, ulOutputLen) \
do{\
    (_pstIobuf)->OutLeftSize -=ulOutputLen;\
    (_pstIobuf)->OffsetCurrt +=ulOutputLen;\
}while(0);

/*重置*/
#define VOS_IOBUF_OUTRESET(_pstIobuf) \
do{\
    (_pstIobuf)->OffsetCurrt = (_pstIobuf)->OffsetStart;\
    (_pstIobuf)->OutLeftSize = (_pstIobuf)->DataSize;\
}while(0);

VOS_IOBUF_S*    VOS_IOBuf_malloc(uint32_t ulMid);

void            VOS_IOBuf_free(VOS_IOBUF_S *pstIobuf);

VOS_IOBUF_S*    VOS_IOBuf_mallocMax(uint32_t ulMid);


#endif
