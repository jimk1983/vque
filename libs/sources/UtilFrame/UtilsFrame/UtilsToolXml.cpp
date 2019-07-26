
#include "UtilsToolStd.h"
#include "UtilsToolXml.h"


/*加载XML文件*/
bool UtilsFrame::LoadFile(const std::string & sFullPath, TiXmlDocument& XmlDoc)
{
    if ( true != XmlDoc.LoadFile(sFullPath.c_str()) )
    {
        return false;
    }

    return true;
}

/*加载XML文件*/
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
/*清理XML文件*/
bool UtilsFrame::ClearFile(TiXmlDocument& XmlDoc)
{
    XmlDoc.Clear();
    return true;
}

/*根据文件缓存加载*/
bool UtilsFrame::ParseFile(const std::string& sFileBuffer, TiXmlDocument& XmlDoc)
{
    XmlDoc.Parse(sFileBuffer.c_str());
    return !XmlDoc.Error();
}

/*将xml文件转换到buffer中*/
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
