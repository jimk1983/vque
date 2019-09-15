/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �������޹�˾

 ******************************************************************************
  �� �� ��   : main.cpp
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2019��7��25�� ������
  ����޸�   :
  ��������   : vque�Ĳ��Դ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��25�� ������
    ��    ��   : jimk
    �޸�����   : �����ļ�

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


