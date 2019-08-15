
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

    std::weak_ptr<CEvtRctor> ptrweak;
{
    std::shared_ptr<CEvtRctor> ptrInit = nullptr;
    {
        std::shared_ptr<CEvtRctor> ptr=std::make_shared<CEvtRctor>(100);
        ptr->Init(100);
        std::cout << ptr.use_count() << std::endl;
        ptrInit = ptr;
        std::cout << ptr.use_count() << std::endl;
        ptrweak = ptr;
        std::cout << ptr.use_count() << std::endl;
    }
    
    printf("hello world! ...\n");
    std::cout << ptrInit.use_count() << std::endl;
}
    
    std::cout << ptrweak.use_count() << std::endl;
    printf("hello world!\n");

    return 0;
}