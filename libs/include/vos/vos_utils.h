/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_utils.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年1月2日
  最近修改   :
  功能描述   : 通用功能头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年1月2日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
 * 常用的宏定义                                       *
 *----------------------------------------------*/
/*IPV4全部地址字符串最大长度*/
#define VOS_IPV4STR_MAXLEN      16
/*IPV4地址单个字符串长度*/
#define VOS_IPADDR_DOILEN       4

#define VOS_GUID_LEN            64
     
/*定义32位整形字符串长度*/
#define VOS_INT32STR_MAXLEN     12

/*一般常见文件名的长度*/
#define VOS_FILE_MAXLEN         256

/*域名长度*/
#define VOS_URL_MAXLEN          128

BOOL_T      VOS_CpuIsBigEndian();
UINT32_T    VOS_GetSelfTId();
UINT32_T    VOS_GetSelfPId();
UINT32_T    VOS_atoi(CHAR *pcStr);
VOID        VOS_itoa(CHAR *pcStr, UINT32_T iVal);
VOID        VOS_hextoa(CHAR *pcStr, INT8_T iVal);
UINT16_T    VOS_Random16();
UINT32_T    VOS_Random32();
VOID        VOS_GenRandom64(DULONG *pstRand64);
VOID        VOS_GenUUid(CHAR *pcUUid, INT32_T ulLen );
VOID        VOS_PrintHex(CHAR *pcBuf, UINT32_T BufLen, CHAR *pcData, UINT32_T DataLen);


