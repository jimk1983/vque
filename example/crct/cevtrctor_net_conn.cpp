#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>

#include "config/config.h"
#include "cevtrctor_def.h"
#include "cevtrctor_net_conn.h"
#include "cevtrctor_net_slave.h"



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

void CEvtrctNetConn::net_conn_send_cb(int ifd, void *pvCtx)
{
    CEvtrctNetConn*         pstConn         = (CEvtrctNetConn*)pvCtx;
    vos_iobuf_sptr          pstIobufTmp     = NULL;
    int32_t                 lRet            = 0;
    char*                   pcData          = 0;
    int32_t                 lLeftLen        = 0;
    int32_t                 ulIndex         = 0;
    
    if ( pstConn->m_conn_status != CONN_STATUS_CONNECTED )
    {
        lRet = connect(pstConn->m_Fd, (struct sockaddr *)&pstConn->m_forward_addr, sizeof(struct sockaddr));
        if ( lRet <=0 )
        {
            if ( errno == EISCONN )
            {
                PEvent("[ECLNT] fd=%d, connect successful!", pstConn->m_Fd);
                pstConn->m_conn_status = CONN_STATUS_CONNECTED;
            }
            else if (errno == EINPROGRESS || errno == EAGAIN || errno == EINTR)
            {
                PEvent("[ECLNT] fd=%d, connecting!", pstConn->m_Fd);
                pstConn->m_conn_status = CONN_STATUS_CONNECTING;
            }
            else
            {
                char                acClientAddr[32]={0};
                strcpy(acClientAddr,inet_ntoa(pstConn->m_forward_addr.sin_addr));
                PError("[ECLNT] connect error=%d:%s, fd=%d, sev-addr=%s:%d, release it!", 
                        errno, strerror(errno), pstConn->m_Fd, acClientAddr, pstConn->m_forward_port);
                pstConn->netconn_release();
                pstConn = NULL;
            }
            return;
        }
        else
        {
            pstConn->m_conn_status = CONN_STATUS_CONNECTED;
            PEvent("[ECLNT] connect to access server successful, fd=%d", pstConn->m_Fd);
        }
    }
    
    if ( true == pstConn->m_stSendList.empty()
        && nullptr == pstConn->m_pstSendIobuf )
    {   
        //PEvent("22 net_conn_sendcb entry!fd=%d,m_conn_status=%d, stList=%p",
        //    ifd, pstConn->m_conn_status, &pstConn->m_stSendList);
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(pstConn->m_rctor_, ifd, VRCT_POLL_LTIN) )
        {
            //printf("[ECLNT] terminal ctrl fd=[%d] error! errno=%d\n", ifd, errno);
            pstConn->netconn_release();
            return;
        }
        else
        {
            //printf("[ECLNT] terminal epoll ctrl close the pollout successful!fd=%d, NETWORK_CTRL_SENDCLOSE\n",
            //   ifd);
        }
        return;
    }
    
    //printf("33 net_conn_sendcb entry!pstConn->m_iSndNums=%d\n", pstConn->m_iSndNums);
    if ( nullptr != pstConn->m_pstSendIobuf )
    {
        //printf("[ECLNT] terminal network connection send iobuf is not empty , must be send again!\n");
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
                pstConn->m_tx_flows +=lLeftLen;
                //printf("send flows=%d\n", pstConn->m_tx_flows);
                //printf("lLeftLen =%d\n", lLeftLen);
                pstConn->m_pstSendIobuf = nullptr;
                continue;
            }
            else if ( 0 < lLeftLen )
            {
                //printf("[ECLNT] Stop007: send iobuf again! leftLen=%d",
                //                        lLeftLen);
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
            //PEvent("[ECLNT] Inner-Send is zero! INNER-Info=[%d]", pstConn->m_Fd);
            return;
        }
        else
        {
            if(errno == EAGAIN )
            {
                //PEvent("[ECLNT] FD=[%d], packets must send again ", pstConn->m_Fd);
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
void CEvtrctNetConn::net_conn_recv_cb(int ifd, void *pvCtx)
{
    CEvtrctNetConn*     pstConn         = (CEvtrctNetConn*)pvCtx;
    cevt_net_conn_sptr  net_conn_sptr   = nullptr;
    char*               pcData          = NULL;
    int32_t             lError          = 0;
    
    if ( pstConn->m_conn_status != CONN_STATUS_CONNECTED )
    {
        lError = connect(pstConn->m_Fd, (struct sockaddr *)&pstConn->m_forward_addr, sizeof(struct sockaddr));
        if ( lError <=0 )
        {
            if ( errno == EISCONN )
            {
                //PEvent("[ECLNT] fd=%d, connect successful!", pstConn->m_Fd);
                pstConn->m_conn_status = CONN_STATUS_CONNECTED;
            }
            else if (errno == EINPROGRESS || errno == EAGAIN || errno == EINTR)
            {
                PEvent("[ECLNT] fd=%d, connecting!", pstConn->m_Fd);
                pstConn->m_conn_status = CONN_STATUS_CONNECTING;
            }
            else
            {
                PError("[ECLNT] connect error=%d:%s, fd=%d, release it!", 
                    errno, strerror(errno), pstConn->m_Fd);
                pstConn->netconn_release();
                pstConn = NULL;
            }
            
            return;
        }
        else
        {
            pstConn->m_conn_status = CONN_STATUS_CONNECTED;
            //PEvent("[ECLNT]  connect to access server successful, fd=%d", pstConn->m_Fd);
        }
    }
    
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
            #if 0
            printf("fd=%d, peer_fd=%d, recv all flow=%d, lError=%d,buf=\n%s\n",
                    pstConn->m_Fd,pstConn->m_peer_fd, 
                    pstConn->m_rx_flows, lError, pcData);
            #endif
            //printf("recv flows=%d\n", pstConn->m_rx_flows);
            /*继续更新*/
            VOS_IOBUF_RCVUPDATE_SIZE(pstConn->m_pstRecvIobuf->m_pstIobuf, (uint32_t)lError);
            if ( 0 >= pstConn->m_pstRecvIobuf->m_pstIobuf->InLeftSize )
            {
                break;
            }
        }
        else if ( 0 == lError )
        {
            //PError("[ECLNT] terminal sock recv error!,fd=%d, errno=[%d]:[%s], lError=%d",
            //            pstConn->m_Fd, errno, strerror(errno), lError);
            pstConn->netconn_release();
            return;
        }
        else
        {
            if ( errno == EAGAIN )
            {
                //PError("[ECLNT] Step006: Start Main handler the recv splice buffer!fd=%d, errno:%s--[%d]", 
                //            pstConn->m_Fd, strerror(errno),  errno);
                break;
            }
            
            if ( errno == EINTR )
            {
                //PError("[ECLNT] Step005: Start Main handler the recv splice buffer!fd=%d, errno=%d", 
                //            pstConn->m_Fd, errno);
                continue;
            }
            
            PError("[ECLNT] terminal sock recv error!,fd=%d, errno=[%d]:[%s], lError=%d",
                        pstConn->m_Fd, errno, strerror(errno), lError);
            pstConn->netconn_release();
            return;
        }
    }
    
    //printf("m_echo_enable=%d, fd=%d,m_forward_enable=%d\n",
    //        pstConn->m_echo_enable,
    //        ifd, 
    //        pstConn->m_forward_enable);
    
    if ( 1 == pstConn->m_echo_enable )
    {
        VOS_IOBUF_OUTRESET(pstConn->m_pstRecvIobuf->m_pstIobuf);
        pstConn->m_stSendList.push_back(pstConn->m_pstRecvIobuf);
        pstConn->m_iSndNums++;
        pstConn->m_pstRecvIobuf = nullptr;
        
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(pstConn->m_slave_ptr->m_Rctor, ifd, VRCT_POLL_LTINOUT) )
        {
            PError("[ESEVR] terminal ctrl fd=[%d] error! errno=%d", ifd, errno);
            pstConn->netconn_release();
            return;
        }
    }
    else if (1 == pstConn->m_forward_enable )
    {
        VOS_IOBUF_OUTRESET(pstConn->m_pstRecvIobuf->m_pstIobuf);
        if ( pstConn->m_slave_ptr->m_arryconns[pstConn->m_peer_fd] != nullptr)
        {
            net_conn_sptr = pstConn->m_slave_ptr->m_arryconns[pstConn->m_peer_fd];
            net_conn_sptr->m_stSendList.push_back(pstConn->m_pstRecvIobuf);
            net_conn_sptr->m_iSndNums++;
        }
        
        pstConn->m_pstRecvIobuf = nullptr;
        
        if( SYS_ERR == VRCT_API_NetworkOptCtrl(net_conn_sptr->m_rctor_, net_conn_sptr->m_Fd, VRCT_POLL_LTINOUT) )
        {
            PError("[ESEVR] terminal ctrl fd=[%d] error! errno=%d", net_conn_sptr->m_Fd, errno);
            pstConn->netconn_release();
            return;
        }
        else
        {
            //printf("forward the data to fd=%d successful! stList=%p\n", 
            //    net_conn_sptr->m_Fd, &net_conn_sptr->m_stSendList);
        }
    }
    else
    {
        pstConn->m_pstRecvIobuf = nullptr;
    }
    
    return;
}

void     CEvtrctNetConn::netconn_release()
{
    //printf("net connection release,fd=%d\n", m_Fd);
    //PrintTraceEvent("[INNER] Inner network connetion release success! conn-info=[%d:%s]", pstConn->lFd, pstConn->stSid.acID );
    //pstConn->pstRctCtx->iAllConnNums--;
    /*内网管理去掉, 然后重新排序*/
    //MUTL_InnerServerUnRegister(pstConn->pstRctCtx, pstConn);
    if ( 0 == m_Fd )
    {
        return;
    }
    
    /*关闭网络事件*/
    VRCT_API_NetworkOptUnRegister(m_rctor_, &m_netopts_);
    
    if ( nullptr != m_pstRecvIobuf )
    {
        m_pstRecvIobuf = nullptr;
    }

    if ( nullptr != m_pstRecvOldIobuf )
    {
        m_pstRecvOldIobuf = nullptr;
    }

    if ( nullptr != m_pstSendIobuf )
    {
        m_pstSendIobuf = nullptr;
    }

    m_stRecvList.clear();
    m_stSendList.clear();
    
    if (nullptr != m_slave_ptr)
    {
        printf("this fd=[%d].share_ptr use_count=%lu\n",
            m_Fd, m_slave_ptr->m_arryconns[m_Fd].use_count());
        
        m_slave_ptr->m_arryconns[m_Fd]= nullptr;
    }
    close(m_Fd);
    m_Fd = 0;

    if ( m_peer_fd > 0 
         && m_peer_fd < ULIMITD_MAXFD 
         && m_slave_ptr->m_arryconns[m_peer_fd] != nullptr )
    {
        m_slave_ptr->m_arryconns[m_peer_fd]->netconn_release();
        m_peer_fd = 0;
    }
}


int32_t  CEvtrctNetConn::netconn_create(CEvtrctNetSlave* slave, int32_t iFd, struct in_addr ClntNAddr, uint32_t ClntPort)
{
    m_Fd                    = iFd;
    m_slave_ptr             = slave;
    m_rctor_                = slave->m_Rctor;
    m_echo_enable           = slave->m_echo_enable;
    m_forward_enable        = slave->m_forward_enable;
    m_conn_status           = CONN_STATUS_CONNECTED;
    
    if( SYS_ERR == VOS_SOCK_SetOption(iFd) )
    {
        PError("[ESEVR] epoll ctrl sock set option,sockfd=%d, errno=%d\n", iFd, errno);
        close(iFd);
        return SYS_ERR;
    }
    
    VRCT_NETOPT_INIT(&m_netopts_,
                    iFd,
                    VRCT_POLL_LTINOUT,
                    net_conn_recv_cb,
                    net_conn_send_cb,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_NetworkOptRegister(m_rctor_, &m_netopts_) )
    {
        std::cout <<"[LISTN] VRCT network register error!" << std::endl;
        close(iFd);
        return SYS_ERR;
    }
    
    if ( 1 == m_forward_enable )
    {
        cevt_net_conn_sptr new_conn_pfw_sptr = std::make_shared<CEvtrctNetConn>();
        
        if( SYS_OK != new_conn_pfw_sptr->netconn_create(slave, slave->m_forward_addr, slave->m_forward_port) )
        {
            std::cout <<"[LISTN] VRCT network netconn_create error!" << std::endl;
            close(iFd);
            return SYS_ERR;
        }
        else
        {
            slave->m_arryconns[new_conn_pfw_sptr->m_Fd]= new_conn_pfw_sptr;
            new_conn_pfw_sptr->m_peer_fd = iFd;
            m_peer_fd = new_conn_pfw_sptr->m_Fd;
            PEvent("net connection create . pointer cout=%lu", new_conn_pfw_sptr.use_count());
        }
    }
    PEvent("new conntion create fd=%d,client-port=%d,forward-enable=%d", 
            iFd, ClntPort, m_forward_enable);
                
    return SYS_OK;
}

int32_t CEvtrctNetConn::netconn_create(CEvtrctNetSlave* slave, const std::string& serv_addr, int32_t  serv_port)
{
    m_slave_ptr             = slave;
    m_rctor_                = slave->m_Rctor;
    m_echo_enable           = slave->m_echo_enable;
    m_forward_enable        = slave->m_forward_enable;
    m_conn_status           = CONN_STATUS_INIT;
    
    m_forward_port = serv_port;
    m_forward_addr.sin_family = AF_INET;
    m_forward_addr.sin_addr.s_addr = inet_addr((const char *)serv_addr.c_str());
    m_forward_addr.sin_port = htons((short)m_forward_port);
    
    m_Fd            = VOS_SOCK_ClntCreate();
    if ( SYS_ERR == m_Fd )
    {
        PError("[ECLNT] socket create failed!");
        close(m_Fd);
        return SYS_ERR;
    }
    
    VRCT_NETOPT_INIT(&m_netopts_,
                    m_Fd,
                    VRCT_POLL_LTINOUT,
                    net_conn_recv_cb,
                    net_conn_send_cb,
                    (void *)this);
    
    if ( SYS_ERR == VRCT_API_NetworkOptRegister(m_rctor_, &m_netopts_) )
    {
        std::cout <<"[LISTN] VRCT network register error!" << std::endl;
        close(m_Fd);
        return SYS_ERR;
    }
    
    //PEvent("forward new conntion create fd=%d, sev-addr=%s:%d",m_Fd, serv_addr.c_str(), serv_port);
    return SYS_OK;
}

CEvtrctNetConn::CEvtrctNetConn()
{
    //std::cout << "CEvtrctNetConn entry" << std::endl;
};

CEvtrctNetConn::~CEvtrctNetConn()
{
    //std::cout << "~CEvtrctNetConn entry,fd="<< m_Fd << std::endl;
}
    
    
