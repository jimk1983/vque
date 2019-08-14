#ifndef _CEVENT_REACTOR_H_
#define _CEVENT_REACTOR_H_

/*严格型*/
#define     CEVTRCT_STYPE_STRICT       1
/*容错型*/
#define     CEVTRCT_STYPE_AFAULT       2


/*服务器负载均衡算法*/
typedef enum
{
    EVTRCT_SEVLB_MINFD=0,   /*最小连接数*/
    EVTRCT_SEVLB_HASHFD,    /*简单FD的Hash求余算法*/

    EVTRCT_SEVLB_NUMS
}EVTRCT_SEVLB_E;
    
namespace CVEvtFrame{

    class CEvtRctor
    {
    public:


    private:
        void                EvtRctor_Init();
        void                EvtRctor_UnInit();
    private:
        int                 m_msqSize;
    public:
        CEvtRctor(int MsqSize);
        CEvtRctor() = delete;
        CEvtRctor(const CEvtRctor&) = delete;
        CEvtRctor& operator=(const CEvtRctor&) = delete;
        ~CEvtRctor();
    };

}
#endif
