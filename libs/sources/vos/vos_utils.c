/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_utils.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统常见公共使用适配
  函数列表   :
                           
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/
#include <vos/vos_pub.h>


/**
 * @brief atoi
 * @param pcStr [in] string nums
 */
UINT32_T VOS_atoi(CHAR *pcStr)
{
    UINT32_T i = VOS_INVALID_VAL;
    
    if ( NULL == pcStr )
    {
        return i;
    }

    i = atoi((const char *)pcStr);
       
    return i;
}

/**
 * @brief atoi
 * @param pcStr [out] string nums
 * @param iVal [in] integer value
 */
VOID VOS_itoa(CHAR *pcStr, UINT32_T iVal)
{
    CHAR acBuf[VOS_INT32STR_MAXLEN] = {0};

    if ( NULL == pcStr )
    {
        return;
    }

    #if VOS_PLAT_LINUX
    snprintf((char *)acBuf, VOS_INT32STR_MAXLEN-1,"%d", iVal);
    #elif VOS_PLAT_WIN
    sprintf_s((char *)acBuf, VOS_INT32STR_MAXLEN-1,"%d", iVal);
    #endif

    memcpy(pcStr, acBuf, strlen((char *)acBuf));
    
    return;
}

/*将十六数转换为字符串*/
void VOS_hextoa(CHAR *pcStr, INT8_T iVal)
{
    char acBuf[VOS_INT32STR_MAXLEN] = {0};

    if ( NULL == pcStr )
    {
        return;
    }

    #if VOS_PLAT_LINUX
    snprintf((char *)acBuf, VOS_INT32STR_MAXLEN-1,"%02x", iVal);
    #elif VOS_PLAT_WIN
    sprintf_s(acBuf, VOS_INT32STR_MAXLEN-1,"%02x", iVal);
    #endif

    memcpy(pcStr, acBuf, strlen((char *)acBuf));
    
    return;
}

/** 二进制打印*/
VOID VOS_PrintHex(CHAR *pcBuf, UINT32_T BufLen, CHAR *pcData, UINT32_T DataLen)
{
    UINT32_T    ulIndex = 0;
    UINT32_T    ulCount = 0;
    UINT8_T*    pcTmp = NULL;
    UINT32_T    offset = 0;
        
    pcTmp = (UINT8_T *)pcData;
    
    if ( DataLen >= BufLen )
    {
        return;
    }
    
    offset +=sprintf((char *)pcBuf + offset, "\r\n0x%p: ", pcData);
    offset +=sprintf((char *)pcBuf + offset, "\r\n0x0000: ");
        
    for(ulIndex=0; ulIndex< DataLen; ulIndex++)
    {
        offset +=sprintf((char *)pcBuf + offset, "%02x", pcTmp[ulIndex]);
        
        if ( (ulIndex+1) %4 == 0 )
        {
            offset += sprintf((char *)pcBuf + offset, " ");
        }
        
        if ( (ulIndex+1) %16 == 0 && ulIndex <DataLen-1)
        {
            ulCount++;
            offset += sprintf((char *)pcBuf + offset, "\r\n0x%04x: ", (ulCount*16));
        }
    }
    
    offset += sprintf((char *)pcBuf + offset,"\n");
    return; 
}

/*大端返回1, 小端返回0*/
BOOL_T VOS_CpuIsBigEndian()
{
    /*小端存放:从低地址到高地址0x01, 0x00, 0x00, 0x00*/
    INT32_T i = 0x00000001;
    /*取一个低字节*/
    INT8_T c = *(INT8_T *)&i;

    /*低地址字节, 为小端*/
    if ( 0x01 == c )
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*获取本线程ID号*/
UINT32_T VOS_GetSelfTId()
{
  UINT32_T ulpthreadId;

#if VOS_PLAT_LINUX
  ulpthreadId = pthread_self();
#endif
  
  return ulpthreadId;
}


/*获取本线程ID号*/
UINT32_T VOS_GetSelfPId()
{
  UINT32_T ulpthreadId;

#if VOS_PLAT_LINUX
  ulpthreadId = getpid();
#endif
  
  return ulpthreadId;
}



/*返回一个16字节的随机数*/
UINT16_T VOS_Random16()
{
    UINT16_T usRandom = 0;  

#if VOS_PLAT_LINUX
    srand((UINT32_T)time(NULL));
    usRandom = rand() & 0x0000FFFF;
#endif

    
    return usRandom;
}

/*返回一个32字节的随机数*/
UINT32_T VOS_Random32()
{
    UINT32_T usRandom = 0;

#if VOS_PLAT_LINUX
    srand((UINT32_T)time(NULL));
    usRandom = rand() & 0xFFFFFFFF;
#endif
    
    return usRandom;
}

VOID VOS_GenRandom64(DULONG *pstRand64)
{
    struct timespec time = { 0, 0 };
    
#if VOS_PLAT_LINUX
    //srand((unsigned)time(NULL));
    //timespec->获取的是纳秒的时间
    clock_gettime(CLOCK_REALTIME,&time);
    //单纯的依赖时间
    srand(time.tv_nsec);
    pstRand64->ulHi = rand();
    pstRand64->ulLo = rand(); 
#endif
}

VOID VOS_GenUUid(CHAR *pcUUid, INT32_T ulLen )
{
    char acUUid[VOS_GUID_LEN] = {0};

    if ( NULL == pcUUid
         || ulLen < VOS_GUID_LEN )
    {
        return;
    }
         
#if VOS_PLAT_LINUX
    int fd = open("/proc/sys/kernel/random/uuid", O_RDONLY);
    if( fd < 0 )
    {
        return;
    }
    
    read(fd, acUUid+1, 36);
    acUUid[37] = 0;
    close(fd);

    memset(pcUUid, 0, ulLen);
    memcpy(pcUUid, acUUid, 36);
    
#endif
    return;
}


