/*通用的读取JSON的功能*/

#include "UtilsMacro.h"


namespace UtilsFrame
{
    /*读取文件内容*/
    bool            JsonReadFile(const char *pcFileName, char *pcBuffer, int iSize);
    
    /*全路径获取文件内容*/
    bool            JsonReadFile(const std::string & sFullPath, char *pcBuffer, int iSize);
    
    /*获取属性值*/
    int             JsonGetIntByName(const char *pcBuffer, const char  *pcName);
    
    /*获取多级属性值*/
    int             JsonGetIntByName(const char *pcBuffer, const char  *pcName, const char *pcSubName);
    
    /*获取属性值*/
    const std::string     JsonGetStringByName(const char *pcBuffer, const char*pcName);
    
    /*获取多级属性值*/
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


