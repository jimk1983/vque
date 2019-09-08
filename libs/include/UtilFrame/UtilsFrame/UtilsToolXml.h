/*通用的读取配置文件的功能*/
/*需要实现的接口功能如下:
1. 获取根节点
2. 根据标签名获取当前节点的下一节点信息，可能有多个
3. 根据获取标签的属性信息
4. 获取根据特定的属性对，找到标签所有属性
5. 获取标签的中间值
6. 获取相对路径下的配置文件路径*/

/*规则:
  <ROOT> </ROOT> ---称之为Root
  <ROOT><Node>value</Node></ROOT> ---节点,节点中间可以设置值
  <ROOT><attr name="aaa"/></ROOT> ---节点的属性，属性可以有多个*/
#include "UtilsMacro.h"
#include <UtilFrame/opensrc/tinyxml/tinyxml.h>

namespace UtilsFrame
{
    /**********************************************************************/
    /************************** 文件加载和清理 ****************************/
    /**********************************************************************/
    /*根据文件缓存加载*/
    bool ParseFile(const std::string& sFileBuffer, TiXmlDocument& XmlDoc);
    /*加载XML文件*/
    bool LoadFile(const std::string& sFilePath, TiXmlDocument& XmlDoc);
    bool LoadFile(const char* pcFilePath, TiXmlDocument& XmlDoc);
    /*清理XML文件*/
    bool ClearFile(TiXmlDocument& XmlDoc);
    /*将xml文件转换到buffer中*/
    bool XDocToString(const TiXmlDocument& XmlDoc, std::string& sFileBuffer);
    
    /**********************************************************************/
    /************************** 节点解析和处理 ****************************/
    /**********************************************************************/
    /*获取单个根元素*/
    bool GetRootElement(const TiXmlDocument& XmlDoc, const std::string &sRootName, TiXmlElement **ppstRootElement);
    /*获取根元素集合*/
    bool GetRootElement(const TiXmlDocument& XmlDoc, const std::string &sRootName, std::vector<const TiXmlElement*> &vElements);

    /*是否为根节点*/
    bool IsRoot(const TiXmlNode* pstXmlNode);
    /**********************************************************************/
    /************************** 节点解析和处理 ****************************/
    /**********************************************************************/
    
    
    
}


