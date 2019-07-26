/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : UtilsToolJson.cpp
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2019��6��6��
  ����޸�   :
  ��������   : json����
  �����б�   :
              UtilsFrame.JsonReadFile
              UtilsFrame.JsonReadFile
  �޸���ʷ   :
  1.��    ��   : 2019��6��6��
    ��    ��   : ����
    �޸�����   : �����ļ�

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
 �� �� ��  : UtilsFrame.JsonReadFile
 ��������  : ��ͬĿ¼���ļ���ȡ
 �������  : const char *pcFileName  
             char *pcBuff            
             int32 iSize             
 �������  : ��
 �� �� ֵ  : bool
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    
    //��ȡ��ǰĿ¼����·������ȥ��������
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
 �� �� ��  : UtilsFrame.JsonReadFile
 ��������  : ͨ��ȫ·����ȡ
 �������  : const std::string & sFullPath  
             char *pcBuffer                 
 �������  : ��
 �� �� ֵ  : bool
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : UtilsFrame.JsonGetIntByName
 ��������  : ��ȡһ������ֵ
 �������  : const char *pcBuffer  
             const char  *pcName   
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : UtilsFrame.JsonGetIntByName
 ��������  : ��ȡ��������ֵ
 �������  : const char *pcBuffer  
             const char  *pcName   
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : UtilsFrame.JsonGetIntByName
 ��������  : ��ȡһ������ֵ
 �������  : const char *pcBuffer  
             const char  *pcName   
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : UtilsFrame.JsonGetIntByName
 ��������  : ��ȡ��������ֵ
 �������  : const char *pcBuffer  
             const char  *pcName   
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��6��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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


/*��ȡ����ֵ*/
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


/*��ȡ����ֵ*/
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

/*��ȡ�༶����ֵ*/
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



/*��ȡ�༶����ֵ*/
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


/*��ȡ����ֵ*/
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


/*��ȡ����ֵ*/
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

/*��ȡ�༶����ֵ*/
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



/*��ȡ�༶����ֵ*/
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


