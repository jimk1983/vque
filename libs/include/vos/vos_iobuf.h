#ifndef _VOS_IOBUF_H_
#define _VOS_IOBUF_H_


#define     VOS_PREHEAD_SIZE    256

#define     VOS_IOBUF_MAXSIZE   2048

#define     VOS_IOBUF_MAGIC     0xAABBCCDD


#define     VOS_IOBUF_BUFMAX    67584 //65536+2048


/*�����ͨ�õ�Iobuf��*/
typedef struct tagVosIobuf
{
    VOS_DLIST_S             stNode;             /*�����õĽڵ�*/
    uint32_t                Magic;              /*���ڵ��������ݳ���*/
    int32_t                 MaxLen;             /*���հ����������ĳ���*/  
    int32_t                 InLeftSize;         /*����ʣ���С*/ 
    uint32_t                OutLeftSize;        /*����ʣ���С*/ 
    uint32_t                OffsetStart;        /*��Чҵ������*/
    uint32_t                OffsetCurrt;        /*��ǰƫ��*/
    uint32_t                DataSize;           /*���εĽ��յ��İ��Ĵ�С���ߴ����͵Ĵ�С*/
    char                    Data[0];            /*��������*/
}VOS_IOBUF_S, *PVOS_IOBUF_S;


/*��ȡ��Ч�����ݴ�С*/
#define VOS_IOBUF_GETCURT_SIZE(_pstIobuf)     ((_pstIobuf)->DataSize)
/*��ȡʣ����пռ��ַ*/
#define VOS_IOBUF_GETCURT_PTR(_pstIobuf)      (char *)(&(_pstIobuf)->Data[0] +(_pstIobuf)->OffsetCurrt)
/*��ȡ��Ч��������ʼ��ַ*/
#define COM_IOBUF_GETORIG_PTR(_pstIobuf)      (char *)(&(_pstIobuf)->Data[0] + (_pstIobuf)->OffsetStart)
/*��ȡʣ����пռ䳤��*/
#define VOS_IOBUF_RCVLEFT_SIZE(_pstIobuf)     ((_pstIobuf)->InLeftSize)
/*�����Ѿ����յ����ݳ��ȣ�������ʣ�����ݿռ䳤��*/
#define VOS_IOBUF_RCVUPDATE_SIZE(_pstIobuf, ulInputLen) \
do{\
    (_pstIobuf)->DataSize   += ulInputLen;\
    (_pstIobuf)->OffsetCurrt+=ulInputLen;\
    (_pstIobuf)->InLeftSize = (_pstIobuf)->MaxLen-(_pstIobuf)->DataSize;\
}while(0);

/*��ȡʣ����пռ䳤��*/
#define VOS_IOBUF_SNDLEFT_SIZE(_pstIobuf)     ((_pstIobuf)->OutLeftSize)

/*������Ч���ݴ����ʣ�೤��*/
#define VOS_IOBUF_SNDUPDATE_SIZE(_pstIobuf, ulOutputLen) \
do{\
    (_pstIobuf)->OutLeftSize -=ulOutputLen;\
    (_pstIobuf)->OffsetCurrt +=ulOutputLen;\
}while(0);

/*����*/
#define VOS_IOBUF_OUTRESET(_pstIobuf) \
do{\
    (_pstIobuf)->OffsetCurrt = (_pstIobuf)->OffsetStart;\
    (_pstIobuf)->OutLeftSize = (_pstIobuf)->DataSize;\
}while(0);

VOS_IOBUF_S*    VOS_IOBuf_malloc(uint32_t ulMid);

void            VOS_IOBuf_free(VOS_IOBUF_S *pstIobuf);

VOS_IOBUF_S*    VOS_IOBuf_mallocMax(uint32_t ulMid);


#endif
