#ifndef _CEVTRCTOR_NET_SERVER_H_
#define _CEVTRCTOR_NET_SERVER_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <memory>
#include <thread>
#include <chrono>

class CEvtrctNetServer : public std::enable_shared_from_this<CEvtrctNetServer>
{
public:
        int                 start(const CEvtRctorCfg& cfg);
        void                stop();
        void                dispatch();
        int                 messagepost(const int PipeFilterID, const    int Value, const void* Data,const int DataLen);
private:
        int                 network_init(const CEvtRctorCfg& cfg);
        void                network_uninit();
        int                 msque_init(const CEvtRctorCfg& cfg);
        void                msque_uninit();
        int                 timer_init(const CEvtRctorCfg& cfg);
        void                timer_uninit();
private:
        void*               m_rctor_;
        int                 m_taskid_;
        int                 m_fliterid_;
        unsigned int        m_msqsize_;
        int                 m_listenfd_;
        int                 m_listenport_;
        std::string         m_listenaddr_;
        uint32_t            m_echo_enable_;
        
        VRCT_NETEVT_OPT_S   m_netopts_;
        VRCT_MSQ_OPT_S      m_msqopts_;
        VRCT_TIMER_OPT_S    m_timeropts_;
public:
        CEvtrctNetServer(const CEvtrctNetServer&) = delete;
        CEvtrctNetServer& operator=(const CEvtrctNetServer&) = delete;
        CEvtrctNetServer();
        ~CEvtrctNetServer();
};

typedef std::shared_ptr<CEvtrctNetServer> cevt_net_server_sptr;  

#endif