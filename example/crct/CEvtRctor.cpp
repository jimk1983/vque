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

int     CEvtRctor::Init(int MsqSize)
{
    m_msqSize_ = MsqSize;
    std::cout << "CEvent Init! msqSize="<< m_msqSize_ << std::endl;
    return 0;
}

void    CEvtRctor::UnInit()
{
    std::cout << "CEvent UnInit!" << std::endl;
    
}

std::shared_ptr<CEvtRctor> CEvtRctor::GetSharedPtr()
{
    return shared_from_this();
}

int     CEvtRctor::GetMsqSize()
{
    return m_msqSize_;
}


CEvtRctor::CEvtRctor(int MsqSize):
        m_msqSize_(MsqSize)
{
    std::cout << "CEvtRctor Entry!" << std::endl;
}

CEvtRctor::~CEvtRctor()
{
    std::cout << "CEvtRctor Leave!" << std::endl;
}



