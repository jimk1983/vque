/******************************************************************************

                  版权所有 (C), 2001-2011, 网络有限公司

 ******************************************************************************
  文 件 名   : main.cpp
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2019年7月25日 星期四
  最近修改   :
  功能描述   : vque的测试代码
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月25日 星期四
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>


#include "public.h"

int main(int argc, char *argv[])
{
    TEST_CONF_S    stCfg={0};
    
    if ( argc < 1 )
    {
        printf("************************************\n");
        printf("************Useage error************\n");
        printf("************************************\n");
        printf("******Run-Example: ./App xxx");
        printf("******Param List : --tcode      test type code\n");
        printf("******Param List : --ethname    eth name\n");
        printf("************************************\n");
        
        return -1;
    }
    
    config_init(&stCfg);
    
    for(int32_t i =1; i < argc ; i++)
    {
        if ( argv[i][0] == '-' )
        {
            if ( 0 == strcmp(argv[i], "--tcode")  )
            {
                i++;
                stCfg.CurrTypeCode = atoi(argv[i]);
            }
            
            if ( 0 == strcmp(argv[i], "--ethname")  )
            {
                i++;
                memset(stCfg.acEthName,0, sizeof((char *)stCfg.acEthName));
                strcpy((char *)stCfg.acEthName, argv[i]);
            }
        }
    }
    
    printf("************************************\n");
    printf("************************************\n");
    printf("******** WelCome To Test  **********\n");
    printf("************************************\n");
    printf("******** TCode=%02d]      **********\n",stCfg.CurrTypeCode);
    printf("************************************\n");

    switch (stCfg.CurrTypeCode)
    {
        case TCODE_CLIENT:
            
            break;
        case TCODE_SERVER:
            
            break;
        case TCODE_MSQ:
            TMain_MsQueue(&stCfg.stMsqCfg);
            break;
        case TCODE_IOBUF:
            TMain_Iobuf();
            break;
        default:
            
            break;
    }
    
    return 0;
}


