#include "UtilsMacro.h"
#include "UtilsToolStd.h"

/*��ע���ظ�����stringstream����:
**stringstream����Ĺ������������ͨ���Ƿǳ��ķ�CPUʱ���*/

template<>
bool UtilsFrame::SetValue(const std::string &sSrc, std::string &sDst)
{
    if (UtilsFrame::IsOwnSelf(sSrc, sDst))
        return false;

    sDst = sSrc;

    return true;
}

bool UtilsFrame::SetValue(const std::string &sSrc, std::string &sDst)
{
    if (UtilsFrame::IsOwnSelf(sSrc, sDst))
        return false;

    sDst = sSrc;
    return true;
}


/*ת���ɴ�д*/
bool UtilsFrame::ToUpper(std::string &sStr)
{
    std::transform(sStr.begin(), sStr.end(), sStr.begin(), ::toupper);
    return true;
}

bool UtilsFrame::ToUpper(const std::string &sSrc, std::string  &sDst)
{
    std::transform(sSrc.begin(), sSrc.end(), sDst.begin(), ::toupper);
    return true;
}

bool UtilsFrame::ToUpper(char *pcStr, int iLen)
{
    for (int i = 0; i < iLen; i++)
    {
        *pcStr = toupper(*pcStr);
        pcStr++;
    }
    return true;
}

bool UtilsFrame::ToUpper(const char *pcSrc, int iSrcLen, char *pcDst, int iDstLen)
{
    if (iDstLen < iSrcLen)
    {
        return false;
    }

    for (int i = 0; i < iSrcLen; i++)
    {
        *pcDst = toupper(*pcSrc);
        pcSrc++;
        pcDst++;
    }
    return true;
}


/*ת���ɴ�д*/
bool UtilsFrame::ToLower(std::string &sStr)
{
    std::transform(sStr.begin(), sStr.end(), sStr.begin(), ::tolower);
    return true;
}

bool UtilsFrame::ToLower(const std::string &sSrc, std::string  &sDst)
{
    std::transform(sSrc.begin(), sSrc.end(), sDst.begin(), ::tolower);
    return true;
}

bool UtilsFrame::ToLower(char *pcStr, int iLen)
{
    for (int i = 0; i < iLen; i++)
    {
        *pcStr = tolower(*pcStr);
        pcStr++;
    }
    return true;
}

bool UtilsFrame::ToLower(const char *pcSrc, int iSrcLen, char *pcDst, int iDstLen)
{
    if (iDstLen < iSrcLen)
    {
        return false;
    }

    for (int i = 0; i < iSrcLen; i++)
    {
        *pcDst = tolower(*pcSrc);
        pcSrc++;
        pcDst++;
    }
    return true;
}

