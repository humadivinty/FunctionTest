#include "stdafx.h"
#include "xmlFunction.h"

TiXmlElement* ReadElememt(TiXmlElement* InputElement, char* pName)
{
    TiXmlElement* ptemp = NULL;
    if (InputElement && 0 == strcmp(pName, InputElement->Value()))
    {
        printf("Find the element :%s \n", InputElement->Value());
        ptemp = InputElement;
        return ptemp;
    }
    else
    {
        printf("%s \n", InputElement->Value());
    }

    TiXmlElement* tmpElement = InputElement;
    if (tmpElement->FirstChildElement())
    {
        ptemp = ReadElememt(tmpElement->FirstChildElement(), pName);
    }
    if (!ptemp)
    {
        tmpElement = tmpElement->NextSiblingElement();
        if (tmpElement)
        {
            ptemp = ReadElememt(tmpElement, pName);
        }
    }
    return ptemp;
}

TiXmlElement SelectElementByName(const char* InputInfo, char* pName, int iXMLType)
{
    //注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
    TiXmlDocument cXmlDoc;
    TiXmlElement* pRootElement = NULL;
    if (iXMLType == 1)
    {
        if (!cXmlDoc.LoadFile(InputInfo))
        {
            printf("parse XML file failed \n");
            return TiXmlElement("");
        }
    }
    else if (iXMLType == 2)
    {
        if (!cXmlDoc.Parse(InputInfo))
        {
            printf("parse XML failed \n");
            return TiXmlElement("");
        }
    }

    pRootElement = cXmlDoc.RootElement();
    if (NULL == pRootElement)
    {
        printf("no have root Element\n");
        return TiXmlElement("");
    }
    else
    {
        TiXmlElement* pTempElement = NULL;
        pTempElement = ReadElememt(pRootElement, pName);
        if (pTempElement)
        {
            printf("find the Name : %s, Text = %s\n", pTempElement->Value(), pTempElement->GetText());
            return *pTempElement;
        }
        else
        {
            return TiXmlElement("");
        }
    }
}

bool GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen)
{
    if (pszAppendInfo == NULL || piRstBufLen == NULL || *piRstBufLen <= 0)
    {
        return false;
    }

    // <RoadNumber value="0" chnname="车道" />
    // <StreetName value="" chnname="路口名称" />
    std::string strAppendInfo = pszAppendInfo;
    size_t siStart = strAppendInfo.find(strItemName);
    if (siStart == std::string::npos)
    {
        return false;
    }
    siStart = strAppendInfo.find("\"", siStart + 1);
    if (siStart == std::string::npos)
    {
        return false;
    }
    size_t siEnd = strAppendInfo.find("\"", siStart + 1);
    if (siEnd == std::string::npos)
    {
        return false;
    }

    std::string strRst = strAppendInfo.substr(siStart + 1, siEnd - siStart - 1);
    if (*piRstBufLen < (int)strRst.length())
    {
        *piRstBufLen = (int)strRst.length();
        return false;
    }

    strncpy_s(pszRstBuf, *piRstBufLen, strRst.c_str(), (int)strRst.length());
    *piRstBufLen = (int)strRst.length();
    return true;
}

