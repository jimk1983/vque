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
#include <vos/vos_platos.h>

#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <complex>
#include <limits>

/*��ҪC++11֧��*/
#if VOS_PLAT_SUPPORT_Class11
#include <array>
#endif

#include <vos/vos_pub.h>
#include <vrct/vrct_api.h>
#include "CEvtRctor.h"

namespace CVEvtFrame{
    
    void     CEvtRctor::EvtRctor_Init()
    {
        std::cout << "EvtRctor_Init() Entry" << std::endl;
        
    }
    
    void    CEvtRctor::EvtRctor_UnInit()
    {
        std::cout << "EvtRctor_UnInit() Entry" << std::endl;
    }
    
    CEvtRctor::CEvtRctor(int MsqSize)
    {
        m_msqSize = MsqSize;
        EvtRctor_Init();
        
    }
    
    CEvtRctor::~CEvtRctor()
    {
        EvtRctor_UnInit();
    }
}

