
#include <memory>
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>
#include <crct/CEvtRctor.h>


int main(int argc, char *argv[])
{
    {
        CEvtRctor aa;
        
        aa.Init(1000);
        
    }

    {
        std::shared_ptr<CEvtRctor> ptr=std::make_shared<CEvtRctor>(100);
        ptr->Init(100);
    }
    
    printf("hello world!\n");

    return 0;
}