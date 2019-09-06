
#include <memory>
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include <crct/cevtrctor_cfg.h>
#include <crct/cevtrctor_net_dispatch.h>

int main(int argc, char *argv[])
{
    CEvtRctorCfg    aa;
    aa.stSeverCfg.addr = "192.168.2.3";

    std::shared_ptr<CEvtrctNetDispatch> rctor_sptr = std::make_shared<CEvtrctNetDispatch>();
    std::cout << "rctor_sptr=" << rctor_sptr << std::endl;
    rctor_sptr->start(aa);

    while(1)
        {
        
    char *ptr=  (char *)malloc(1024*1024*500);
    printf("ptr=%p\n", ptr);
        VOS_Sleep(1);
        }
    rctor_sptr->stop();
    
    getchar();
    return 0;
}
