
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include "public.h"

typedef struct tagMsgQueConn
{
    PVOID           pvRctor;
    VRCT_MSQ_OPT_S  stMsqOpt;
    
}TMSQ_CONN_S, *PTMSQ_CONN_S;

void *TMSQ_WorkCustomter(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    
    printf("Value:%d\n", Value);
    
    return NULL;
}


void *TMSQ_WorkProducer(void *pvArgv)
{
    PTMSQ_CONN_S        pstMsqConn  = (PTMSQ_CONN_S)pvArgv;
    INT32_T             Value = 0;
    
    while(1)
    {
        Value++;
        printf("push value=%d\n", Value);
        VRCT_API_MsqOptPush(pstMsqConn->pvRctor, 0, Value, NULL, 0);
        VOS_Sleep(1);
    }
    
    return NULL;
}


VOID TMain_MsQueue(PTEST_MSQ_CFG_S pstCfg)
{
    PTMSQ_CONN_S        pstMsqConn  = NULL;
    INT32_T             Ret         = 0;
    pthread_t           PthHandle   = 0;
    
    pstMsqConn = (PTMSQ_CONN_S)malloc(sizeof(TMSQ_CONN_S));
    if ( NULL == pstMsqConn )
    {
        PError("malloc error!");
        return;
    }
    
    pstMsqConn->pvRctor = VRCT_API_Create(0,100000);
    if ( NULL == pstMsqConn->pvRctor )
    {
        PError("Vos Reactor create error!");
        goto ErrorExit;
    }
    
    VRCT_MSQOPT_INIT(&pstMsqConn->stMsqOpt,
                     0,
                     TMSQ_WorkCustomter,
                     pstMsqConn);
    
    if ( VOS_ERR == VRCT_API_MsqOptRegister( pstMsqConn->pvRctor, &pstMsqConn->stMsqOpt) )
    {
        PError("vos reactor message queue register!");
        goto ErrorExit;
    }

    if ( VOS_ERR == VRCT_API_Start(pstMsqConn->pvRctor)  )
    {
        PError("vos reactor start failed!");
        goto ErrorExit;
    }
    
    Ret = pthread_create(&PthHandle, NULL, TMSQ_WorkProducer, pstMsqConn);
    if ( 0 != Ret )
    {
        PError("vos reactor work producer!");
        goto ErrorExit;
    }
    
    while(1)
    {
        VOS_Sleep(10);
    }
    
ErrorExit:
    
    if ( NULL != pstMsqConn )
    {
        if ( NULL != pstMsqConn->pvRctor )
        {
            VRCT_API_Release(&pstMsqConn->pvRctor);
        }
        
        free(pstMsqConn);
    }
    
    return;
}







