
#include <memory>
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>
#include <crct/CEvtRctor.h>

int main(int argc, char *argv[])
{
    {
        CEvtRctor aa(10);
        std::cout << "11-" << aa.GetMsqSize() <<std::endl;
        aa.Init(1000);
        std::cout << "22-" << aa.GetMsqSize() <<std::endl;
        
    }

    std::weak_ptr<CEvtRctor> ptrweak;
{
    std::shared_ptr<CEvtRctor> ptrInit = nullptr;
    {
        std::shared_ptr<CEvtRctor> ptr=std::make_shared<CEvtRctor>(100);
        
        std::cout << "33-" << ptr->GetMsqSize() <<std::endl;
        ptr->Init(1000);
        
        std::cout << "44-" << ptr->GetMsqSize() <<std::endl;
        std::cout << ptr.use_count() << std::endl;
        ptrInit = ptr;
        std::cout << "55-" << ptrInit->GetMsqSize() <<std::endl;
        std::cout << ptr.use_count() << std::endl;
        ptrweak = ptr->GetSharedPtr();
        std::cout << ptr.use_count() << std::endl;
    }
    std::cout << "66-" << ptrweak.use_count() <<std::endl;
    
    std::cout << "77-" << ptrInit->GetMsqSize() <<std::endl;
    std::cout << ptrweak.use_count() << std::endl;
    std::shared_ptr<CEvtRctor> ptrTmp=ptrweak.lock();
    if (ptrTmp != nullptr)
    {
        std::cout << "88-" << ptrTmp->GetMsqSize() <<std::endl;
    }
    printf("hello world! ...\n");
    std::cout << ptrInit.use_count() << std::endl;
    std::cout << "99-" << ptrInit->GetMsqSize() <<std::endl;
}
    
    std::cout << ptrweak.use_count() << std::endl;
    std::shared_ptr<CEvtRctor> ptrTmp=ptrweak.lock();
    if (ptrTmp == nullptr)
        {
        std::cout << "evtreactor is nullptr!" << std::endl;
        }
    printf("hello world!\n");

    return 0;
}
