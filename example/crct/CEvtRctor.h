#ifndef _CEVENT_REACTOR_H_
#define _CEVENT_REACTOR_H_

#include <memory>

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

class CEvtRctor : public std::enable_shared_from_this<CEvtRctor> {
public:
    std::shared_ptr<CEvtRctor>      GetSharedPtr();
    int                             GetMsqSize();
    
public:
    int                             Init(int MsqSize);
    void                            UnInit();
    
private:
    int                             m_msqSize_;
public:
    CEvtRctor(int MsqSize);
    CEvtRctor() = delete;
    CEvtRctor(const CEvtRctor&) = delete;
    CEvtRctor& operator=(const CEvtRctor&) = delete;
    ~CEvtRctor();
};

#endif

