/*ͨ�õĶ�ȡJSON�Ĺ���*/

#include "UtilsMacro.h"


namespace UtilsFrame
{
    /*��ȡ�ļ�����*/
    bool            JsonReadFile(const char *pcFileName, char *pcBuffer, int iSize);
    
    /*ȫ·����ȡ�ļ�����*/
    bool            JsonReadFile(const std::string & sFullPath, char *pcBuffer, int iSize);
    
    /*��ȡ����ֵ*/
    int             JsonGetIntByName(const char *pcBuffer, const char  *pcName);
    
    /*��ȡ�༶����ֵ*/
    int             JsonGetIntByName(const char *pcBuffer, const char  *pcName, const char *pcSubName);
    
    /*��ȡ����ֵ*/
    const std::string     JsonGetStringByName(const char *pcBuffer, const char*pcName);
    
    /*��ȡ�༶����ֵ*/
    const std::string     JsonGetStringByName(const char *pcBuffer, const char*pcName, const char *pcSubName);
    
    int             JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex);

    int             JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex, const char *pcArrayName);

    int             JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned int iArryIndex);
    
    int             JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned int iArryIndex, const char *pcArrayName);

    const std::string     JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex);

    const std::string     JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex, const char *pcArrayName);
    
    const std::string     JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned int iArryIndex);

    const std::string     JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned int iArryIndex, const char *pcArrayName);

}


