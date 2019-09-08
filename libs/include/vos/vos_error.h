/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_pub.h
  �� �� ��   : ����
  ��    ��  : ����
  ��������   : 
  ����޸�   :
  ��������   : VOS����ͳһ�ṩ�Ĵ�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef _VOS_ERROR_H_
#define _VOS_ERROR_H_

#ifndef VOS_OK
#define VOS_OK               0
#endif
        
#ifndef VOS_ERR
#define VOS_ERR             (~(0))
#endif

#ifndef VOS_TRUE
#define VOS_TRUE            1
#endif
    
#ifndef VOS_FALSE
#define VOS_FALSE           0
#endif
    

/**************************************************/
/***************** ϵͳ������       ********************/
/**************************************************/

#define VOS_ERRMASK                         (INT32_T)0xff000000


/*ϵͳ����*/
#define VOS_SYS_ERR                         (VOS_ERRMASK | 1) /** fail */
/*��������*/
#define VOS_ERR_PARAM                       (VOS_ERRMASK | 2)
/*��ʱ����*/
#define VOS_ERR_TIMEOUT                     (VOS_ERRMASK | 3)
/*����*/
#define VOS_ERR_EAGAIN                      (VOS_ERRMASK | 4)
/*�Ѿ�����*/
#define VOS_ERR_EXIST                       (VOS_ERRMASK | 5)
/*�Ѿ�����*/
#define VOS_ERR_NOTEXIST                    (VOS_ERRMASK | 6)
/*д����*/
#define VOS_ERR_WERR                        (VOS_ERRMASK | 7)
/*������*/
#define VOS_ERR_QUEFULL                     (VOS_ERRMASK | 8)



/**************************************************/
/***************** ϵͳ�ض���       ********************/
/**************************************************/

#ifndef SYS_OK
#define SYS_OK                              VOS_OK
#endif

#ifndef SYS_ERR
#define SYS_ERR                             VOS_ERR
#endif


#ifndef SYS_TRUE
#define SYS_TRUE                            VOS_TRUE
#endif
    
#ifndef SYS_FALSE
#define SYS_FALSE                           VOS_FALSE
#endif

#ifndef SYS_ERR_PARAM
#define SYS_ERR_PARAM                       VOS_ERR_PARAM
#endif

#ifndef SYS_ERR_TIMEOUT
#define SYS_ERR_TIMEOUT                     VOS_ERR_TIMEOUT
#endif

#ifndef SYS_ERR_EWOULDBLOCK                
#define SYS_ERR_EWOULDBLOCK                 VOS_ERR_EAGAIN
#endif

#ifndef SYS_ERR_EXIST
#define SYS_ERR_EXIST                       VOS_ERR_EXIST
#endif

#ifndef SYS_ERR_NOTEXIST
#define SYS_ERR_NOTEXIST                    VOS_ERR_NOTEXIST
#endif

#ifndef SYS_ERR_WERR
#define SYS_ERR_WERR                        VOS_ERR_WERR
#endif

#ifndef MAX_PATH
#define MAX_PATH                            260
#endif

#ifndef SYS_IPV4LEN
#define SYS_IPV4LEN                         32
#endif


#endif
