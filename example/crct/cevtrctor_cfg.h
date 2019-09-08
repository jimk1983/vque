#ifndef _CEVTRCTOR_CFG_H_
#define _CEVTRCTOR_CFG_H_


#include <memory>

typedef struct tagCEvtRctorConfig
{
    std::string         addr;
    uint32_t            port;
    uint32_t            echo_enable;
    
}CERCT_SEVER_CFG_S, *PCERCT_SEVER_CFG_S;



class CEvtRctorCfg{
        
public:
    CERCT_SEVER_CFG_S           stSeverCfg;
    
    
public:
    CEvtRctorCfg(){};
    ~CEvtRctorCfg(){};

};

typedef std::shared_ptr<CEvtRctorCfg>  cevtor_cfg_sptr;





#endif
