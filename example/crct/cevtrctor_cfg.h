#ifndef _CEVTRCTOR_CFG_H_
#define _CEVTRCTOR_CFG_H_



typedef struct tagCEvtRctorConfig
{
    std::string         addr;
    std::string         port;
    
}CERCT_SEVER_CFG_S, *PCERCT_SEVER_CFG_S;



class CEvtRctorCfg{
        
public:
    CERCT_SEVER_CFG_S           stSeverCfg;
    
    
public:
    CEvtRctorCfg(){};
    ~CEvtRctorCfg(){};

};



#endif
