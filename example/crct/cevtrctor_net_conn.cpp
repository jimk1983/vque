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

#if 0
void CEvtrctNetConn::net_conn_sendcb(int ifd, void *pvCtx)
{
    CEvtrctNetConn*         pstConn         = (CEvtrctNetConn*)pvCtx;
    PVOS_DLIST_S            pstEntry        = NULL;
    int32_t                 lRet            = 0;
    char*                   pcData          = 0;
    int32_t                 lLeftLen        = 0;
    int32_t                 ulIndex         = 0;
    VOS_IOBUF_S*            pstIobufTmp     = NULL;

    printf("11 net_conn_sendcb entry! pstConn->m_pstSendIobuf=%p\n",
            pstConn->m_pstSendIobuf);
    
    if ( VOS_TRUE == VOS_DList_IsEmpty(&pstConn->m_stSendList) 
        && NULL == pstConn->m_pstSendIobuf )
    {   
        printf("22 net_conn_sendcb entry!\n");
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(pstConn->m_rctor_, ifd, VRCT_POLL_LTIN) )
        {
            printf("[ECLNT] terminal ctrl fd=[%d] error! errno=%d\n", ifd, errno);
            pstConn->netconn_release();
            return;
        }
        else
        {
            printf("[ECLNT] terminal epoll ctrl close the pollout successful!fd=%d, NETWORK_CTRL_SENDCLOSE\n",
                ifd);
        }
        return;
    }
    
    printf("33 net_conn_sendcb entry!pstConn->m_iSndNums=%d\n", pstConn->m_iSndNums);
    if ( NULL != pstConn->m_pstSendIobuf )
    {
        printf("[ECLNT] terminal network connection send iobuf is not empty , must be send again!data=%d\n",pstConn->m_pstSendIobuf->DataSize);
        goto SendAgain;
    }
    
    for(ulIndex = 0; ulIndex < pstConn->m_iSndNums; ulIndex++)
    {
        VOS_DList_HeadGet(&pstConn->m_stSendList, &pstEntry);
        pstConn->m_iSndNums--;
        
        pstIobufTmp = VOS_DLIST_ENTRY(pstEntry, VOS_IOBUF_S, stNode);
        
        if ( NULL == pstIobufTmp )
        {
            pstConn->netconn_release();
            return;
        }
        
        /*进入到发送流程*/
        pstConn->m_pstSendIobuf = pstIobufTmp;

SendAgain:
        pcData   = COM_IOBUF_GETORIG_PTR(pstConn->m_pstSendIobuf);
        lLeftLen = VOS_IOBUF_SNDLEFT_SIZE(pstConn->m_pstSendIobuf);
        
        lRet = send(ifd, pcData, lLeftLen, MSG_NOSIGNAL);
        if( lRet > 0 )
        {
            VOS_IOBUF_SNDUPDATE_SIZE(pstConn->m_pstSendIobuf, lRet);
            lLeftLen = VOS_IOBUF_SNDLEFT_SIZE(pstConn->m_pstSendIobuf);
            if ( lLeftLen == 0 )
            {
                printf("lLeftLen =%d\n", lLeftLen);
                VOS_IOBuf_free(pstConn->m_pstSendIobuf);
                pstConn->m_pstSendIobuf = NULL;
                continue;
            }
            else if ( 0 < lLeftLen )
            {
                printf("[ECLNT] Stop007: send iobuf again! leftLen=%d",
                                        lLeftLen);
                goto SendAgain;
            }
            else
            {
                PError("[ECLNT] system error! INNER-Info=[%d]", pstConn->m_Fd);
                pstConn->netconn_release();
                return;
            }
        }
        else if ( lRet == 0 )
        {
            PEvent("[ECLNT] Inner-Send is zero! INNER-Info=[%d]", pstConn->m_Fd);
            return;
        }
        else
        {
            if(errno == EAGAIN )
            {
                PEvent("[ECLNT] FD=[%d], packets must send again ", pstConn->m_Fd);
                return;
            }
            
            if ( errno == EINTR )
            {
               goto SendAgain;
            }
            
            PError("[ECLNT] Stop009: Socket send error! INNER-FD=[%d], errno=%d:%s", 
                        pstConn->m_Fd, 
                        errno, 
                        strerror(errno));
            pstConn->netconn_release();
            return;
        }
    }
}


/*快速接收处理*/
void CEvtrctNetConn::net_conn_recvcb(int ifd, void *pvCtx)
{
    CEvtrctNetConn*     pstConn         = (CEvtrctNetConn*)pvCtx;
    char*               pcData          = NULL;
    int32_t             lError          = 0;
    
    if ( NULL == pstConn->m_pstRecvIobuf )
    {    
        /*申请TCP类型的IOBUF内存*/
        pstConn->m_pstRecvIobuf = VOS_IOBuf_mallocMax(0);
        //pstConn->m_pstRecvIobuf = VOS_IOBuf_malloc(0);
        if ( NULL == pstConn->m_pstRecvIobuf )
        {
            PError("[ECLNT] iobuf malloc error!");
            pstConn->netconn_release();
            return;
        }
    }
    
    for(;;)
    {
        pcData = VOS_IOBUF_GETCURT_PTR(pstConn->m_pstRecvIobuf);
        
        lError = recv(pstConn->m_Fd, pcData, pstConn->m_pstRecvIobuf->InLeftSize, 0);
        if ( lError > 0  )
        {
            pstConn->m_rx_flows += lError;
            printf("recv all flow=%d, lError=%d\n", pstConn->m_rx_flows, lError);
            /*继续更新*/
            VOS_IOBUF_RCVUPDATE_SIZE(pstConn->m_pstRecvIobuf, (uint32_t)lError);
            if ( 0 >= pstConn->m_pstRecvIobuf->InLeftSize )
            {
                break;
            }
        }
        else if ( 0 == lError )
        {
            PError("[ECLNT] terminal sock recv error!,fd=%d, errno=[%d]:[%s], lError=%d\n",
                        pstConn->m_Fd, errno, strerror(errno), lError);
            pstConn->netconn_release();
            return;
        }
        else
        {
            if ( errno == EAGAIN )
            {
                PDebug("[ECLNT] Step006: Start Main handler the recv splice buffer!fd=%d, errno=%d", 
                            pstConn->m_Fd, errno);
                break;
            }
            
            if ( errno == EINTR )
            {
                PDebug("[ECLNT] Step005: Start Main handler the recv splice buffer!fd=%d, errno=%d", 
                            pstConn->m_Fd, errno);
                continue;
            }
            
            PError("[ECLNT] terminal sock recv error!,fd=%d, errno=[%d]:[%s], lError=%d\n",
                        pstConn->m_Fd, errno, strerror(errno), lError);
            pstConn->netconn_release();
            return;
        }
    }
    
    printf("m_echo_enable=%d, fd=%d\n", pstConn->m_echo_enable, ifd);
    
    if ( 1 == pstConn->m_echo_enable )
    {
        VOS_IOBUF_OUTRESET(pstConn->m_pstRecvIobuf);
        VOS_DLIST_ADD_TAIL(&pstConn->m_stSendList, (VOS_DLIST_S *)(&pstConn->m_pstRecvIobuf->stNode));
        pstConn->m_iSndNums++;
        pstConn->m_pstRecvIobuf = NULL;
        
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(pstConn->m_slave_ptr->m_Rctor, ifd, VRCT_POLL_LTINOUT) )
        {
            PError("[ESEVR] terminal ctrl fd=[%d] error! errno=%d\n", ifd, errno);
            pstConn->netconn_release();
            return;
        }
    }
    else if (1 == pstConn->m_forward_enable )
    {
        
    }
    else
    {
        VOS_IOBuf_free(pstConn->m_pstRecvIobuf);
        pstConn->m_pstRecvIobuf = NULL;
    }
    
    return;
}

void     CEvtrctNetConn::netconn_release()
{
    PVOS_DLIST_S        pthisEntry  = NULL, pNextEntry=NULL, plistHead=NULL;
    VOS_IOBUF_S*        pstIobuf    = NULL;
    
    //PrintTraceEvent("[INNER] Inner network connetion release success! conn-info=[%d:%s]", pstConn->lFd, pstConn->stSid.acID );
    //pstConn->pstRctCtx->iAllConnNums--;
    /*内网管理去掉, 然后重新排序*/
    //MUTL_InnerServerUnRegister(pstConn->pstRctCtx, pstConn);
    
    /*关闭网络事件*/
    VRCT_API_NetworkOptUnRegister(m_rctor_, &m_netopts_);
    
    if ( NULL != m_pstRecvIobuf )
    {
        VOS_IOBuf_free(m_pstRecvIobuf);
        m_pstRecvIobuf = NULL;
    }

    if ( NULL != m_pstSendIobuf )
    {
        VOS_IOBuf_free(m_pstSendIobuf);
        m_pstSendIobuf = NULL;
    }

    if ( SYS_OK != VOS_DList_IsEmpty(&m_stRecvList))
    {
        plistHead = &m_stRecvList;
        for (pthisEntry = plistHead->prev;
            pthisEntry != plistHead;
            pthisEntry = pNextEntry)
        {
            pNextEntry = pthisEntry->prev;
            pstIobuf = VOS_DLIST_ENTRY(pthisEntry, VOS_IOBUF_S, stNode);
            VOS_DLIST_DEL(&pstIobuf->stNode);
            VOS_IOBuf_free(pstIobuf);
            pthisEntry = plistHead->prev;
        }
    }

    if ( SYS_OK != VOS_DList_IsEmpty(&m_stSendList))
    {
        plistHead = &m_stSendList;
        for (pthisEntry = plistHead->prev;
            pthisEntry != plistHead;
            pthisEntry = pNextEntry)
        {
            pNextEntry = pthisEntry->prev;
            pstIobuf = VOS_DLIST_ENTRY(pthisEntry, VOS_IOBUF_S, stNode);
            VOS_DLIST_DEL(&pstIobuf->stNode);
            VOS_IOBuf_free(pstIobuf);
            pthisEntry = plistHead->prev;
        }
    }
    close(m_Fd);
    m_slave_ptr->m_arryconns[m_Fd]= nullptr;
    
    printf("net connection release success,fd=%d\n", m_Fd);
}
#endif
void CEvtrctNetConn::net_conn_sendcb(int ifd, void *pvCtx)
{
    CEvtrctNetConn*         pstConn         = (CEvtrctNetConn*)pvCtx;
    vos_iobuf_sptr          pstIobufTmp     = NULL;
    int32_t                 lRet            = 0;
    char*                   pcData          = 0;
    int32_t                 lLeftLen        = 0;
    int32_t                 ulIndex         = 0;

    printf("11 net_conn_sendcb entry!\n");
    
    if ( true == pstConn->m_stSendList.empty()
        && nullptr == pstConn->m_pstSendIobuf )
    {   
        printf("22 net_conn_sendcb entry!\n");
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(pstConn->m_rctor_, ifd, VRCT_POLL_LTIN) )
        {
            printf("[ECLNT] terminal ctrl fd=[%d] error! errno=%d\n", ifd, errno);
            pstConn->netconn_release();
            return;
        }
        else
        {
            printf("[ECLNT] terminal epoll ctrl close the pollout successful!fd=%d, NETWORK_CTRL_SENDCLOSE\n",
                ifd);
        }
        return;
    }
    
    printf("33 net_conn_sendcb entry!pstConn->m_iSndNums=%d\n", pstConn->m_iSndNums);
    if ( nullptr != pstConn->m_pstSendIobuf )
    {
        printf("[ECLNT] terminal network connection send iobuf is not empty , must be send again!\n");
        goto SendAgain;
    }
    
    for(ulIndex = 0; true != pstConn->m_stSendList.empty(); ulIndex++)
    {
        //VOS_DList_HeadGet(&pstConn->m_stSendList, &pstEntry);
        pstConn->m_pstSendIobuf = pstConn->m_stSendList.front();
        pstConn->m_stSendList.pop_front();
        pstConn->m_iSndNums--;
        
SendAgain:
        pcData   = COM_IOBUF_GETORIG_PTR(pstConn->m_pstSendIobuf->m_pstIobuf);
        lLeftLen = VOS_IOBUF_SNDLEFT_SIZE(pstConn->m_pstSendIobuf->m_pstIobuf);
        
        lRet = send(ifd, pcData, lLeftLen, MSG_NOSIGNAL);
        if( lRet > 0 )
        {
            VOS_IOBUF_SNDUPDATE_SIZE(pstConn->m_pstSendIobuf->m_pstIobuf, lRet);
            lLeftLen = VOS_IOBUF_SNDLEFT_SIZE(pstConn->m_pstSendIobuf->m_pstIobuf);
            if ( lLeftLen == 0 )
            {
                printf("lLeftLen =%d\n", lLeftLen);
                pstConn->m_pstSendIobuf = nullptr;
                continue;
            }
            else if ( 0 < lLeftLen )
            {
                printf("[ECLNT] Stop007: send iobuf again! leftLen=%d",
                                        lLeftLen);
                goto SendAgain;
            }
            else
            {
                PError("[ECLNT] system error! INNER-Info=[%d]", pstConn->m_Fd);
                pstConn->netconn_release();
                return;
            }
        }
        else if ( lRet == 0 )
        {
            PEvent("[ECLNT] Inner-Send is zero! INNER-Info=[%d]", pstConn->m_Fd);
            return;
        }
        else
        {
            if(errno == EAGAIN )
            {
                PEvent("[ECLNT] FD=[%d], packets must send again ", pstConn->m_Fd);
                return;
            }
            
            if ( errno == EINTR )
            {
               goto SendAgain;
            }
            
            PError("[ECLNT] Stop009: Socket send error! INNER-FD=[%d], errno=%d:%s", 
                        pstConn->m_Fd, 
                        errno, 
                        strerror(errno));
            pstConn->netconn_release();
            return;
        }
    }
}


/*快速接收处理*/
void CEvtrctNetConn::net_conn_recvcb(int ifd, void *pvCtx)
{
    CEvtrctNetConn*     pstConn         = (CEvtrctNetConn*)pvCtx;
    char*               pcData          = NULL;
    int32_t             lError          = 0;
    
    if ( nullptr == pstConn->m_pstRecvIobuf )
    {    
        /*申请TCP类型的IOBUF内存*/
        pstConn->m_pstRecvIobuf = std::make_shared<CVosIobuf>(4096);
        if ( nullptr == pstConn->m_pstRecvIobuf )
        {
            PError("[ECLNT] iobuf malloc error!");
            pstConn->netconn_release();
            return;
        }
    }
    
    for(;;)
    {
        pcData = VOS_IOBUF_GETCURT_PTR(pstConn->m_pstRecvIobuf->m_pstIobuf);
        
        lError = recv(pstConn->m_Fd, pcData, pstConn->m_pstRecvIobuf->m_pstIobuf->InLeftSize, 0);
        if ( lError > 0  )
        {
            pstConn->m_rx_flows += lError;
            printf("recv all flow=%d, lError=%d\n", pstConn->m_rx_flows, lError);
            /*继续更新*/
            VOS_IOBUF_RCVUPDATE_SIZE(pstConn->m_pstRecvIobuf->m_pstIobuf, (uint32_t)lError);
            if ( 0 >= pstConn->m_pstRecvIobuf->m_pstIobuf->InLeftSize )
            {
                break;
            }
        }
        else if ( 0 == lError )
        {
            PError("[ECLNT] terminal sock recv error!,fd=%d, errno=[%d]:[%s], lError=%d\n",
                        pstConn->m_Fd, errno, strerror(errno), lError);
            pstConn->netconn_release();
            return;
        }
        else
        {
            if ( errno == EAGAIN )
            {
                PDebug("[ECLNT] Step006: Start Main handler the recv splice buffer!fd=%d, errno=%d", 
                            pstConn->m_Fd, errno);
                break;
            }
            
            if ( errno == EINTR )
            {
                PDebug("[ECLNT] Step005: Start Main handler the recv splice buffer!fd=%d, errno=%d", 
                            pstConn->m_Fd, errno);
                continue;
            }
            
            PError("[ECLNT] terminal sock recv error!,fd=%d, errno=[%d]:[%s], lError=%d\n",
                        pstConn->m_Fd, errno, strerror(errno), lError);
            pstConn->netconn_release();
            return;
        }
    }
    
    printf("m_echo_enable=%d, fd=%d\n", pstConn->m_echo_enable, ifd);
    
    if ( 1 == pstConn->m_echo_enable )
    {
        VOS_IOBUF_OUTRESET(pstConn->m_pstRecvIobuf->m_pstIobuf);
        pstConn->m_stSendList.push_back(pstConn->m_pstRecvIobuf);
        pstConn->m_iSndNums++;
        pstConn->m_pstRecvIobuf = nullptr;
        
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(pstConn->m_slave_ptr->m_Rctor, ifd, VRCT_POLL_LTINOUT) )
        {
            PError("[ESEVR] terminal ctrl fd=[%d] error! errno=%d\n", ifd, errno);
            pstConn->netconn_release();
            return;
        }
    }
    else if (1 == pstConn->m_forward_enable )
    {
        
    }
    else
    {
        pstConn->m_pstRecvIobuf = nullptr;
    }
    
    return;
}

void     CEvtrctNetConn::netconn_release()
{
    //PrintTraceEvent("[INNER] Inner network connetion release success! conn-info=[%d:%s]", pstConn->lFd, pstConn->stSid.acID );
    //pstConn->pstRctCtx->iAllConnNums--;
    /*内网管理去掉, 然后重新排序*/
    //MUTL_InnerServerUnRegister(pstConn->pstRctCtx, pstConn);
    
    /*关闭网络事件*/
    VRCT_API_NetworkOptUnRegister(m_rctor_, &m_netopts_);
    
    if ( nullptr != m_pstRecvIobuf )
    {
        m_pstRecvIobuf = nullptr;
    }

    if ( nullptr != m_pstSendIobuf )
    {
        m_pstSendIobuf = nullptr;
    }

    m_stRecvList.clear();
    m_stSendList.clear();
    
    close(m_Fd);
    m_slave_ptr->m_arryconns[m_Fd]= nullptr;
    
    printf("net connection release success,fd=%d\n", m_Fd);
}


int32_t  CEvtrctNetConn::netconn_create(CEvtrctNetSlave* slave, int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort)
{
    //VOS_DLIST_INIT(&m_stRecvList);
    //VOS_DLIST_INIT(&m_stSendList);

    m_Fd                    = iFd;
    m_slave_ptr             = slave;
    m_rctor_                = slave->m_Rctor;
    m_iConnStatus           = CONN_STATUS_INIT;
    m_echo_enable           = slave->m_echo_enable;
    m_forward_enable        = slave->m_forward_enable;
    
    if ( SYS_ERR == VOS_SOCK_SetOption(iFd) )
    {
        std::cout <<"[LISTN] VRCT network VOS_SOCK_SetOption error!" << std::endl;
        close(iFd);
        return -1;
    }

    VRCT_NETOPT_INIT(&m_netopts_,
                    iFd,
                    VRCT_POLL_LTINOUT,
                    net_conn_recvcb,
                    net_conn_sendcb,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_NetworkOptRegister(m_rctor_, &m_netopts_) )
    {
        std::cout <<"[LISTN] VRCT network register error!" << std::endl;
        close(iFd);
        return -1;
    }
                                                    
    return SYS_OK;
}

int32_t CEvtrctNetConn::netconn_create(CEvtrctNetSlave* slave, const std::string& serv_addr, int32_t  serv_port)
{
    m_slave_ptr             = slave;
    m_rctor_                = slave->m_Rctor;
    m_iConnStatus           = CONN_STATUS_INIT;
    m_echo_enable           = slave->m_echo_enable;
    m_forward_enable        = slave->m_forward_enable;
    

    VRCT_NETOPT_INIT(&m_netopts_,
                    m_Fd,
                    VRCT_POLL_LTINOUT,
                    net_conn_recvcb,
                    net_conn_sendcb,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_NetworkOptRegister(m_rctor_, &m_netopts_) )
    {
        std::cout <<"[LISTN] VRCT network register error!" << std::endl;
        close(m_Fd);
        return -1;
    }


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
                if ( cevt_net_slave->m_forward_enable )
                {
                    evt_netconn_sptr new_conn_pfw_sptr = std::make_shared<CEvtrctNetConn>();
                    new_conn_pfw_sptr->netconn_create(cevt_net_slave, cevt_net_slave->m_serv_addr, cevt_net_slave->m_serv_port);
                }
                
                std::shared_ptr<CEvtrctNetConn> new_conn_sptr = std::make_shared<CEvtrctNetConn>();
                new_conn_sptr->netconn_create(cevt_net_slave, pstmsg->fd, pstmsg->ClntNAddr, pstmsg->ClntPort);
                
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


int32_t CEvtrctNetSlave::init(const uint32_t echo_enable, const uint32_t forward_enable)
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
    if ( VOS_ERR == VRCT_API_Start(m_Rctor) )
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

CVosIobuf::CVosIobuf(int32_t buf_size)
        :m_size(buf_size)
{
    if (buf_size > VOS_IOBUF_MAXSIZE)
    {
        m_pstIobuf = VOS_IOBuf_mallocMax(0);
    }
    else
    {
        m_pstIobuf = VOS_IOBuf_malloc(0);
    }
    printf("CVosIobuf entry, new this=%p\n", this);
}

CVosIobuf::~CVosIobuf()
{
    printf("~CVosIobuf entry, delete this=%p\n", this);
    if (NULL != m_pstIobuf )
    {
        VOS_IOBuf_free(m_pstIobuf);
        m_pstIobuf = NULL;
    }
}


