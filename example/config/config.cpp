/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : config.cpp
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2019��7��28�� ������
  ����޸�   :
  ��������   : �����ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��28�� ������
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>
#include "config.h"


#define     TEST_CFGSEVR_PATHNAME       "config_server.json"

VOID config_init(PTEST_CONF_S      pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(TEST_CFGSEVR_PATHNAME, buffer, sizeof(buffer));
    
    strcpy((char *)pstCfg->acEthName, UtilsFrame::JsonGetStringByName(buffer, "ethname").c_str());
    pstCfg->CurrTypeCode            = UtilsFrame::JsonGetIntByName(buffer, "tcode");
    
    /**1. Client mode*/
    pstCfg->stClntCfg.TypeCode      = UtilsFrame::JsonGetIntByName(buffer, "client", "tcode");
    pstCfg->stClntCfg.HeadOffset    = UtilsFrame::JsonGetIntByName(buffer, "client", "headoffset");
    pstCfg->stClntCfg.BodySize      = UtilsFrame::JsonGetIntByName(buffer, "client", "body");
    pstCfg->stClntCfg.ServerPort    = UtilsFrame::JsonGetIntByName(buffer, "client", "port");
    pstCfg->stClntCfg.PthNums       = UtilsFrame::JsonGetIntByName(buffer, "client", "pthnums");
    pstCfg->stClntCfg.PthPerConns   = UtilsFrame::JsonGetIntByName(buffer, "client", "pthperconns");
    strcpy((char *)pstCfg->stClntCfg.acServerAddr, UtilsFrame::JsonGetStringByName(buffer, "client", "addr").c_str());
    
    /**2. Server mode*/
    pstCfg->stServCfg.TypeCode      = UtilsFrame::JsonGetIntByName(buffer, "server", "tcode");
    pstCfg->stServCfg.HeadOffset    = UtilsFrame::JsonGetIntByName(buffer, "server", "headoffset");
    pstCfg->stServCfg.ServerPort    = UtilsFrame::JsonGetIntByName(buffer, "server", "port");
    pstCfg->stServCfg.DirectEcho    = UtilsFrame::JsonGetIntByName(buffer, "server", "dctecho");
    pstCfg->stServCfg.ForwardEnable = UtilsFrame::JsonGetIntByName(buffer, "server", "fwenable");
    pstCfg->stServCfg.ForwardPort   = UtilsFrame::JsonGetIntByName(buffer, "server", "fwport");
    strcpy((char *)pstCfg->stServCfg.acServerAddr, UtilsFrame::JsonGetStringByName(buffer, "server", "addr").c_str());
    strcpy((char *)pstCfg->stServCfg.acForwardAddr, UtilsFrame::JsonGetStringByName(buffer, "server", "fwaddr").c_str());
    
    /**3. Message Queue mode*/
    pstCfg->stMsqCfg.TypeCode       = UtilsFrame::JsonGetIntByName(buffer, "msgque", "tcode");
    pstCfg->stMsqCfg.ProducerPths   = UtilsFrame::JsonGetIntByName(buffer, "msgque", "producer");
    pstCfg->stMsqCfg.ConsumerPths   = UtilsFrame::JsonGetIntByName(buffer, "msgque", "consumer");
    pstCfg->stMsqCfg.PthFreqUs      = UtilsFrame::JsonGetIntByName(buffer, "msgque", "pfreq-us");
    pstCfg->stMsqCfg.PthBatchs      = UtilsFrame::JsonGetIntByName(buffer, "msgque", "pfreq-batch");
    
    std::cout << "ethname=" << pstCfg->acEthName << std::endl;
    std::cout << "typecode=" << pstCfg->CurrTypeCode << std::endl;
}



