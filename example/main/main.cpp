#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include <getopt.h>
#include "config/config.h"
#include "server/server.h"
#include "proxy/proxy.h"

//no_argument：0,required_argument:1,optional_argument:2
static const struct option long_options[] = { 
    {"start   ",    optional_argument,  NULL, 's'}, 
    {"exist    ",   optional_argument,  NULL, 'e'}, 
    {"version ",    optional_argument,  NULL, 'v'},
    {"address ",    required_argument,  NULL, 'a'},
    {"listen  ",    required_argument,  NULL, 'l'},
    {"remote-addr", required_argument,  NULL, 'r'},
    {"remote-port", required_argument,  NULL, 'p'},
    {"mode ",       required_argument,  NULL, 'm'},
    {"core ",       required_argument,  NULL, 'c'},
    {"help    ",    optional_argument,  NULL, 'h'}, 
    {0, 0, 0, 0}
};

#define print_opt_help(opt_index, help_str)             \
do {                                \
    printf("\t---%s\t-%c\t%s", long_options[opt_index].name, (char)long_options[opt_index].val, help_str); \
} while (0)

void option_usage() {
    printf("Usage:\n");
    print_opt_help(0,  "action  start with config_*.json, example: -s 0(0:monitor,1:client,2:server,3:proxy)\n");
    print_opt_help(1,  "action  exit \n");
    print_opt_help(2,  "display version \n");
    print_opt_help(3,  "address.                        -a 192.168.1.1\n");
    print_opt_help(4,  "listen port.                    -l 9527 \n");
    print_opt_help(5,  "remote server address.          -r 192.168.1.100 \n");
    print_opt_help(6,  "remote server port..            -p 3306\n");
    print_opt_help(7,  "mode.                           -m (0:monitor,1:client,2:server,3:proxy)\n");
    print_opt_help(8,  "core..                          -c 2\n");
    print_opt_help(9,  "help.        \n");
    
    printf("Examples:\n");
    printf("\t./app-test -l 9527 -r 192.168.1.300 -p 3306 -m 3 \n");
}

enum
{
    ARGV_MODE_CFGFILE = 0,      /*配置文件模式*/
    ARGV_MODE_SERVER,           /*服务端*/
    ARGV_MODE_CLIENT,           /*客户端*/
    ARGV_MDOE_MQUE,             /*消息队列*/
    ARGV_MODE_PROXY,            /*TCP代理模式*/
    
    ARGV_MODE_NUMS
};
    
typedef struct tag_argv_action_Info
{
    int32_t     mode;
    int32_t     cfgtype;
    char        addr[32];
    int32_t     port;
    char        pxy_addr[32];
    int32_t     pxy_port;
    int32_t     core_nums;
}argv_actinfo_s,*pargv_actinfo_s;

static char optstring[] = "sev:a:l:r:p:m:c:h";

void     start_with_config(pargv_actinfo_s pstinfo)
{
    switch(pstinfo->cfgtype)
    {
        case EXM_CFGTYPE_MONTOR:
            {
                exm_eth_monitor_s   stcfg;
                config_monitor_init(&stcfg);
            }
            break;
        case EXM_CFGTYPE_CLNT:
            {
                exm_clnt_cfg_s      stcfg;
                config_clent_init(&stcfg);
            }
            break;
        case EXM_CFGTYPE_SEVR:
            {
                exm_serv_cfg_s      stcfg;
                config_server_init(&stcfg);
                server_main(&stcfg);
            }
            break;
        case EXM_CFGTYPE_PRXY:
            {
                exm_proxy_cfg_s     stcfg;
                config_proxy_init(&stcfg);
            }
            break;
        default:
            break;
    }
}

/*简单模型*/
void     start(pargv_actinfo_s pstinfo)
{
    switch(pstinfo->mode)
    { 
        case ARGV_MODE_SERVER:
            {
                exm_serv_cfg_s      stcfg;
                /*默认开启*/
                stcfg.EchoEnalbe = 1;
                strcpy((char*)stcfg.acAddr, pstinfo->addr);
                stcfg.Port = pstinfo->port;
                server_main(&stcfg);
            }
            break;
        case ARGV_MODE_CLIENT:
            {
                
            }
            break;
        case ARGV_MDOE_MQUE:
            {
                
            }
            break;
        case ARGV_MODE_PROXY:
            {
                printf("====>start proxy_main entry!\n");
                exm_proxy_cfg_s      stcfg;
                
                /*默认开启2个线程转发*/
                stcfg.ProxyPthNums = pstinfo->core_nums;
                strcpy((char*)stcfg.acLocalAddr, pstinfo->addr);
                stcfg.LocalPort = pstinfo->port;
                strcpy((char*)stcfg.acProxyAddr, pstinfo->pxy_addr);
                stcfg.ProxyPort = pstinfo->pxy_port;
                proxy_main(&stcfg);
            }
            break;
        default:
            break;
    }
}


void        stop()
{
    
}

void        restart(pargv_actinfo_s pstinfo)
{
    stop();
    start(pstinfo);
}

void        version()
{
    
}

void        server(pargv_actinfo_s pstinfo)
{
    
}

void        client(pargv_actinfo_s pstinfo)
{
    
}

void        tcpproxy(pargv_actinfo_s pstinfo)
{
    
}


int main(int argc, char *argv[])
{
    int opt_c                   = 0;
    argv_actinfo_s  stactinfo   = {0};
    
    while ((opt_c = getopt_long(argc, argv, optstring, long_options, NULL)) != EOF) {
        switch (opt_c) {
            case 's':
                stactinfo.mode = ARGV_MODE_CFGFILE;
                return 0;
            case 'e':
                stop();
                break;
            case 'v':
                version();
                break;
            case 'a':
                strcpy(stactinfo.addr,  optarg);
                break;
            case 'l':
                stactinfo.port          = std::atoi(optarg);
                break;
            case 'r':
                strcpy(stactinfo.pxy_addr,  optarg);
                break;
            case 'p':
                stactinfo.pxy_port      = std::atoi(optarg);
                break;
            case 'm':
                stactinfo.mode = std::atoi(optarg);
                break;
            case 'c':
                stactinfo.core_nums = std::atoi(optarg);
                break;
            case 'h':
                option_usage();
                break;
            default:
                option_usage();
            return -1;
        }
    }
    
    if( 0 == stactinfo.core_nums)
    {
        stactinfo.core_nums = 1;
    }
    
    printf("stactinfo: addr=%s\n", stactinfo.addr);
    printf("         : port=%d\n", stactinfo.port);
    printf("         : proxy addr=%s\n", stactinfo.pxy_addr);
    printf("         : proxy port=%d\n", stactinfo.pxy_port);
    printf("         : mode=%d\n", stactinfo.mode);
    printf("         : core=%d\n", stactinfo.core_nums);
    
    /*通过配置文件启动*/
    if ( stactinfo.mode == ARGV_MODE_CFGFILE) {
        start_with_config(&stactinfo);
    }
    else {
    /*简单的参数启动*/
        start(&stactinfo);
    }
                
    
    
    
    return 0;
}


