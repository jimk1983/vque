#ifndef _CEVTRCTOR_NET_SERVER_H_
#define _CEVTRCTOR_NET_SERVER_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <crct/cevtrctor_net_iobuf.h>
#include <crct/cevtrctor_net_conn.h>

#include <memory>
#include <thread>
#include <chrono>


class CEvtrctNetServer : public std::enable_shared_from_this<CEvtrctNetServer>
{
public:
        int                 start(const pexm_serv_cfg_s cfg);
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
        int                 m_taskid_;
        int                 m_fliterid_;
        unsigned int        m_msqsize_;
        uint32_t            m_head_magic_;
        uint32_t            m_head_offset_;
private:
        int                 m_listenfd_;
        uint32_t            m_listenport_;
        std::string         m_listenaddr_;
        uint32_t            m_echo_enable_;
        uint32_t            m_forward_enable_;
        uint32_t            m_forward_port_;
        std::string         m_forward_addr_;
public:
        void*               m_rctor_;
        VRCT_NETEVT_OPT_S   m_netopts_;
        VRCT_MSQ_OPT_S      m_msqopts_;
        VRCT_TIMER_OPT_S    m_timeropts_;
public:
        CEvtrctNetServer(const CEvtrctNetServer&) = delete;
        CEvtrctNetServer& operator=(const CEvtrctNetServer&) = delete;
        CEvtrctNetServer();
        ~CEvtrctNetServer();
};


#endif
