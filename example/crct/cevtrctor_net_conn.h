#ifndef _CEVTRCTOR_NET_CONN_H_
#define _CEVTRCTOR_NET_CONN_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <memory>
#include <thread>
#include <chrono>


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

class CEvtrctNetConn
{
public:
    int32_t                 m_Fd;                        
    //DULONG                stRandID;                   /*唯一ID*/
    UCHAR                   m_Guid[VOS_GUID_LEN];       
    int32_t                 m_iConnStatus;                /*连接状态*/
    int32_t                 m_lErrorCode;                 /*老化时的错误码*/
    uint32_t                m_iBodySize;                  
    VOS_DLIST_S             m_stRecvList;  
    int32_t                 m_iRcvNums;    
    VRCT_IOBUF_S*           m_pstRecvIobuf;
    VRCT_IOBUF_S*           m_pstRecvOldIobuf;            /*上个的遗留IO*/
    
    VOS_DLIST_S             m_stSendList;
    volatile int32_t        m_iSndNums;
    VRCT_IOBUF_S*           m_pstSendIobuf;
    int32_t                 m_uiSndBlockCount;
    struct sockaddr_in      m_stServAddr;                 /*服务器地址*/
    struct timeval          m_stStartTime;                
    struct timeval          m_stStopTime;                 
    struct in_addr          m_ClntAddr;                   /*客户端地址*/
    uint32_t                m_ClntPort;                   /*客户端端口*/
    
    void*                   m_rctor_;
    VRCT_NETEVT_OPT_S       m_netopts_;
    CEvtrctNetSlave*        m_slave_ptr;
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
