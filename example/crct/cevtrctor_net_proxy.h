#ifndef _CEVTRCTOR_NET_PROXY_H_
#define _CEVTRCTOR_NET_PROXY_H_


#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <crct/cevtrctor_net_iobuf.h>
#include <crct/cevtrctor_net_conn.h>
#include <memory>
#include <thread>
#include <chrono>

class CEvtrctNetProxy : public std::enable_shared_from_this<CEvtrctNetProxy>
{
public:
        int                 start(const pexm_proxy_cfg_s cfg);
        void                stop();
        void                dispatch();
        int                 messagepost(const int PipeFilterID, const    int Value, const void* Data,const int DataLen);
        
private:
        int                 init();
        int                 network_init();
        void                network_uninit();
        int                 msque_init();
        void                msque_uninit();
        int                 timer_init();
        void                timer_uninit();
public:
        uint32_t            GetSlaveNums();
        cevt_net_slave_sptr GetSlaveByIndex(int32_t HashIndex);
        
private:
        int32_t             slave_task_init();
        void                slave_task_uninit();
        cevt_net_slave_sptr m_arry_slaver[32];
        uint32_t            m_arry_slave_nums_;
        
private:
        void*               m_rctor_;
        int                 m_taskid_;
        int                 m_fliterid_;
        unsigned int        m_msqsize_;
        int                 m_listenfd_;
        int                 m_listenport_;
        std::string         m_listenaddr_;
        
        int                 m_forward_port_;
        std::string         m_forward_addr_;
        VRCT_NETEVT_OPT_S   m_netopts_;
        VRCT_MSQ_OPT_S      m_msqopts_;
        VRCT_TIMER_OPT_S    m_timeropts_;
public:
        CEvtrctNetProxy(const CEvtrctNetProxy&) = delete;
        CEvtrctNetProxy& operator=(const CEvtrctNetProxy&) = delete;
        CEvtrctNetProxy();
        ~CEvtrctNetProxy();
};



#endif