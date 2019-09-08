/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : UtilsToolStd.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2019��2��6��
  ����޸�   :
  ��������   : �Զ����STD�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��2��6��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

/****˵��****/
/****ģ��: �����ʵ��һ��Ҫ��ͷ�ļ���, ����������ͨ�����������ⲿ******/
#include "UtilsMacro.h"

namespace UtilsFrame
{
    /**********************************************************************/
    /************************** ��������ת�� ******************************/
    /**********************************************************************/
    template<typename T, typename K> 
    bool SetValue(const T& tSrc, K& tDst);
    
    template<typename T>
    bool SetValue(const T& tSrc, T& tDst);

    template<>
    bool SetValue(const std::string &sSrc, std::string &sDst);
    bool SetValue(const std::string &sSrc, std::string &sDst);
    
    /*********************************************************************/
    /*************************** �ַ������� ******************************/
    /*********************************************************************/
    /*ת����д*/
    bool ToUpper(std::string &sStr);
    bool ToUpper(const std::string &sSrc, std::string  &sDst);
    bool ToUpper(char *pcStr, int iLen);
    bool ToUpper(const char *pcSrc, int iSrcLen, char *pcDst, int iDstLen);
    
    /*ת��Сд*/
    bool ToLower(std::string &sStr);
    bool ToLower(const std::string &sSrc, std::string  &sDst);
    bool ToLower(char *pcStr, int iLen);
    bool ToLower(const char *pcSrc, int iSrcLen, char *pcDst, int iDstLen);
    
    /*********************************************************************/
    /*************************** ���ʹ��� ********************************/
    /*********************************************************************/
    /*�ж�2�������Ƿ�һ��*/
    template <typename T, typename K>
    bool IsOwnSelf(const T& tVct1, const K& tVct2); 

    /*��Դǿ��ת��ΪĿ�����͸�ֵ*/
    template <typename T, typename K>
    bool StaticCast(const T& tSrc, K& tDst);

    /*��ͬ���ͣ�ֱ�Ӹ�ֵ*/
    template <typename T>
    bool StaticCast(const T& tSrc, T& tDst);
};

template<typename T, typename K>
bool UtilsFrame::SetValue(const T& tSrc, K& tDst)
{
    if (IsOwnSelf(tSrc, tDst))
        return false;

    StaticCast(tSrc, tDst);
    return true;
}

template<typename T>
bool UtilsFrame::SetValue(const T& tSrc, T& tDst)
{
    if (IsOwnSelf(tSrc, tDst))
        return false;

    tDst = tSrc;
    return true;
}

template <typename T, typename K>
bool UtilsFrame::StaticCast(const T& tSrc, K& tDst)
{
    tDst = static_cast<K>(tSrc);
    return true;
}

template <typename T>
bool UtilsFrame::StaticCast(const T& tSrc, T& tDst)
{
    tDst = tSrc;
    return true;
}

template <typename T, typename K>
bool UtilsFrame::IsOwnSelf(const T& tVct1, const K& tVct2)
{
    const void* p1 = &tVct1;
    const void* p2 = &tVct2;
    return p1 == p2;
}