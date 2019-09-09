#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include "config/config.h"
#include "cevtrctor_def.h"
#include "cevtrctor_net_conn.h"


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

int32_t  CEvtrctNetConn::netconn_create(void*pvRctor, int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort)
{
    
    return SYS_OK;
}


CEvtrctNetConn::CEvtrctNetConn()
{
    std::cout << "CEvtrctNetConn entry" << std::endl;
};

CEvtrctNetConn::~CEvtrctNetConn()
{

    std::cout << "~CEvtrctNetConn entry" << std::endl;
}
    
    
static void msqctrl_cb(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    CEvtrctNetSlave*    cevt_net_slave = (CEvtrctNetSlave*)pvCtx;
    printf("salve msqctl_cb entry! value=%d\n", Value);
    switch(Value)
    {
        case SLAVE_MSG_DISPATH_CLNT:
            {   
                pslave_msgdpconn_s pstmsg = (pslave_msgdpconn_s)pvMsg;
                printf("slave msqctrl_cb->fd=%d\n", pstmsg->fd);
                
                std::shared_ptr<CEvtrctNetConn> new_conn_sptr = std::make_shared<CEvtrctNetConn>();
                new_conn_sptr->netconn_create(cevt_net_slave->m_Rctor,pstmsg->fd, pstmsg->ClntNAddr, pstmsg->ClntPort);
                cevt_net_slave->m_arryconns[pstmsg->fd]= new_conn_sptr;
                
                free(pstmsg);
            }
            break;
        default:
            break;
    }
    
}

int32_t CEvtrctNetSlave::dispatch_connect(int fd, struct in_addr ClntNAddr, uint32_t ClntPort)
{
    pslave_msgdpconn_s      pstMsgData = NULL;
    
    pstMsgData = (pslave_msgdpconn_s)malloc(sizeof(slave_msgdpconn_s));
    if(NULL == pstMsgData)
    {
        printf("dispatch connection message malloc failed! errno=%d\n", errno);
        return VOS_ERR;
    }
    
    memset(pstMsgData,0,sizeof(slave_msgdpconn_s));
    pstMsgData->fd          = fd;
    pstMsgData->ClntNAddr   = ClntNAddr;
    pstMsgData->ClntPort    = ClntPort;
    printf("dispatch connection fd=%d, rctor=%p\n", fd, m_Rctor);
    return VRCT_API_MsqOptPush(m_Rctor, 0, SLAVE_MSG_DISPATH_CLNT, (CHAR *)pstMsgData, sizeof(slave_msgdpconn_s));
}


int32_t CEvtrctNetSlave::init()
{
    m_conn_hashtbl  = VOS_HashTbl_Create(1024, NULL, NULL);
    if ( NULL == m_conn_hashtbl )
    {
        std::cout <<"vrct create error! m_taskid="<< m_taskid << std::endl;
        return VOS_ERR;
    }

    m_Rctor= VRCT_API_Create(m_taskid, m_msqsize);
    if ( NULL == m_Rctor )
    {
        std::cout <<"vrct create error! m_taskid="<< m_taskid << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl);
        return VOS_ERR;
    }
    
    m_fliterid_ = 0;
    /*接收消息*/
    VRCT_MSQOPT_INIT(&m_msqopts_,
                     m_fliterid_,
                     msqctrl_cb,
                    (void *)this);
    
    if ( VOS_ERR == VRCT_API_MsqOptRegister(m_Rctor, &m_msqopts_) )
    {
        std::cout <<"vrct msg queue register error!" << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl);
        VRCT_API_Release(&m_Rctor);
        return VOS_ERR;
    }

    return VOS_OK;
}

int32_t CEvtrctNetSlave::start()
{
    if ( VOS_ERR == VRCT_API_Start(m_Rctor)  )
    {
        std::cout <<"vos reactor start failed!" << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl);
        VRCT_API_Release(&m_Rctor);
        return VOS_ERR;
    }
    
    return VOS_OK;
}

void CEvtrctNetSlave::uninit()
{
    VRCT_API_MsqOptUnRegister(m_Rctor, &m_msqopts_);
    
    if ( NULL != m_conn_hashtbl)
    {
        VOS_HashTbl_Release(&m_conn_hashtbl);
    }
    
    if (NULL != m_Rctor)
    {
        VRCT_API_Release(&m_Rctor);
    }
}

