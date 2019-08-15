#ifndef _CEVENT_REACTOR_H_
#define _CEVENT_REACTOR_H_

/*�ϸ���*/
#define     CEVTRCT_STYPE_STRICT       1
/*�ݴ���*/
#define     CEVTRCT_STYPE_AFAULT       2


/*���������ؾ����㷨*/
typedef enum
{
    EVTRCT_SEVLB_MINFD=0,   /*��С������*/
    EVTRCT_SEVLB_HASHFD,    /*��FD��Hash�����㷨*/

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
