/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : config.cpp
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2019年7月28日 星期日
  最近修改   :
  功能描述   : 配置文件
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月28日 星期日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>
#include "config.h"


#define     EXAMPLE_CFGMONR_PATHNAME       "./config_montor.json"
#define     EXAMPLE_CFGCLNT_PATHNAME       "./config_client.json"
#define     EXAMPLE_CFGSEVR_PATHNAME       "./config_server.json"
#define     EXAMPLE_CFGTPXY_PATHNAME       "./config_tcproxy.json"

VOID config_monitor_init(pexm_eth_monitor_s        pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(EXAMPLE_CFGMONR_PATHNAME, buffer, sizeof(buffer));
    
    strcpy((char *)pstCfg->ethname, UtilsFrame::JsonGetStringByName(buffer, "ethname").c_str());
    pstCfg->TimerSec                = UtilsFrame::JsonGetIntByName(buffer, "MonitorSecond");
    
    std::cout << "configure init monitor: " << std::endl;
    printf("    : ethname=%s\n",pstCfg->ethname);
    printf("    : MonitorSecond=%d\n",pstCfg->TimerSec);
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
    
    std::cout << "configure init client: " << std::endl;
    printf("    : HeadMagic=0x%08x\n",pstCfg->HeadMagic);
    printf("    : HeadOffset=%d\n",pstCfg->HeadOffset);
    printf("    : BodySize=%d\n",pstCfg->BodySize);
    printf("    : Address=%s\n",pstCfg->acAddr);
    printf("    : Port=%d\n",pstCfg->Port);
    printf("    : PthreadNums=%d\n",pstCfg->PthNums);
    printf("    : PthreadNums=%d\n",pstCfg->PthPerConns);
}

VOID config_server_init(pexm_serv_cfg_s       pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(EXAMPLE_CFGSEVR_PATHNAME, buffer, sizeof(buffer));
    
    pstCfg->HeadMagic               = UtilsFrame::JsonGetIntByName(buffer, "HeadMagic");
    pstCfg->HeadOffset              = UtilsFrame::JsonGetIntByName(buffer, "HeadOffset");
    strcpy((char *)pstCfg->acAddr, UtilsFrame::JsonGetStringByName(buffer, "Address").c_str());
    pstCfg->Port                    = UtilsFrame::JsonGetIntByName(buffer, "Port");
    pstCfg->EchoEnalbe              = UtilsFrame::JsonGetIntByName(buffer, "EchoEnable");

    std::cout << "configure init server: " << std::endl;
    printf("    : HeadMagic=0x%08x\n",pstCfg->HeadMagic);
    std::cout << "    : HeadOffset=" << pstCfg->HeadOffset <<std::endl;
    std::cout << "    : Address=" << pstCfg->acAddr <<std::endl;
    std::cout << "    : Port=" << pstCfg->Port <<std::endl;
    std::cout << "    : EchoEnalbe=" << pstCfg->EchoEnalbe <<std::endl;
}


VOID config_proxy_init(pexm_proxy_cfg_s       pstCfg)
{
    char buffer[2048] = {0};
    
    UtilsFrame::JsonReadFile(EXAMPLE_CFGTPXY_PATHNAME, buffer, sizeof(buffer));
    
    pstCfg->HeadMagic               = UtilsFrame::JsonGetIntByName(buffer, "HeadMagic");
    pstCfg->HeadOffset              = UtilsFrame::JsonGetIntByName(buffer, "HeadOffset");
    strcpy((char *)pstCfg->acLocalAddr, UtilsFrame::JsonGetStringByName(buffer, "LocalAddr").c_str());
    pstCfg->LocalPort                    = UtilsFrame::JsonGetIntByName(buffer, "LocalPort");
    pstCfg->ProxyPthNums                 = UtilsFrame::JsonGetIntByName(buffer, "ProxyPthNums");
    strcpy((char *)pstCfg->acProxyAddr, UtilsFrame::JsonGetStringByName(buffer, "ProxyAddr").c_str());
    pstCfg->ProxyPort                    = UtilsFrame::JsonGetIntByName(buffer, "ProxyPort");

    
    std::cout << "configure init proxy: " << std::endl;
    printf("    : HeadMagic=0x%08x\n",pstCfg->HeadMagic);
    std::cout << "    : LocalAddr=" << pstCfg->acLocalAddr <<std::endl;
    std::cout << "    : LocalPort=" << pstCfg->LocalPort <<std::endl;
    std::cout << "    : ProxyPthNums=" << pstCfg->ProxyPthNums <<std::endl;
    std::cout << "    : ProxyAddr=" << pstCfg->acProxyAddr <<std::endl;
    std::cout << "    : ProxyPort=" << pstCfg->ProxyPort <<std::endl;
}


