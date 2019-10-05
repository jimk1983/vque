#ifndef _CEVTRCTOR_NET_SLAVE_H_
#define _CEVTRCTOR_NET_SLAVE_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <crct/cevtrctor_net_iobuf.h>

#include <memory>
#include <thread>
#include <chrono>
#include <list>
#include <iostream>

typedef enum
{
    SLAVE_MSG_UNKNOW = 0,
    SLAVE_MSG_DISPATH_CLNT,
    
}SLAVE_MSG_TYPE_E;

typedef struct tagSlaveMsgDispatchConnection
{
    int32_t             fd;             /*描述符*/
    struct sockaddr_in  ServNAddr;      /*所属服务器地址*/
    struct in_addr      ClntNAddr;      /*客户端地址*/
    uint32_t            ClntPort;       /*客户端端口信息*/
}slave_msgdpconn_s, *pslave_msgdpconn_s;

class CEvtrctNetSlave: public std::enable_shared_from_this<CEvtrctNetSlave>
{
public:
    void*               m_Rctor;
    int                 m_taskid;
    uint32_t            m_msqsize;
    cevt_net_conn_sptr  m_arryconns[ULIMITD_MAXFD];
    int32_t             m_echo_enable;
    int32_t             m_forward_enable;
    std::string         m_forward_addr;
    int32_t             m_forward_port;
    
private:
    VRCT_MSQ_OPT_S      m_msqopts_;
    uint32_t            m_fliterid_;
    PVOS_HASH_TABLE_S   m_conn_hashtbl_;
public:
    int32_t             init(const uint32_t echo_enable, const uint32_t forward_enable);
    void                info_set(const std::string& forward_Addr, const uint32_t& forward_Port);
    int32_t             start();
    void                uninit();
    int32_t             dispatch_connect(const int& fd,const struct in_addr& ClntNAddr,const uint32_t& ClntPort);
public:
    CEvtrctNetSlave():m_msqsize(1024),m_echo_enable(0),m_forward_enable(0){};
    ~CEvtrctNetSlave(){};
};

#endif

