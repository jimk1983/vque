#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include "config/config.h"
#include "cevtrctor_def.h"
#include "cevtrctor_net_conn.h"
#include "cevtrctor_net_slave.h"

static void msqctrl_cb(UINT32_T Value, VOID *pvMsg, INT32_T iMsgLen, VOID *pvCtx)
{
    CEvtrctNetSlave*    cevt_net_slave = (CEvtrctNetSlave*)pvCtx;
    switch(Value)
    {
        case SLAVE_MSG_DISPATH_CLNT:
            {   
                pslave_msgdpconn_s pstmsg = (pslave_msgdpconn_s)pvMsg;
                
                std::shared_ptr<CEvtrctNetConn> new_conn_sptr = std::make_shared<CEvtrctNetConn>();
                if(SYS_ERR == new_conn_sptr->netconn_create(cevt_net_slave, pstmsg->fd, pstmsg->ClntNAddr, pstmsg->ClntPort) )
                {
                    return;
                }
                else
                {
                    cevt_net_slave->m_arryconns[pstmsg->fd]= new_conn_sptr;
                }
                
                free(pstmsg);
            }
            break;
        default:
            break;
    }
    
}

int32_t CEvtrctNetSlave::dispatch_connect(const int& fd,const struct in_addr& ClntNAddr,const uint32_t& ClntPort)
{
    pslave_msgdpconn_s      pstMsgData = NULL;

    if (0 >= fd )
    {
        return VOS_ERR;
    }
    
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


int32_t CEvtrctNetSlave::init(const uint32_t echo_enable, const uint32_t forward_enable)
{
    m_conn_hashtbl_  = VOS_HashTbl_Create(1024, NULL, NULL);
    if ( NULL == m_conn_hashtbl_ )
    {
        std::cout <<"vrct create error! m_taskid="<< m_taskid << std::endl;
        return VOS_ERR;
    }
    
    m_Rctor= VRCT_API_Create(m_taskid, m_msqsize);
    if ( NULL == m_Rctor )
    {
        std::cout <<"vrct create error! m_taskid="<< m_taskid << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl_);
        return VOS_ERR;
    }
    
    m_fliterid_ = 0;
    m_echo_enable = echo_enable;
    m_forward_enable = forward_enable;
    /*接收消息*/
    VRCT_MSQOPT_INIT(&m_msqopts_,
                     m_fliterid_,
                     msqctrl_cb,
                    (void *)this);
    
    if ( VOS_ERR == VRCT_API_MsqOptRegister(m_Rctor, &m_msqopts_) )
    {
        std::cout <<"vrct msg queue register error!" << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl_);
        VRCT_API_Release(&m_Rctor);
        return VOS_ERR;
    }
    
    return VOS_OK;
}

void    CEvtrctNetSlave::info_set(const std::string& forward_Addr, const uint32_t& forward_Port)
{
    m_forward_addr      = forward_Addr;
    m_forward_port      = forward_Port;
    m_forward_enable    = 1;
}

int32_t CEvtrctNetSlave::start()
{
    if ( VOS_ERR == VRCT_API_Start(m_Rctor) )
    {
        std::cout <<"vos reactor start failed!" << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl_);
        VRCT_API_Release(&m_Rctor);
        return VOS_ERR;
    }
    
    return VOS_OK;
}

void CEvtrctNetSlave::uninit()
{
    VRCT_API_MsqOptUnRegister(m_Rctor, &m_msqopts_);
    
    if ( NULL != m_conn_hashtbl_)
    {
        VOS_HashTbl_Release(&m_conn_hashtbl_);
    }
    
    if (NULL != m_Rctor)
    {
        VRCT_API_Release(&m_Rctor);
    }
}

