#ifndef _CEVENT_REACTOR_H_
#define _CEVENT_REACTOR_H_

/*�ϸ���*/
#define     CEVTRCT_STYPE_STRICT       1
/*�ݴ���*/
#define     CEVTRCT_STYPE_AFAULT       2

class CEvtRctor
{
public:
    int     Init(int MsqSize);
    void    UnInit();
    

private:
    int     m_msqSize;
    
public:
    CEvtRctor(){};
    ~CEvtRctor(){};
};

#endif
