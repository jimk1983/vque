/******************************************************************************

                  版权所有 (C), 2018-2028, 技术有限公司

 ******************************************************************************
  文 件 名   : vos_pub.h
  版 本 号   : 初稿
  作    者  : 蒋康
  生成日期   : 
  最近修改   :
  功能描述   : VOS对外统一提供的错误码
  函数列表   :
  修改历史   :
  1.日    期   :
    作    者   : 蒋康
    修改内容   : 创建文件

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
/***************** 系统错误码       ********************/
/**************************************************/

#define VOS_ERRMASK                         (INT32_T)0xff000000


/*系统错误*/
#define VOS_SYS_ERR                         (VOS_ERRMASK | 1) /** fail */
/*参数错误*/
#define VOS_ERR_PARAM                       (VOS_ERRMASK | 2)
/*超时错误*/
#define VOS_ERR_TIMEOUT                     (VOS_ERRMASK | 3)
/*阻塞*/
#define VOS_ERR_EAGAIN                      (VOS_ERRMASK | 4)
/*已经存在*/
#define VOS_ERR_EXIST                       (VOS_ERRMASK | 5)
/*已经存在*/
#define VOS_ERR_NOTEXIST                    (VOS_ERRMASK | 6)
/*写错误*/
#define VOS_ERR_WERR                        (VOS_ERRMASK | 7)
/*队列满*/
#define VOS_ERR_QUEFULL                     (VOS_ERRMASK | 8)



/**************************************************/
/***************** 系统重定义       ********************/
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
