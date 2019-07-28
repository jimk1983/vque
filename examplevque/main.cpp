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

#include "config/config.h"

int main(int argc, char *argv[])
{
    TEST_CONF_S    stCfg={0};
    
    config_init(&stCfg);

    printf("Test End!\n");
    return 0;
}


