
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include "cevtrctor_def.h"
#include "cevtrctor_cfg.h"
#include "cevtrctor_net_server.h"


static void accept_cb(int fd, void *pvArgv)
{
    CEvtrctNetServer* net_dispatch = (CEvtrctNetServer*)pvArgv;
    
    //printf("accept_cb entry!\n");
    
    net_dispatch->dispatch();
}

static void timer_cb(void *pvArgv)
{
    CEvtrctNetServer* net_dispatch = (CEvtrctNetServer*)pvArgv;
    
    //printf("timer_cb entry!\n");
    
    net_dispatch->messagepost(0, 222 ,NULL, 0);
    
}
    
static void msqctrl_cb(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    CEvtrctNetServer* net_dispatch = (CEvtrctNetServer*)pvCtx;
    
    //printf("msqctrl_cb entry! Value=%d\n", Value);
    
    net_dispatch->dispatch();
}

void    CEvtrctNetServer::dispatch()
{
    
    //std::this_thread::sleep_for(std::chrono::seconds(1)); 
}

int     CEvtrctNetServer::network_init(const CEvtRctorCfg& cfg)
{
    m_listenport_ = std::atoi(cfg.stSeverCfg.port.c_str());
    m_listenaddr_ = cfg.stSeverCfg.addr;
    m_listenfd_   = VOS_SOCK_ServCreate(NULL, m_listenport_);
    if ( SYS_ERR == m_listenfd_ )
    {
        std::cout << "[LISTN] VRCT server socket fd create failed! port=" << m_listenport_ << std::endl;
        
        return -1;
    }
    
    VRCT_NETOPT_INIT(&m_netopts_,
                    m_listenfd_,
                    VRCT_POLL_LTIN,
                    accept_cb,
                    NULL,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_NetworkOptRegister(m_rctor_, &m_netopts_) )
    {
        std::cout <<"[LISTN] VRCT network register error!" << std::endl;
        close(m_listenfd_);
        return -1;
    }
    
    return 0;
}

void     CEvtrctNetServer::network_uninit()
{
    if ( -1 != m_listenfd_ )
    {
        VRCT_API_NetworkOptUnRegister(m_rctor_, &m_netopts_);
        close(m_listenfd_);
        m_listenfd_ = -1;
    }
}

int     CEvtrctNetServer::msque_init(const CEvtRctorCfg& cfg)
{
    VRCT_MSQOPT_INIT(&m_msqopts_,
                     m_fliterid_,
                     msqctrl_cb,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_MsqOptRegister(m_rctor_, &m_msqopts_) )
    {
        std::cout <<"[LISTN] VRCT msg queue register error!" << std::endl;
        return -1;
    }
    
    return 0;
}

void     CEvtrctNetServer::msque_uninit()
{
    VRCT_API_MsqOptUnRegister(m_rctor_, &m_msqopts_);
}

int      CEvtrctNetServer::messagepost(const int PipeFilterID, const    int Value, const void* Data,const int DataLen)
{
    return VRCT_API_MsqOptPush(m_rctor_, (UINT32_T)PipeFilterID, (UINT32_T)Value, (CHAR *)Data, (UINT32_T)DataLen);
}


int     CEvtrctNetServer::timer_init(const CEvtRctorCfg& cfg)
{
    VRCT_TIMEROPT_INIT(&m_timeropts_,
                    VRCT_TMTYPE_RECYLE,
                    1,
                    timer_cb,
                    (void *)this);
    if ( SYS_ERR == VRCT_API_TimerOptRegister(m_rctor_, &m_timeropts_) )
    {
        std::cout <<"[LISTN] VRCT timer register error!" << std::endl;
        return -1;
    }
    
    return 0;
}

void     CEvtrctNetServer::timer_uninit()
{
    VRCT_API_TImerOptUnRegister(m_rctor_, &m_timeropts_);
}
    
int     CEvtrctNetServer::start(const CEvtRctorCfg& cfg)
{
    m_rctor_ = VRCT_API_Create(m_taskid_, m_msqsize_);
    if ( NULL == m_rctor_ )
    {
        std::cout <<"[LISTN] VRCT create error!" << std::endl;
        return -1;
    }
    
    if ( 0 != network_init(cfg) )
    {
        std::cout <<"[LISTN] network init error!" << std::endl;
        VRCT_API_Release(&m_rctor_);
        return -1;
    }
    
    if ( 0 != timer_init(cfg) )
    {
        std::cout <<"[LISTN] timer init error!" << std::endl;
        network_uninit();
        VRCT_API_Release(&m_rctor_);
        return -1;
    }
    
    if ( 0 != msque_init(cfg) )
    {
        std::cout <<"[LISTN] message init error!" << std::endl;
        timer_uninit();
        network_uninit();
        VRCT_API_Release(&m_rctor_);
        return -1;
    }
    
    /*start*/
    if ( VOS_ERR == VRCT_API_Start(m_rctor_)  )
    {
        std::cout <<"vos reactor start failed!" << std::endl;
        timer_uninit();
        network_uninit();
        msque_uninit();
        VRCT_API_Release(&m_rctor_);
        return -1;
    }
    
    return 0;
}
    
void    CEvtrctNetServer::stop()
{
    std:: cout << "stop" << std::endl;
    
    if ( m_rctor_ != NULL )
    {
        VRCT_API_Release(&m_rctor_);
    }
    
    if ( -1 != m_listenfd_ )
    {
        close(m_listenfd_);
        m_listenfd_ = -1;
    }
}
    
CEvtrctNetServer::CEvtrctNetServer():
        m_rctor_(NULL),
        m_taskid_(0),
        m_fliterid_(0),
        m_msqsize_(1000),
        m_listenfd_(-1),
        m_listenport_(9527)
{
    std::cout << "net server entry!" << std::endl;
}
    
CEvtrctNetServer::~CEvtrctNetServer()
{
    std::cout << "net server leave!" << std::endl;
}


