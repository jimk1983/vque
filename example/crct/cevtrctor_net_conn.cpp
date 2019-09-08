#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include "config/config.h"
#include "cevtrctor_def.h"
#include "cevtrctor_net_conn.h"


int32_t  CEvtrctNetConn::netconn_create(int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort)
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
    
    
}


void CEvtrctNetSlave::init()
{
    m_conn_hashtbl  = VOS_HashTbl_Create(1024, NULL, NULL);
    if ( NULL == m_conn_hashtbl )
    {
        std::cout <<"vrct create error! m_taskid="<< m_taskid << std::endl;
        return;
    }

    m_Rctor= VRCT_API_Create(m_taskid, m_msqsize);
    if ( NULL == m_Rctor )
    {
        std::cout <<"vrct create error! m_taskid="<< m_taskid << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl);
        return;
    }
    
    /*接收消息*/
    VRCT_MSQOPT_INIT(&m_msqopts_,
                     m_fliterid_,
                     msqctrl_cb,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_MsqOptRegister(m_Rctor, &m_msqopts_) )
    {
        std::cout <<"vrct msg queue register error!" << std::endl;
        VOS_HashTbl_Release(&m_conn_hashtbl);
        VRCT_API_Release(&m_Rctor);
        return;
    }
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

