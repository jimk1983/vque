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

class CEvtRctor 
{
public:
    int     Init(int MsqSize);
    void    UnInit();
    
    
private:
    int     m_msqSize;
    
public:
    explicit CEvtRctor(){std::cout << "CEvtRctor entry!" << std::endl;};
    explicit CEvtRctor(int MsqSize)  :m_msqSize(MsqSize){std::cout << "CEvtRctor entry!" << std::endl;};
    ~CEvtRctor(){std::cout << "CEvtRctor Leave!" << std::endl;};
    CEvtRctor& operator=(const CEvtRctor&) = delete;
};

#endif
