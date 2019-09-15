
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include "public.h"

typedef struct tagServConn
{
    PVOID               pvRctor;
    VRCT_NETEVT_OPT_S   stNetOpt;

    CHAR                acSevAddr[32];
    UINT32_T            Port;
    
}TSEV_CONN_S, *PTSEV_CONN_S;


VOID TMain_Server(TEST_SEVR_CFG_S pstCfg)
{
    
    
}


























