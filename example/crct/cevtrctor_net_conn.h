#ifndef _CEVTRCTOR_NET_CONN_H_
#define _CEVTRCTOR_NET_CONN_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <memory>
#include <thread>
#include <chrono>
#include <list>

typedef enum
{
    CONN_STATUS_INIT=0,
    CONN_STATUS_CONNECTING,
    CONN_STATUS_CONNECTED,
}CONN_STATUS_E;


#define  ULIMITD_MAXFD      40960

class CEvtrctNetConn;
class CEvtrctNetSlave;

typedef std::shared_ptr<CEvtrctNetConn>     evt_netconn_sptr;
typedef std::shared_ptr<CEvtrctNetSlave>    evt_slave_sptr;

#define     EXTERN_C_STYLE       0

class CVosIobuf
{
public:
    int32_t         m_size;
    VOS_IOBUF_S*    m_pstIobuf;
public:
    CVosIobuf(int32_t buf_size);
    ~CVosIobuf();
};

typedef std::shared_ptr<CVosIobuf>          vos_iobuf_sptr;

class CEvtrctNetConn
{
public:
    int32_t                 m_Fd;                        
    //DULONG                stRandID;                   /*唯一ID*/
    UCHAR                   m_Guid[VOS_GUID_LEN];       
    int32_t                 m_iConnStatus;                /*连接状态*/
    int32_t                 m_lErrorCode;                 /*老化时的错误码*/
    uint32_t                m_iBodySize;                  
    volatile int32_t        m_iRcvNums;    
    
    #if EXTERN_C_STYLE
    //VOS_DLIST_S           m_stRecvList;  
    //VOS_IOBUF_S*          m_pstRecvIobuf;
    //VOS_IOBUF_S*          m_pstRecvOldIobuf;            /*上个的遗留IO*/
    //VOS_DLIST_S           m_stSendList;
    //VOS_IOBUF_S*          m_pstSendIobuf;
    #else
    
    std::list<vos_iobuf_sptr>   m_stRecvList;
    vos_iobuf_sptr              m_pstRecvIobuf;
    vos_iobuf_sptr              m_pstRecvOldIobuf;
    
    std::list<vos_iobuf_sptr>   m_stSendList;
    vos_iobuf_sptr              m_pstSendIobuf;
    #endif
    
    int32_t                 m_uiSndBlockCount;
    volatile int32_t        m_iSndNums;
    struct sockaddr_in      m_stServAddr;                 /*服务器地址*/
    struct timeval          m_stStartTime;                
    struct timeval          m_stStopTime;                 
    struct in_addr          m_ClntAddr;                   /*客户端地址*/
    uint32_t                m_ClntPort;                   /*客户端端口*/
    
    void*                   m_rctor_;
    VRCT_NETEVT_OPT_S       m_netopts_;
    CEvtrctNetSlave*        m_slave_ptr;
    
    uint32_t                m_rx_flows;
    uint32_t                m_tx_flows;

    int32_t                 m_echo_enable;
    int32_t                 m_forward_enable;
public:
    int32_t     netconn_create(CEvtrctNetSlave* slave, int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort);
    void        netconn_release();
public:
    static void net_conn_sendcb(int ifd, void *pvCtx);
    static void net_conn_recvcb(int ifd, void *pvCtx);
public:
    CEvtrctNetConn();
    ~CEvtrctNetConn();
};

class CEvtrctNetSlave
{
public:
    void*               m_Rctor;
    int                 m_taskid;
    uint32_t            m_msqsize;
    evt_netconn_sptr    m_arryconns[ULIMITD_MAXFD];
    int32_t             m_echo_enable;
    int32_t             m_forward_enable;
private:
    VRCT_MSQ_OPT_S      m_msqopts_;
    uint32_t            m_fliterid_;
    struct sockaddr_in  m_sevNAddr;
    PVOS_HASH_TABLE_S   m_conn_hashtbl;
public:
    int32_t             init();
    int32_t             start();
    void                uninit();
    int32_t             dispatch_connect(int fd, struct in_addr ClntNAddr, uint32_t uiClntPort);
public:
    CEvtrctNetSlave(){};
    ~CEvtrctNetSlave(){};
};


#endif
