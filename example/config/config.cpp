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


#define     EXAMPLE_CFGMONR_PATHNAME       "config_montor.json"
#define     EXAMPLE_CFGCLNT_PATHNAME       "config_client.json"
#define     EXAMPLE_CFGSEVR_PATHNAME       "config_server.json"

VOID config_monitor_init(pexm_eth_monitor_s        pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(EXAMPLE_CFGMONR_PATHNAME, buffer, sizeof(buffer));
    
    strcpy((char *)pstCfg->ethname, UtilsFrame::JsonGetStringByName(buffer, "ethname").c_str());
    pstCfg->TimerSec                = UtilsFrame::JsonGetIntByName(buffer, "MonitorSecond");
}

VOID config_clent_init(pexm_clnt_cfg_s       pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(EXAMPLE_CFGCLNT_PATHNAME, buffer, sizeof(buffer));
    
    pstCfg->HeadMagic               = UtilsFrame::JsonGetIntByName(buffer, "HeadMagic");
    pstCfg->HeadOffset              = UtilsFrame::JsonGetIntByName(buffer, "HeadOffset");
    pstCfg->BodySize                = UtilsFrame::JsonGetIntByName(buffer, "BodySize");
    strcpy((char *)pstCfg->acAddr, UtilsFrame::JsonGetStringByName(buffer, "Address").c_str());
    pstCfg->Port                    = UtilsFrame::JsonGetIntByName(buffer, "Port");
    pstCfg->PthNums                 = UtilsFrame::JsonGetIntByName(buffer, "PthreadNums");
    pstCfg->PthPerConns             = UtilsFrame::JsonGetIntByName(buffer, "PthreadConnNums");
}

VOID config_server_init(pexm_serv_cfg_s       pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(EXAMPLE_CFGCLNT_PATHNAME, buffer, sizeof(buffer));
    
    pstCfg->HeadMagic               = UtilsFrame::JsonGetIntByName(buffer, "HeadMagic");
    pstCfg->HeadOffset              = UtilsFrame::JsonGetIntByName(buffer, "HeadOffset");
    strcpy((char *)pstCfg->acAddr, UtilsFrame::JsonGetStringByName(buffer, "Address").c_str());
    pstCfg->Port                    = UtilsFrame::JsonGetIntByName(buffer, "Port");
    pstCfg->EchoEnalbe                 = UtilsFrame::JsonGetIntByName(buffer, "EchoEnable");
}


VOID config_proxy_init(pexm_proxy_cfg_s       pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(EXAMPLE_CFGCLNT_PATHNAME, buffer, sizeof(buffer));
    
    pstCfg->HeadMagic               = UtilsFrame::JsonGetIntByName(buffer, "HeadMagic");
    pstCfg->HeadOffset              = UtilsFrame::JsonGetIntByName(buffer, "HeadOffset");
    strcpy((char *)pstCfg->acLocalAddr, UtilsFrame::JsonGetStringByName(buffer, "LocalAddr").c_str());
    pstCfg->LocalPort                    = UtilsFrame::JsonGetIntByName(buffer, "LocalPort");
    pstCfg->ProxyPthNums                 = UtilsFrame::JsonGetIntByName(buffer, "ProxyPthNums");
    strcpy((char *)pstCfg->acProxyAddr, UtilsFrame::JsonGetStringByName(buffer, "ProxyAddr").c_str());
    pstCfg->ProxyPort                    = UtilsFrame::JsonGetIntByName(buffer, "ProxyPort");
}


