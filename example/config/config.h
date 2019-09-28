

typedef struct tagExampleEthMonitor
{
    char        ethname[MAX_PATH];
    UINT32_T    TimerSec;
}exm_eth_monitor_s,*pexm_eth_monitor_s;

typedef struct tagExampleClient
{
    UINT32_T    HeadMagic;
    UINT32_T    HeadOffset;
    UINT32_T    BodySize;
    CHAR        acAddr[SYS_IPV4LEN];
    UINT32_T    Port;
    UINT32_T    PthNums;
    UINT32_T    PthPerConns;
}exm_clnt_cfg_s,*pexm_clnt_cfg_s;


typedef struct tagExampleServer
{
    UINT32_T    HeadMagic;
    UINT32_T    HeadOffset;
    CHAR        acAddr[SYS_IPV4LEN];
    UINT32_T    Port;
    UINT32_T    EchoEnalbe;
}exm_serv_cfg_s,*pexm_serv_cfg_s;


typedef struct tagExampleProxy
{
    UINT32_T    HeadMagic;
    UINT32_T    HeadOffset;
    
    CHAR        acLocalAddr[SYS_IPV4LEN];
    UINT32_T    LocalPort;
    UINT32_T    ProxyPthNums;
    CHAR        acProxyAddr[SYS_IPV4LEN];
    UINT32_T    ProxyPort;
}exm_proxy_cfg_s,*pexm_proxy_cfg_s;

typedef enum
{
    EXM_CFGTYPE_UNKNOW =0,
    EXM_CFGTYPE_MONTOR,
    EXM_CFGTYPE_CLNT,
    EXM_CFGTYPE_SEVR,
    EXM_CFGTYPE_PRXY,
    
    EXM_CFGTYPE_NUMS
}EXMPLE_CFGTYPE_E;

VOID config_monitor_init(pexm_eth_monitor_s        pstCfg);
VOID config_clent_init(pexm_clnt_cfg_s       pstCfg);
VOID config_server_init(pexm_serv_cfg_s       pstCfg);
VOID config_proxy_init(pexm_proxy_cfg_s       pstCfg);


