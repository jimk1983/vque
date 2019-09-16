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


void CEvtrctNetConn::net_conn_sendcb(int ifd, void *pvCtx)
{
#if 0
    PVOS_DLIST_S            pstEntry        = NULL;
    int32_t                 lRet            = 0;
    char*                   pcData          = 0;
    int32_t                 lLeftLen        = 0;
    int32_t                 ulIndex         = 0;
    VRCT_IOBUF_S*           pstIobufTmp     = NULL;
    
    if ( VOS_OK == VOS_DList_IsEmpty(&m_stSendList) 
        && NULL == m_pstSendIobuf )
    {   
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(m_rctor_, ifd, VRCT_POLL_LTIN) )
        {
            printf("[ECLNT] terminal ctrl fd=[%d] error! errno=%d\n", ifd, errno);
            netconn_release();
            return;
        }
        else
        {
            printf("[ECLNT] terminal epoll ctrl close the pollout successful!fd=%d, NETWORK_CTRL_SENDCLOSE\n",
                ifd);
        }
        return;
    }

    if ( NULL != m_pstSendIobuf )
    {
        printf("[ECLNT] terminal network connection send iobuf is not empty , must be send again!\n");
        goto SendAgain;
    }
    
    for(ulIndex = 0; ulIndex < m_iSndNums; ulIndex++)
    {
        VOS_DList_HeadGet(&m_stSendList, &pstEntry);
        m_iSndNums--;
        
        pstIobufTmp = VOS_DLIST_ENTRY(pstEntry, VRCT_IOBUF_S, stNode);
        
        if ( NULL == pstIobufTmp )
        {
            //printf("[ECLNT] Stop005: Inner POP the iobuf list error! INNER-ID=[%s],FD=[%d], Nums=[%d]", 
            //                            pstConn->stSid.acID, 
            //                            pstConn->lFd, 
            //                            pstConn->iSndNums);
            
            netconn_release();
            return;
        }
        
        /*进入到发送流程*/
        m_pstSendIobuf = pstIobufTmp;

SendAgain:
        pcData   = COM_IOBUF_GETSAVED_DATA(pstConn->pstSendIobuf);
        lLeftLen = COM_IOBUF_GETSAVED_LEN(pstConn->pstSendIobuf);
        
        lRet = send(ifd, pcData, lLeftLen, MSG_NOSIGNAL);
        if( lRet > 0 )
        {
            pstConn->pstMgr->iTxPacketSize += lRet;
            
            COM_IOBUF_SETOUTPUTED_LEN(pstConn->pstSendIobuf, lRet);
            lLeftLen = COM_IOBUF_GETSAVED_LEN(pstConn->pstSendIobuf);
            
            if ( lLeftLen == 0 )
            {
                pstConn->pstMgr->iSendPPs++;
                PrintTraceDebug("[ECLNT] Stop006: Inner-Server Send iobuf successful!INNER-ID=[%s],FD=[%d], DataSize=%d",
                                        pstConn->stSid.acID, 
                                        pstConn->lFd,
                                        pstConn->pstSendIobuf->ulDataLen);
                
                COM_IOBUF_RESET(pstConn->pstSendIobuf);
                continue;
            }
            else if ( 0 < lLeftLen )
            {
                PrintTraceDebug("[ECLNT] Stop007: send iobuf again! INNER-ID=[%s],FD=[%d], DataSize=%d, leftLen=%d",
                                        pstConn->stSid.acID, 
                                        pstConn->lFd, 
                                        pstConn->pstSendIobuf->ulDataLen, 
                                        lLeftLen);
                goto SendAgain;
            }
            else
            {
                PrintError("[ECLNT] system error! INNER-Info=[%s:%d]", pstConn->stSid.acID, pstConn->lFd);
                Test_EQue_ConnRelease(pstConn);
                return;
            }
        }
        else if ( lRet == 0 )
        {
            PrintTraceInfo("[ECLNT] Inner-Send is zero! INNER-Info=[%s:%d]", pstConn->stSid.acID, pstConn->lFd);
            return;
        }
        else
        {
            if(errno == EAGAIN )
            {
                PrintTraceWarning("[ECLNT] INNER-ID=[%s],FD=[%d], packets must send again ", pstConn->stSid.acID, pstConn->lFd);
                return;
            }
            
            if ( errno == EINTR )
            {
               goto SendAgain;
            }
            
            PrintError("[ECLNT] Stop009: Socket send error! INNER-ID=[%s],FD=[%d], errno=%d:%s",
                        pstConn->stSid.acID, 
                        pstConn->lFd, 
                        errno, 
                        VOS_GetStrLastError());
            pstConn->lErrorCode = errno;
            Test_EQue_ConnRelease(pstConn);
            return;
        }
    }
#endif
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
    
    VOS_IOBuf_free(pstConn->m_pstRecvIobuf);
    pstConn->m_pstRecvIobuf = NULL;
    
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

int32_t  CEvtrctNetConn::netconn_create(CEvtrctNetSlave* slave, int32_t iFd, struct in_addr ClntNAddr, uint32_t uiClntPort)
{
    VOS_DLIST_INIT(&m_stRecvList);
    VOS_DLIST_INIT(&m_stSendList);

    m_Fd                    = iFd;
    m_slave_ptr             = slave;
    m_rctor_                = slave->m_Rctor;
    m_iConnStatus           = CONN_STATUS_INIT;
    
    VRCT_NETOPT_INIT(&m_netopts_,
                    iFd,
                    VRCT_POLL_LTINOUT,
                    net_conn_recvcb,
                    net_conn_sendcb,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_NetworkOptRegister(m_rctor_, &m_netopts_) )
    {
        std::cout <<"[LISTN] VRCT network register error!" << std::endl;
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
                
                std::shared_ptr<CEvtrctNetConn> new_conn_sptr = std::make_shared<CEvtrctNetConn>();
                new_conn_sptr->netconn_create(cevt_net_slave,pstmsg->fd, pstmsg->ClntNAddr, pstmsg->ClntPort);
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

