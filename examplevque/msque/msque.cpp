
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include "public.h"

typedef struct tagMsgQueConn TMSQ_CONN_S, *PTMSQ_CONN_S;
struct tagMsgQueConn
{
    PVOID           pvRctor;
    VRCT_MSQ_OPT_S  stMsqOpt;
    PTMSQ_CONN_S    pstNextConn;
};

void *TMSQ_WorkCustomter2(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    PTMSQ_CONN_S        pstMsqConn  = (PTMSQ_CONN_S)pvCtx;
    if ( (Value & 0x7FFFFF) == 0x7FFFFF )
    {
        printf("22222 [%d][Customter2]:POP Value:%d,time=%s,ctx=[%p],nextptr=%p\n", 
                VOS_GetSelfTId(),Value, VOS_GetSysTimeNowStr(), pstMsqConn, pstMsqConn->pstNextConn);
    }
    
    return NULL;
}


void *TMSQ_WorkCustomter(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    PTMSQ_CONN_S        pstMsqConn  = (PTMSQ_CONN_S)pvCtx;
    INT32_T             Ret = 0;
    if ( (Value & 0x7FFFFF) == 0x7FFFFF )
    {
        printf("11111 [%d][Customter]:POP Value:%d,time=%s,ctx=[%p],nextptr=%p\n", 
                VOS_GetSelfTId(),Value, VOS_GetSysTimeNowStr(), pstMsqConn, pstMsqConn->pstNextConn);
        
        Ret = VRCT_API_MsqOptPush(pstMsqConn->pstNextConn->pvRctor, 0, Value, NULL, 0);
        if( Ret < 0)
        {
            printf("vrct message push err=%d\n", Ret);
        }
    }
    
    return NULL;
}

void *TMSQ_WorkProducer(void *pvArgv)
{
    INT32_T             Value = 0;
    INT32_T             Ret = 0;
    PTMSQ_CONN_S        pstMsqConn  = (PTMSQ_CONN_S)pvArgv;
    
    while(1)
    {
        Value++;
        if ( (Value & 0x7FFFFF) == 0x7FFFFF )
                printf("[Producer]: PUSH value=%d,time=%s, msgConn=%p,nextptr=%p\n", 
                        Value, VOS_GetSysTimeNowStr(), pstMsqConn, pstMsqConn->pstNextConn);

        Ret = VRCT_API_MsqOptPush(pstMsqConn->pvRctor, 0, Value, NULL, 0);
        if( Ret < 0)
        {
            printf("vrct message push err=%d\n", Ret);
        }
        
        //usleep(1);
    }
    
    return NULL;
}


PTMSQ_CONN_S TMain_MsMutlConn(PTEST_MSQ_CFG_S pstCfg)
{
    PTMSQ_CONN_S        pstMsqConn  = NULL;
    
    pstMsqConn = (PTMSQ_CONN_S)malloc(sizeof(TMSQ_CONN_S));
    if ( NULL == pstMsqConn )
    {
        PError("malloc error!");
        return NULL;
    }
    memset(pstMsqConn,0,sizeof(TMSQ_CONN_S));
    
    pstMsqConn->pvRctor = VRCT_API_Create(0,100000);
    if ( NULL == pstMsqConn->pvRctor )
    {
        PError("Vos Reactor create error!");
        free(pstMsqConn);
        return NULL;
    }
    pstMsqConn->pstNextConn = NULL;
    
    VRCT_MSQOPT_INIT(&pstMsqConn->stMsqOpt,
                     0,
                     TMSQ_WorkCustomter2,
                     pstMsqConn);
    
    if ( VOS_ERR == VRCT_API_MsqOptRegister( pstMsqConn->pvRctor, &pstMsqConn->stMsqOpt) )
    {
        PError("vos reactor message queue register!");
        free(pstMsqConn);
        VRCT_API_Release(&pstMsqConn->pvRctor);
        return NULL;
    }
    
    if ( VOS_ERR == VRCT_API_Start(pstMsqConn->pvRctor)  )
    {
        PError("vos reactor start failed!");
        free(pstMsqConn);
        VRCT_API_Release(&pstMsqConn->pvRctor);
        return NULL;
    }
    
    return pstMsqConn;
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
    
    pstMsqConn->pstNextConn = NULL;
    
    VRCT_MSQOPT_INIT(&pstMsqConn->stMsqOpt,
                     0,
                     TMSQ_WorkCustomter,
                     pstMsqConn);
    
    if ( VOS_ERR == VRCT_API_MsqOptRegister( pstMsqConn->pvRctor, &pstMsqConn->stMsqOpt) )
    {
        PError("vos reactor message queue register!");
        goto ErrorExit;
    }
    
    pstMsqConn->pstNextConn = TMain_MsMutlConn(pstCfg);
    if ( NULL == pstMsqConn->pstNextConn )
    {
        PError("vos reactor start failed!");
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







