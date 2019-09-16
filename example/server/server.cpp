#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include <memory>
#include <mcheck.h>
#include <config/config.h>
#include <crct/cevtrctor_net_server.h>
#include <server/server.h>

void    server_main(pexm_serv_cfg_s      pstCfg)
{
    cevt_net_server_sptr    dispatch_sptr = std::make_shared<CEvtrctNetServer>();
    
    dispatch_sptr->start(pstCfg);
    
    
    while(1)
    {
        VOS_Sleep(10);
    }
    
    return;
}













