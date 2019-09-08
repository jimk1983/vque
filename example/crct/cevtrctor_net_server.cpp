
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include "config/config.h"
#include "cevtrctor_def.h"
#include "cevtrctor_net_conn.h"
#include "cevtrctor_net_server.h"

#define     SLAVE_START_TASKID          100

static void accept_cb(int fd, void *pvArgv)
{
    CEvtrctNetServer*   net_sev         = (CEvtrctNetServer*)pvArgv;
    struct sockaddr_in  stClientAddr    = {0};
    socklen_t           stSerLen        = sizeof(stClientAddr);
    int32_t             lClientFd       = 0;
    unsigned short      usClientPort    = 0;
    char                acClientAddr[32]={0};
    
    if ( NULL == pvArgv )
    {
        return;
    }
    
    lClientFd = accept(fd, (struct sockaddr *)&stClientAddr, &stSerLen);
    if( lClientFd < 0 )
    {
        if ( errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR )
        {
            return;
        }
        else
        {
            VRCT_API_NetworkOptUnRegister(net_sev->m_rctor_, &net_sev->m_netopts_);
            printf("[ESEVR] Terminal accept socket error=%d\n", errno);
            return;
        }
    }
    usClientPort = ntohs(stClientAddr.sin_port);
    
    memcpy(acClientAddr,inet_ntoa(stClientAddr.sin_addr), strlen(inet_ntoa(stClientAddr.sin_addr)));
    printf("Terminal accept a new client: %s:%d,fd=%d\n", 
        acClientAddr, 
        usClientPort, 
        lClientFd);
    
    close(lClientFd);
    //net_dispatch->dispatch();
}

static void timer_cb(void *pvArgv)
{
    //CEvtrctNetServer* net_dispatch = (CEvtrctNetServer*)pvArgv;
    
    //net_dispatch->messagepost(0, 222 ,NULL, 0);
    
}
    
static void msqctrl_cb(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    //CEvtrctNetServer* net_dispatch = (CEvtrctNetServer*)pvCtx;
    
    //net_dispatch->dispatch();
}


void    CEvtrctNetServer::dispatch()
{
    //std::this_thread::sleep_for(std::chrono::seconds(1)); 
}

int     CEvtrctNetServer::network_init()
{
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

int     CEvtrctNetServer::msque_init()
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


int     CEvtrctNetServer::timer_init()
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


void    CEvtrctNetServer::slave_task_init()
{
    m_arry_slave_nums_ = VOS_GetCpuCoreNum()*2;
    
    std::cout << "slave task init()" << std::endl;
    
    for (uint32_t i = 0; i < m_arry_slave_nums_; i++)
    {
        m_arry_slaver[i] =std::make_shared<CEvtrctNetSlave>();
        m_arry_slaver[i]->m_taskid = SLAVE_START_TASKID + i;
        m_arry_slaver[i]->m_msqsize=1000;
        m_arry_slaver[i]->init();
        //std::cout << "1 rctor count=" << m_arry_slaver[i].use_count() << std::endl;
        //m_arry_slaver[i]->uninit();
        //m_arry_slaver[i] = nullptr;
        //std::cout << "2 rctor count=" << m_arry_slaver[i].use_count() << std::endl;
    }
}

void    CEvtrctNetServer::slave_task_uninit()
{
    for (uint32_t i = 0; i < m_arry_slave_nums_; i++)
    {
        m_arry_slaver[i]->uninit();
        m_arry_slaver[i] = nullptr;
        //std::cout << "rctor count=" << m_arry_slaver[i].use_count() << std::endl;
    }
    
}

void     CEvtrctNetServer::timer_uninit()
{
    VRCT_API_TImerOptUnRegister(m_rctor_, &m_timeropts_);
}
    
int     CEvtrctNetServer::start(const pexm_serv_cfg_s cfg)
{
    m_listenport_       = cfg->Port;
    m_listenaddr_       = (char *)cfg->acAddr;
    m_echo_enable_      = cfg->EchoEnalbe;
    m_head_magic_       = cfg->HeadMagic;
    m_head_offset_      = cfg->HeadOffset;
    
    std::cout << "listenaddr=" << m_listenaddr_ << std::endl;
    std::cout << "listenport=" << m_listenport_ << std::endl;
    std::cout << "head_magic=" << m_head_magic_ << std::endl;
    std::cout << "head_offset=" << m_head_offset_ << std::endl;
    std::cout << "echo_enable=" << m_echo_enable_ << std::endl;
    
    slave_task_init();
    
    m_rctor_ = VRCT_API_Create(m_taskid_, m_msqsize_);
    if ( NULL == m_rctor_ )
    {
        std::cout <<"[LISTN] vrct create error!" << std::endl;
        return -1;
    }
    
    if ( 0 != network_init() )
    {
        std::cout <<"[LISTN] network init error!" << std::endl;
        VRCT_API_Release(&m_rctor_);
        return -1;
    }
    
    if ( 0 != timer_init() )
    {
        std::cout <<"[LISTN] timer init error!" << std::endl;
        network_uninit();
        VRCT_API_Release(&m_rctor_);
        return -1;
    }
    
    if ( 0 != msque_init() )
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
    
    slave_task_uninit();
    
    if ( -1 != m_listenfd_ )
    {
        close(m_listenfd_);
        m_listenfd_ = -1;
    }
}
    
CEvtrctNetServer::CEvtrctNetServer():
        m_taskid_(0),
        m_fliterid_(0),
        m_msqsize_(1000),
        m_listenfd_(-1),
        m_listenport_(9527),
        m_rctor_(NULL)
{
    std::cout << "net server entry!" << std::endl;
}
    
CEvtrctNetServer::~CEvtrctNetServer()
{
    std::cout << "net server leave!" << std::endl;
}


