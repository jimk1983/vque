#ifndef _CEVTRCTOR_NET_CONN_H_
#define _CEVTRCTOR_NET_CONN_H_


#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <crct/cevtrctor_net_iobuf.h>

#define     ULIMITD_MAXFD       40960
#define     EXTERN_C_STYLE      0


class CEvtrctNetConn: public std::enable_shared_from_this<CEvtrctNetConn>
{
public:
    int32_t                     m_Fd;                        
    //DULONG                    stRandID;                       /*唯一ID*/
    UCHAR                       m_Guid[VOS_GUID_LEN];       
    int32_t                     m_lErrorCode;                   /*老化时的错误码*/
    uint32_t                    m_iBodySize;                  
    volatile int32_t            m_iRcvNums;                 
    volatile int32_t            m_conn_status;              
    
    #if EXTERN_C_STYLE
    //VOS_DLIST_S           m_stRecvList;  
    //VOS_IOBUF_S*          m_pstRecvIobuf;
    //VOS_IOBUF_S*          m_pstRecvOldIobuf;              /*上个的遗留IO*/
    //VOS_DLIST_S           m_stSendList;
    //VOS_IOBUF_S*          m_pstSendIobuf;
    #else
    
    std::list<vos_iobuf_sptr>   m_stRecvList;
    vos_iobuf_sptr              m_pstRecvIobuf;
    vos_iobuf_sptr              m_pstRecvOldIobuf;
    
    std::list<vos_iobuf_sptr>   m_stSendList;
    vos_iobuf_sptr              m_pstSendIobuf;
    #endif
    
    int32_t                     m_uiSndBlockCount;
    volatile int32_t            m_iSndNums;
    struct timeval              m_stStartTime;                
    struct timeval              m_stStopTime;                 
    struct in_addr              m_ClntAddr;                 /*客户端地址*/
    uint32_t                    m_ClntPort;                 /*客户端端口*/
    
    void*                       m_rctor_;
    VRCT_NETEVT_OPT_S           m_netopts_;
    CEvtrctNetSlave*            m_slave_ptr;
    
    uint32_t                    m_rx_flows;
    uint32_t                    m_tx_flows;
    int32_t                     m_echo_enable;
    
    int32_t                     m_forward_fd;
    struct sockaddr_in          m_forward_addr;
    int32_t                     m_forward_port;
    int32_t                     m_forward_enable;
    int32_t                     m_peer_fd;
    
public:
    int32_t     netconn_create(CEvtrctNetSlave* slave, int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort);
    int32_t     netconn_create(CEvtrctNetSlave* slave, const std::string& serv_addr, int32_t serv_port);
    void        netconn_release();
public:
    int32_t     netconn_clnt_create(const CEvtrctNetSlave* slave, int32_t pair_fd);
    
public:
    static void net_conn_send_cb(int ifd, void *pvCtx);
    static void net_conn_recv_cb(int ifd, void *pvCtx);
public:
    CEvtrctNetConn();
    ~CEvtrctNetConn();
};


#endif
