/*ͨ�õĶ�ȡ�����ļ��Ĺ���*/
/*��Ҫʵ�ֵĽӿڹ�������:
1. ��ȡ���ڵ�
2. ���ݱ�ǩ����ȡ��ǰ�ڵ����һ�ڵ���Ϣ�������ж��
3. ���ݻ�ȡ��ǩ��������Ϣ
4. ��ȡ�����ض������Զԣ��ҵ���ǩ��������
5. ��ȡ��ǩ���м�ֵ
6. ��ȡ���·���µ������ļ�·��*/

/*����:
  <ROOT> </ROOT> ---��֮ΪRoot
  <ROOT><Node>value</Node></ROOT> ---�ڵ�,�ڵ��м��������ֵ
  <ROOT><attr name="aaa"/></ROOT> ---�ڵ�����ԣ����Կ����ж��*/
#include "UtilsMacro.h"
#include <UtilFrame/opensrc/tinyxml/tinyxml.h>

namespace UtilsFrame
{
    /**********************************************************************/
    /************************** �ļ����غ����� ****************************/
    /**********************************************************************/
    /*�����ļ��������*/
    bool ParseFile(const std::string& sFileBuffer, TiXmlDocument& XmlDoc);
    /*����XML�ļ�*/
    bool LoadFile(const std::string& sFilePath, TiXmlDocument& XmlDoc);
    bool LoadFile(const char* pcFilePath, TiXmlDocument& XmlDoc);
    /*����XML�ļ�*/
    bool ClearFile(TiXmlDocument& XmlDoc);
    /*��xml�ļ�ת����buffer��*/
    bool XDocToString(const TiXmlDocument& XmlDoc, std::string& sFileBuffer);
    
    /**********************************************************************/
    /************************** �ڵ�����ʹ��� ****************************/
    /**********************************************************************/
    /*��ȡ������Ԫ��*/
    bool GetRootElement(const TiXmlDocument& XmlDoc, const std::string &sRootName, TiXmlElement **ppstRootElement);
    /*��ȡ��Ԫ�ؼ���*/
    bool GetRootElement(const TiXmlDocument& XmlDoc, const std::string &sRootName, std::vector<const TiXmlElement*> &vElements);

    /*�Ƿ�Ϊ���ڵ�*/
    bool IsRoot(const TiXmlNode* pstXmlNode);
    /**********************************************************************/
    /************************** �ڵ�����ʹ��� ****************************/
    /**********************************************************************/
    
    
    
}


