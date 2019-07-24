/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vrct_api.h
  �� �� ��   : ����
  ��    ��  : jimk
  ��������   : 2019��7��22��
  ����޸�   :
  ��������   : epoll�����ṩ��APIʹ�ýӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
/**
 * @vrct_api.h
 * @brief vos reactor application interface
 * @author jimk 
 * @date 2019-07
 */

#ifndef _VRCT_API_H_
#define _VRCT_API_H_

/** ���İ�����*/
#define     VRCT_PACK_MAX           65536

/** Э����ͷ���趨: ����Сֱ�ӹ涨��INT32_T����ʾ*/
typedef struct tagVosReactorSetPacketInfo
{
    INT32_T     MaxSize;            /** ����󳤶�*/
    INT32_T     HeadOffset;         /** ͷ����Сƫ��*/
}VRCT_PACK_INFO_S, *PVRCT_PACK_INFO_S;

typedef VOID    (*PFVRCT_COMM_CB)(VOID *pvCtx);
typedef VOID    (*PFVRCT_NETEVT_CB)(INT32_T fd, VOID *pvConn);


/** IO�Ĺ���ģʽ*/
typedef enum
{
    VRCT_IOTYPE_NET = 0,            /** ����IO*/
    VRCT_IOTYPE_MSG,                /** ��ϢIO*/
    VRCT_IOTYPE_TIMER,              /** ��ʱ��IO*/
    VRCT_IOTYPE_SPAIR,              /** ��Ϣ��IO*/
    
    VRCT_IOTYPE_NUMS
}VRCT_IOTYPE_E;

#define VRCT_POLL_ETIN      (EPOLLIN  | EPOLLET)
#define VRCT_POLL_ETOUT     (EPOLLOUT | EPOLLET)
#define VRCT_POLL_ETINOUT   (EPOLLOUT | EPOLLIN | EPOLLET)

#define VRCT_POLL_LTIN      (EPOLLIN)
#define VRCT_POLL_LTOUT     (EPOLLOUT)
#define VRCT_POLL_LTINOUT   (EPOLLOUT | EPOLLIN)

#define VRCT_POLL_ADD       EPOLL_CTL_ADD
#define VRCT_POLL_MOD       EPOLL_CTL_MOD
#define VRCT_POLL_DEL       EPOLL_CTL_DEL


/** ��ʱ��������*/
typedef enum
{
    VRCT_TMTYPE_ONE = 0,              /** һ���Զ�ʱ��*/
    VRCT_TMTYPE_RECYLE,               /** ѭ����ʱ��*/
    
    VRCT_TMTYPE_NUMS
}VRCT_TMTYPE_E;



#endif

