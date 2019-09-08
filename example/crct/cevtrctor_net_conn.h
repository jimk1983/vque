#ifndef _CEVTRCTOR_NET_CONN_H_
#define _CEVTRCTOR_NET_CONN_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <memory>
#include <thread>
#include <chrono>

class CEvtrctNetConn;
class CEvtrctNetSlave;

typedef std::shared_ptr<CEvtrctNetConn>     evt_netconn_sptr;
typedef std::shared_ptr<CEvtrctNetSlave>    evt_slave_sptr;

class CEvtrctNetConn
{
public:
    int32_t                 lFd;                        
    //DULONG                  stRandID;                   /*唯一ID*/
    UCHAR                   Guid[VOS_GUID_LEN];       
    int32_t                 iConnStatus;                /*连接状态*/
    int32_t                 lErrorCode;                 /*老化时的错误码*/
    uint32_t                iBodySize;                  
    VOS_DLIST_S             stRecvList;  
    int32_t                 iRcvNums;    
    VRCT_IOBUF_S*           pstRecvIobuf;
    VRCT_IOBUF_S*           pstRecvOldIobuf;            /*上个的遗留IO*/
    
    VOS_DLIST_S             stSendList;
    volatile int32_t        iSndNums;
    VRCT_IOBUF_S*           pstSendIobuf;
    int32_t                 uiSndBlockCount;
    
    void*                   pvQue;                      
    struct sockaddr_in      stServAddr;                 /*服务器地址*/
    struct timeval          stStartTime;                
    struct timeval          stStopTime;                 
    struct in_addr          ClntAddr;                   /*客户端地址*/
    uint32_t                ClntPort;                   /*客户端端口*/
public:
    int32_t netconn_create(int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort);
    
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
    
private:
    VRCT_MSQ_OPT_S      m_msqopts_;
    uint32_t            m_fliterid_;

    PVOS_HASH_TABLE_S   m_conn_hashtbl;
public:
    void                init();
    void                uninit();
    void                dispatch_connetion(int fd, struct in_addr ClntNAddr, uint32_t uiClntPort);
public:
    CEvtrctNetSlave(){};
    ~CEvtrctNetSlave(){};
};


#endif
