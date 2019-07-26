
#include "UtilsToolStd.h"
#include "UtilsToolXml.h"


/*����XML�ļ�*/
bool UtilsFrame::LoadFile(const std::string & sFullPath, TiXmlDocument& XmlDoc)
{
    if ( true != XmlDoc.LoadFile(sFullPath.c_str()) )
    {
        return false;
    }

    return true;
}

/*����XML�ļ�*/
bool UtilsFrame::LoadFile(const char* pcFullPath, TiXmlDocument& XmlDoc)
{
    if ( NULL == pcFullPath)
    {
        return false;
    }

    if (true != XmlDoc.LoadFile(pcFullPath))
    {
        return false;
    }

    return true;
}
/*����XML�ļ�*/
bool UtilsFrame::ClearFile(TiXmlDocument& XmlDoc)
{
    XmlDoc.Clear();
    return true;
}

/*�����ļ��������*/
bool UtilsFrame::ParseFile(const std::string& sFileBuffer, TiXmlDocument& XmlDoc)
{
    XmlDoc.Parse(sFileBuffer.c_str());
    return !XmlDoc.Error();
}

/*��xml�ļ�ת����buffer��*/
bool UtilsFrame::XDocToString(const TiXmlDocument& XmlDoc, std::string &sFileBuffer)
{
    TiXmlPrinter printer;
    XmlDoc.Accept(&printer);
    
    sFileBuffer = (0 == printer.Size() ? "" : printer.CStr());
    return true;
}

bool UtilsFrame::GetRootElement(const TiXmlDocument& XmlDoc, const std::string &sRootName, TiXmlElement **ppstRootElement)
{

    return true;
}

bool UtilsFrame::GetRootElement(const TiXmlDocument& XmlDoc, const std::string &sRootName, std::vector<const TiXmlElement*> &vElements)
{

    return true;
}


bool UtilsFrame::IsRoot(const TiXmlNode* pstXmlNode)
{
    if (!pstXmlNode->Parent())
        return true;

    if (!pstXmlNode->Parent()->Parent())
        return true;

    return false;
}
