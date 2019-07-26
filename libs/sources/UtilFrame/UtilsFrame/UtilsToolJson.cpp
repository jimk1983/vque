/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : UtilsToolJson.cpp
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2019年6月6日
  最近修改   :
  功能描述   : json操作
  函数列表   :
              UtilsFrame.JsonReadFile
              UtilsFrame.JsonReadFile
  修改历史   :
  1.日    期   : 2019年6月6日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#include "UtilsToolStd.h"
#include "UtilsToolJson.h"

#include <UtilFrame/opensrc/rapidjson/document.h>
#include <UtilFrame/opensrc/rapidjson/filereadstream.h>
#include <UtilFrame/opensrc/rapidjson/filewritestream.h>   
#include <UtilFrame/opensrc/rapidjson/prettywriter.h>
#include <UtilFrame/opensrc/rapidjson/stringbuffer.h>  
#include <UtilFrame/opensrc/rapidjson/writer.h>
#include <UtilFrame/opensrc/rapidjson/memorystream.h>
#include <UtilFrame/opensrc/rapidjson/rapidjson.h>
#include <UtilFrame/opensrc/rapidjson/error/error.h>
#include <UtilFrame/opensrc/rapidjson/error/en.h>

using namespace std;
using namespace rapidjson;


/*****************************************************************************
 函 数 名  : UtilsFrame.JsonReadFile
 功能描述  : 相同目录下文件读取
 输入参数  : const char *pcFileName  
             char *pcBuff            
             int32 iSize             
 输出参数  : 无
 返 回 值  : bool
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
bool UtilsFrame::JsonReadFile(const char *pcFileName, char *pcBuff, int iSize)
{
    int     iRet                = 0;
    int     index               = 0;
    char    acFileName[MAX_PATH]= {0};
    
    iRet = readlink("/proc/self/exe", acFileName, MAX_PATH-1);
    if (iRet < 0 || iRet >= MAX_PATH-1)
    {
        printf("Readlink error=%d:%s!\n", errno, strerror(errno));
        return false;
    }
    
    //获取当前目录绝对路径，即去掉程序名
    for (index = iRet; index >=0; --index)
    {
        if ( acFileName[index] == '/')
        {
            acFileName[index+1] = '\0';
            break;
        }
    }
    
    strcat(acFileName, pcFileName);
    
    FILE *fp = fopen(acFileName, "rb");
    if ( NULL == fp )
    {
        printf("fopen error=%d:%s, path=%s!\n", errno, strerror(errno), acFileName);
        return false;
    }
    
    rapidjson::FileReadStream is(fp, pcBuff, iSize);
    
    return true;
}
    

/*****************************************************************************
 函 数 名  : UtilsFrame.JsonReadFile
 功能描述  : 通过全路径获取
 输入参数  : const std::string & sFullPath  
             char *pcBuffer                 
 输出参数  : 无
 返 回 值  : bool
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
bool UtilsFrame::JsonReadFile(const std::string & sFullPath, char *pcBuff, int iSize)
{
    FILE *fp = fopen(sFullPath.c_str(), "rb");
    if ( NULL == fp )
    {
        printf("fopen error=%d:%s, path=%s!\n", errno, strerror(errno), sFullPath.c_str());
        return false;
    }
    
    rapidjson::FileReadStream is(fp, pcBuff, iSize);
    
    return true;
}

/*****************************************************************************
 函 数 名  : UtilsFrame.JsonGetIntByName
 功能描述  : 获取一层属性值
 输入参数  : const char *pcBuffer  
             const char  *pcName   
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
int     UtilsFrame::JsonGetIntByName(const char *pcBuffer, const char  *pcName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return 0;
    }
    
    const rapidjson::Value &Object = document[pcName];
    if ( !Object.IsInt()  )
    {
        return 0;
    }
    
    return Object.GetInt();
}


/*****************************************************************************
 函 数 名  : UtilsFrame.JsonGetIntByName
 功能描述  : 获取二层属性值
 输入参数  : const char *pcBuffer  
             const char  *pcName   
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
int     UtilsFrame::JsonGetIntByName(const char *pcBuffer, const char  *pcName, const char *pcSubName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return 0;
    }
    
    const rapidjson::Value &Object = document[pcName];
    if(!Object.IsObject())
    {
    	return 0;
    }

    if(!Object.HasMember(pcSubName))
    {
        return 0;
    }
    
    const rapidjson::Value &value = Object[pcSubName];
    if ( !value.IsInt()  )
    {
        return 0;
    }
    
    return value.GetInt();

}

/*****************************************************************************
 函 数 名  : UtilsFrame.JsonGetIntByName
 功能描述  : 获取一层属性值
 输入参数  : const char *pcBuffer  
             const char  *pcName   
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/

const std::string  UtilsFrame::JsonGetStringByName(const char *pcBuffer, const char*pcName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return "";
    }
    
    const rapidjson::Value &Object = document[pcName];
    if ( !Object.IsString()  )
    {
        return "";
    }
    
    return Object.GetString();
}

/*****************************************************************************
 函 数 名  : UtilsFrame.JsonGetIntByName
 功能描述  : 获取二层属性值
 输入参数  : const char *pcBuffer  
             const char  *pcName   
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月6日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/

const std::string  UtilsFrame::JsonGetStringByName(const char *pcBuffer, const char*pcName, const char *pcSubName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return "";
    }
    
    const rapidjson::Value &Object = document[pcName];
    if(!Object.IsObject())
    {
    	return "";
    }

    if(!Object.HasMember(pcSubName))
    {
        return "";
    }
    
    const rapidjson::Value &value = Object[pcSubName];
    if ( !value.IsString()  )
    {
        return "";
    }
    
    return value.GetString();
}


/*获取属性值*/
int     UtilsFrame::JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return 0;
    }
    
    const rapidjson::Value &Object = document[pcName];
    if ( !Object.IsArray() )
    {
        return 0;
    }
    
    if ( iArryIndex >= Object.Size() )
    {
        return 0;
    }
    
    const rapidjson::Value &vArray = Object[iArryIndex];
    if ( !vArray.IsInt()  )
    {
        return 0;
    }
    
    return vArray.GetInt();
}


/*获取属性值*/
int     UtilsFrame::JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex, const char *pcArrayName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return 0;
    }
    
    const rapidjson::Value &Object = document[pcName];
    if ( !Object.IsArray() )
    {
        return 0;
    }
    
    if ( iArryIndex >= Object.Size() )
    {
        return 0;
    }
    
    const rapidjson::Value &vArray = Object[iArryIndex];
    if(!vArray.HasMember(pcArrayName))
    {
        return 0;
    }
    
    const rapidjson::Value &vMember = vArray[pcArrayName];
    if ( !vMember.IsInt()  )
    {
        return 0;
    }
    
    return vMember.GetInt();
}

/*获取多级属性值*/
int     UtilsFrame::JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned  int iArryIndex)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return 0;
    }
    
    const rapidjson::Value &Object = document[pcName];
    if(!Object.IsObject())
    {
    	return 0;
    }

    if(!Object.HasMember(pcSubName))
    {
        return 0;
    }
    
    const rapidjson::Value &value = Object[pcSubName];
    if ( !value.IsArray() )
    {
        return 0;
    }
    
    if ( iArryIndex >= value.Size() )
    {
        return 0;
    }
    
    const rapidjson::Value &vMember = Object[iArryIndex];
    if ( !vMember.IsInt()  )
    {
        return 0;
    }
    
    return vMember.GetInt();
}



/*获取多级属性值*/
int     UtilsFrame::JsonGetIntByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned int iArryIndex, const char *pcArrayName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return 0;
    }
    
    const rapidjson::Value &Object = document[pcName];
    if(!Object.IsObject())
    {
    	return 0;
    }

    if(!Object.HasMember(pcSubName))
    {
        return 0;
    }
    
    const rapidjson::Value &value = Object[pcSubName];
    if ( !value.IsArray() )
    {
        return 0;
    }
    
    if ( iArryIndex >= value.Size() )
    {
        return 0;
    }
    
    const rapidjson::Value &vArray = Object[iArryIndex];
    if(!vArray.HasMember(pcArrayName))
    {
        return 0;
    }
    
    const rapidjson::Value &vMember = vArray[pcArrayName];
    if ( !vMember.IsInt()  )
    {
        return 0;
    }
    return vArray.GetInt();
}


/*获取属性值*/
const std::string     UtilsFrame::JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return "";
    }
    
    const rapidjson::Value &Object = document[pcName];
    if ( !Object.IsArray() )
    {
        return "";
    }
    
    if ( iArryIndex >= Object.Size() )
    {
        return "";
    }
    
    const rapidjson::Value &vArray = Object[iArryIndex];
    if ( !vArray.IsString()  )
    {
        return "";
    }
    
    return vArray.GetString();
}


/*获取属性值*/
const std::string    UtilsFrame::JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const unsigned int iArryIndex, const char *pcArrayName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return "";
    }
    
    const rapidjson::Value &Object = document[pcName];
    if ( !Object.IsArray() )
    {
        return "";
    }
    
    if ( iArryIndex >= Object.Size() )
    {
        return "";
    }
    
    const rapidjson::Value &vArray = Object[iArryIndex];
    if(!vArray.HasMember(pcArrayName))
    {
        return "";
    }
    
    const rapidjson::Value &vMember = vArray[pcArrayName];
    if ( !vMember.IsString()  )
    {
        return "";
    }
    
    return vMember.GetString();
}

/*获取多级属性值*/
const std::string     UtilsFrame::JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned int iArryIndex)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return "";
    }
    
    const rapidjson::Value &Object = document[pcName];
    if(!Object.IsObject())
    {
    	return "";
    }

    if(!Object.HasMember(pcSubName))
    {
        return "";
    }
    
    const rapidjson::Value &value = Object[pcSubName];
    if ( !value.IsArray() )
    {
        return "";
    }
    
    if ( iArryIndex >= value.Size() )
    {
        return "";
    }
    
    const rapidjson::Value &vMember = Object[iArryIndex];
    if ( !vMember.IsString()  )
    {
        return "";
    }
    
    return vMember.GetString();
}



/*获取多级属性值*/
const std::string  UtilsFrame::JsonGetStringByArryName(const char *pcBuffer, const char  *pcName, const char *pcSubName, const unsigned int iArryIndex, const char *pcArrayName)
{
    Document document;
    
    if ( document.Parse(pcBuffer).HasParseError() 
        || !document.HasMember(pcName) ) 
    {
        return "";
    }
    
    const rapidjson::Value &Object = document[pcName];
    if(!Object.IsObject())
    {
        return "";
    }

    if(!Object.HasMember(pcSubName))
    {
        return "";
    }
    
    const rapidjson::Value &value = Object[pcSubName];
    if ( !value.IsArray() )
    {
        return "";
    }
    
    if ( iArryIndex >= value.Size() )
    {
        return "";
    }
    
    const rapidjson::Value &vArray = Object[iArryIndex];
    if(!vArray.HasMember(pcArrayName))
    {
        return "";
    }
    
    const rapidjson::Value &vMember = vArray[pcArrayName];
    if ( !vMember.IsString()  )
    {
        return "";
    }
    
    return vArray.GetString();
}


