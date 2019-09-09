#ifndef _CEVTRCTOR_NET_CONN_H_
#define _CEVTRCTOR_NET_CONN_H_

#include <crct/cevtrctor_def.h>
#include <crct/cevtrctor_cfg.h>
#include <memory>
#include <thread>
#include <chrono>

#define  ULIMITD_MAXFD      40960

class CEvtrctNetConn;
class CEvtrctNetSlave;

typedef std::shared_ptr<CEvtrctNetConn>     evt_netconn_sptr;
typedef std::shared_ptr<CEvtrctNetSlave>    evt_slave_sptr;

class CEvtrctNetConn
{
public:
    int32_t                 lFd;                        
    //DULONG                  stRandID;                   /*ΨһID*/
    UCHAR                   Guid[VOS_GUID_LEN];       
    int32_t                 iConnStatus;                /*����״̬*/
    int32_t                 lErrorCode;                 /*�ϻ�ʱ�Ĵ�����*/
    uint32_t                iBodySize;                  
    VOS_DLIST_S             stRecvList;  
    int32_t                 iRcvNums;    
    VRCT_IOBUF_S*           pstRecvIobuf;
    VRCT_IOBUF_S*           pstRecvOldIobuf;            /*�ϸ�������IO*/
    
    VOS_DLIST_S             stSendList;
    volatile int32_t        iSndNums;
    VRCT_IOBUF_S*           pstSendIobuf;
    int32_t                 uiSndBlockCount;
    
    void*                   pvQue;                      
    struct sockaddr_in      stServAddr;                 /*��������ַ*/
    struct timeval          stStartTime;                
    struct timeval          stStopTime;                 
    struct in_addr          ClntAddr;                   /*�ͻ��˵�ַ*/
    uint32_t                ClntPort;                   /*�ͻ��˶˿�*/
public:
    int32_t netconn_create(void*pvRctor, int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort);
    
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
