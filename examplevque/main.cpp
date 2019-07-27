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


int main(int argc, char *argv[])
{
    VOID *pvRctor = NULL;
    char buffer[2048] = {0};
    
    //std::string sValue;
    //UtilsFrame::JsonReadFile("1.json", buffer, sizeof(buffer));
    
    while(1)
    {
        pvRctor = VRCT_API_Create(0, 100000);
        if ( NULL == pvRctor )
        {
            PError("Reactor create failed!");
            return -1;
        }
        
        if( VOS_ERR == VRCT_API_Start(pvRctor) )
        {
            PError("Reactor start failed!");
            return -1;
        }

        
        
        //PEvent("system-time=%s", VOS_GetSysTimeNowStr());
        VOS_Sleep(300);
        
        VRCT_API_Release(&pvRctor);
    }
    return 0;
}


