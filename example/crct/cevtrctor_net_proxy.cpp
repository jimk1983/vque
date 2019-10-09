
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include <config/config.h>
#include "cevtrctor_def.h"
#include "cevtrctor_net_conn.h"
#include "cevtrctor_net_slave.h"
#include "cevtrctor_net_proxy.h"

#define     SLAVE_PROXY_TASKID          200


static void accept_cb(int fd, void *pvArgv)
{
    CEvtrctNetProxy* net_dispatch = (CEvtrctNetProxy*)pvArgv;
    net_dispatch->dispatch();
}

static void timer_cb(void *pvArgv)
{
    
}

static void msqctrl_cb(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    
}

uint32_t    CEvtrctNetProxy::GetSlaveNums()
{
    return m_arry_slave_nums_;
}

cevt_net_slave_sptr  CEvtrctNetProxy::GetSlaveByIndex(int32_t HashIndex)
{
    return m_arry_slaver[HashIndex];
}

int32_t    CEvtrctNetProxy::slave_task_init()
{
    //m_arry_slave_nums_ = VOS_GetCpuCoreNum()*2;
    
    
    std::cout << "slave task init()" << std::endl;
    
    for (uint32_t i = 0; i < m_arry_slave_nums_; i++)
    {
        m_arry_slaver[i] =std::make_shared<CEvtrctNetSlave>();
        m_arry_slaver[i]->m_taskid = SLAVE_PROXY_TASKID + i;
        m_arry_slaver[i]->m_msqsize= 1000;
        if (VOS_ERR == m_arry_slaver[i]->init(0, 1) )
        {
            printf("slave task init error!\n");
            return VOS_ERR;
        }
        else
        {
            m_arry_slaver[i]->info_set(m_forward_addr_, m_forward_port_);
            (void)m_arry_slaver[i]->start();
        }
        //std::cout << "1 rctor count=" << m_arry_slaver[i].use_count() << std::endl;
        //m_arry_slaver[i]->uninit();
        //m_arry_slaver[i] = nullptr;
        //std::cout << "2 rctor count=" << m_arry_slaver[i].use_count() << std::endl;
    }

    return VOS_OK;
}

void    CEvtrctNetProxy::slave_task_uninit()
{
    for (uint32_t i = 0; i < m_arry_slave_nums_; i++)
    {
        m_arry_slaver[i]->uninit();
        m_arry_slaver[i] = nullptr;
        //std::cout << "rctor count=" << m_arry_slaver[i].use_count() << std::endl;
    }
    
}

void    CEvtrctNetProxy::dispatch()
{
    struct sockaddr_in  stClientAddr    = {0};
    socklen_t           stSerLen        = sizeof(stClientAddr);
    int32_t             lClientFd       = 0;
    unsigned short      usClientPort    = 0;
    char                acClientAddr[32]={0};
    int32_t             hashIndex       = 0;
    
    lClientFd = accept(m_listenfd_, (struct sockaddr *)&stClientAddr, &stSerLen);
    if( lClientFd < 0 )
    {
        if ( errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR )
        {
            return;
        }
        else
        {
            VRCT_API_NetworkOptUnRegister(m_rctor_, &m_netopts_);
            printf("[ESEVR] Terminal accept socket error=%d\n", errno);
            return;
        }
    }
    usClientPort = ntohs(stClientAddr.sin_port);
    
    memcpy(acClientAddr,inet_ntoa(stClientAddr.sin_addr), strlen(inet_ntoa(stClientAddr.sin_addr)));
    
    hashIndex = lClientFd % GetSlaveNums();
    if(nullptr == GetSlaveByIndex(hashIndex))
    {
        printf("some slave has error!\n");
        hashIndex = 0;
    }
    
    if( VOS_ERR == GetSlaveByIndex(hashIndex)->dispatch_connect(lClientFd, stClientAddr.sin_addr, usClientPort))
    {
        printf("proxy-server: slave dispatch connect failed!lClientFd=%d\n", lClientFd);
        VOS_SOCK_Close(lClientFd);
        return;
    } 
}

int     CEvtrctNetProxy::network_init()
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

void     CEvtrctNetProxy::network_uninit()
{
    if ( -1 != m_listenfd_ )
    {
        VRCT_API_NetworkOptUnRegister(m_rctor_, &m_netopts_);
        close(m_listenfd_);
        m_listenfd_ = -1;
    }
}

int     CEvtrctNetProxy::msque_init()
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

void     CEvtrctNetProxy::msque_uninit()
{
    VRCT_API_MsqOptUnRegister(m_rctor_, &m_msqopts_);
}

int      CEvtrctNetProxy::messagepost(const int PipeFilterID, const    int Value, const void* Data,const int DataLen)
{
    return VRCT_API_MsqOptPush(m_rctor_, (UINT32_T)PipeFilterID, (UINT32_T)Value, (CHAR *)Data, (UINT32_T)DataLen);
}

int     CEvtrctNetProxy::timer_init()
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

void     CEvtrctNetProxy::timer_uninit()
{
    VRCT_API_TImerOptUnRegister(m_rctor_, &m_timeropts_);
}

int     CEvtrctNetProxy::start(const pexm_proxy_cfg_s cfg)
{
    m_listenport_       = cfg->LocalPort;
    m_listenaddr_       = (char *)cfg->acLocalAddr;
    m_forward_port_     = cfg->ProxyPort;
    m_forward_addr_     = (char *)cfg->acProxyAddr;
    m_arry_slave_nums_  = cfg->ProxyPthNums;
    
    std::cout << "local addr=" << m_listenaddr_ << std::endl;
    std::cout << "local port=" << m_listenport_ << std::endl;
    std::cout << "listen addr=" << m_forward_addr_ << std::endl;
    std::cout << "listen port=" << m_forward_port_ << std::endl;
    std::cout << "slave nums=" << m_arry_slave_nums_ << std::endl;
    
    init();
    
    return 0;
}

int     CEvtrctNetProxy::init()
{
    if( VOS_ERR == slave_task_init() )
    {
        std::cout <<"slave_task_init error!" << std::endl;
        return -1;
    }
    
    m_rctor_ = VRCT_API_Create(m_taskid_, m_msqsize_);
    if ( NULL == m_rctor_ )
    {
        std::cout <<"[LISTN] VRCT create error!" << std::endl;
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
    
    if ( VOS_ERR == VRCT_API_Start(m_rctor_)  )
    {
        std::cout <<"vos reactor start failed!" << std::endl;
        VRCT_API_Release(&m_rctor_);
        return -1;
    }
    
    return 0;
}


void    CEvtrctNetProxy::stop()
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
    
CEvtrctNetProxy::CEvtrctNetProxy():
        m_rctor_(NULL),
        m_taskid_(0),
        m_fliterid_(0),
        m_msqsize_(1000),
        m_listenfd_(-1),
        m_listenport_(9527)
{
    std::cout << "net forward entry!" << std::endl;
}

CEvtrctNetProxy::~CEvtrctNetProxy()
{
    std::cout << "net forward leave!" << std::endl;
}


