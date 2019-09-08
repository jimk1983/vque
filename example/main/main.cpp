#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include <UtilFrame/UtilsFrame.h>

#include <getopt.h>
#include "config/config.h"
#include "server/server.h"

//no_argument：0,required_argument:1,optional_argument:2
static const struct option long_options[] = { 
    {"start   ",    required_argument,  NULL, 'b'}, 
    {"stop    ",    optional_argument,  NULL, 'q'}, 
    {"restart ",    required_argument,  NULL, 'r'},
    {"version ",    optional_argument,  NULL, 'v'},
    {"server  ",    optional_argument,  NULL, 's'},
    {"client  ",    optional_argument,  NULL, 'c'},
    {"tcpproxy",    optional_argument,  NULL, 't'},
    {"address ",    required_argument,  NULL, 'a'},
    {"port    ",    required_argument,  NULL, 'p'},
    {"help    ",    optional_argument,  NULL, 'h'}, 
    {0, 0, 0, 0}
};

#define print_opt_help(opt_index, help_str)             \
do {                                \
    printf("\t---%s\t-%c\t%s", long_options[opt_index].name, (char)long_options[opt_index].val, help_str); \
} while (0)

void option_usage() {
    printf("Usage:\n");
    print_opt_help(0,  "action start with config_*.json, example: -s 0(0:monitor,1:client,2:server,3:proxy)\n");
    print_opt_help(1,  "action stop \n");
    print_opt_help(2,  "action restart with config_*.json\n");
    print_opt_help(3,  "action version \n");
    print_opt_help(4,  "server mode.     example:    --server \n");
    print_opt_help(5,  "client mode.     example:    --client \n");
    print_opt_help(6,  "tcpproxy mode.   example:    --tcpproxy \n");
    print_opt_help(7,  "address.         example:    --address  192.168.1.1\n");
    print_opt_help(8,  "port.            example:    --port     9527\n");
    print_opt_help(9,  "help.        \n");
    
    printf("Examples:\n");
    printf("\t./app-test -s -p 9527                 --- action server listen port is 9527\n");
    printf("\t./app-test -c -a 192.168.1.1 -p 9527  --- action client connect addr is 192.168.1.1:9527\n");
}

enum
{
    ARGV_MODE_CFGFILE = 0,      /*配置文件模式*/
    ARGV_MODE_SERVER,           /*服务端*/
    ARGV_MODE_CLIENT,           /*客户端*/
    ARGV_MDOE_MQUE,             /*消息队列*/
    ARGV_MODE_TCPPROXY,         /*TCP代理模式*/
    
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
}argv_actinfo_s,*pargv_actinfo_s;

static char optstring[] = "b:qr:vscta:p:h";

void        start(pargv_actinfo_s pstinfo)
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
            case 'b':
                stactinfo.mode = ARGV_MODE_CFGFILE;
                stactinfo.cfgtype = std::atoi(optarg);
                start(&stactinfo);
                break;
            case 'q':
                stop();
                break;
            case 'r':
                stactinfo.mode = ARGV_MODE_CFGFILE;
                stactinfo.cfgtype = std::atoi(optarg);
                restart(&stactinfo);
                break;
            case 'v':
                version();
                break;
            case 's':
                stactinfo.mode = ARGV_MODE_SERVER;
                server(&stactinfo);
                break;
            case 'c':
                stactinfo.mode = ARGV_MODE_CLIENT;
                client(&stactinfo);
                break;
            case 't':
                stactinfo.mode = ARGV_MODE_TCPPROXY;
                tcpproxy(&stactinfo);
                break;
            case 'a':
                switch(stactinfo.mode)
                {
                    case ARGV_MODE_CLIENT:
                    case ARGV_MODE_SERVER:
                        strcpy(stactinfo.addr,      optarg);
                        break;
                    case ARGV_MODE_TCPPROXY:
                        strcpy(stactinfo.pxy_addr,  optarg);
                        break;
                    default:
                        break;
                }
                break;
            case 'p':
                switch(stactinfo.mode)
                {
                    case ARGV_MODE_CLIENT:
                    case ARGV_MODE_SERVER:
                        stactinfo.port      = std::atoi(optarg);
                        break;
                    case ARGV_MODE_TCPPROXY:
                        stactinfo.pxy_port  = std::atoi(optarg);
                        break;
                    default:
                        break;
                }
                break;
            case 'h':
                option_usage();
                break;
            default:
                option_usage();
            return -1;
        }
    }
    
    return 0;
}


