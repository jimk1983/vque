#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include <memory>
#include <mcheck.h>
#include <config/config.h>
#include <crct/cevtrctor_net_proxy.h>
#include <proxy/proxy.h>

void    proxy_main(pexm_proxy_cfg_s      pstCfg)
{
    cevt_net_proxy_sptr    dispatch_sptr = std::make_shared<CEvtrctNetProxy>();
    //pexm_proxy_cfg_s ptrtest  = NULL;
    
    dispatch_sptr->start(pstCfg);
    //printf("ptrtest test=%s\n",ptrtest->acLocalAddr);
    
    while(1)
    {
        VOS_Sleep(10);
    }
    
    return;
}













