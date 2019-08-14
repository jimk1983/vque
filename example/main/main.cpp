
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>
#include <crct/CEvtRctor.h>

using namespace CVEvtFrame;

int main(int argc, char *argv[])
{
    CEvtRctor *pEvtRctor = new CEvtRctor(10000);
    
    printf("hello world!\n");

    delete pEvtRctor;

    {
	CEvtRctor a(111);
        printf("a=%p\n",&a);
    }


    return 0;
}
