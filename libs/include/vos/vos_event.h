/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_event.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : VOS的事件封装
  函数列表   :
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#ifndef _VOS_EVENT_H_
#define _VOS_EVENT_H_

#if VOS_PLAT_LINUX
struct tagLinuxEvent
{
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
};
typedef struct tagLinuxEvent VOS_EVT_T, *PVOS_EVT_T;
#elif VOS_PLAT_MAC

#elif VOS_PLAT_WIN
#ifndef HANDLE
#define HANDLE (void *)
#endif
typedef HANDLE  VOS_EVT_T, *PVOS_EVT_T;
#endif


INT32_T  VOS_ThreadEvent_Init(VOS_EVT_T *pstEvent);

void     VOS_ThreadEvent_Destroy(VOS_EVT_T *pstEvent);

INT32_T  VOS_ThreadEvent_Waitfor(VOS_EVT_T *pstEvent, UINT32_T uiMillTimeout);

INT32_T  VOS_ThreadEvent_Notify(VOS_EVT_T *pstEvent);


#endif
