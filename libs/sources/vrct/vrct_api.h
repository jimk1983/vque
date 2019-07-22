/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vrct_api.h
  版 本 号   : 初稿
  作    者  : jimk
  生成日期   : 2019年7月22日
  最近修改   :
  功能描述   : epoll对外提供的API使用接口
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/**
 * @vrct_api.h
 * @brief vos reactor application interface
 * @author jimk 
 * @date 2019-07
 */

#ifndef _VRCT_API_H_
#define _VRCT_API_H_

/** 最大的包长度*/
#define     VRCT_PACK_MAX       65536

/** 协议框架头部设定: 包大小直接规定用INT32_T来表示*/
typedef struct tagVosReactorSetPacketInfo
{
    INT32_T     MaxSize;            /*包最大长度*/
    INT32_T     HeadOffset;         /*头部大小偏移*/
}VRCT_PACK_INFO_S, *PVRCT_PACK_INFO_S;


/*IO的工作模式*/
typedef enum
{
    VRCT_IOTYPE_NET = 0,            /*网络IO*/
    VRCT_IOTYPE_MSG,                /*消息IO*/
    VRCT_IOTYPE_TIMER,              /*定时器IO*/
    VRCT_IOTYPE_SPAIR,              /*消息对IO*/
    
    VRCT_IOTYPE_NUMS
}VRCT_IOTYPE_E;




#endif

