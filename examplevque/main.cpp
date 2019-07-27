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


