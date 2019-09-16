
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include "public.h"



VOID TMain_Iobuf()
{
    VOS_IOBUF_S* pstIobufA = NULL;
    VOS_IOBUF_S* pstIobufB = NULL;
    VOS_IOBUF_S* pstIobufC = NULL;
    
    for(int i=0; i< 1000; i++)
    {
        printf("[%s] run num=%d\n", VOS_GetSysTimeNowStr(),i);
        pstIobufA = VOS_IOBuf_malloc(0);
        if(NULL == pstIobufA)
        {
            return;
        }
        memset(pstIobufA->Data,'A', 1680-i);
        VOS_IOBuf_free(pstIobufA);
        pstIobufA=NULL;
        pstIobufB = VOS_IOBuf_malloc(0);
        if(NULL == pstIobufB)
        {
            return;
        }
        memset(pstIobufB->Data,'B', 1680);
        VOS_IOBuf_free(pstIobufB);
        pstIobufB=NULL;

        pstIobufC = VOS_IOBuf_malloc(0);
        if(NULL == pstIobufC)
        {
            return;
        }
        memset(pstIobufC->Data,'C', 1680);
        VOS_IOBuf_free(pstIobufC);
        pstIobufC=NULL;
    }

    printf("[%s] Iobuf main test successful!\n", VOS_GetSysTimeNowStr());
}



