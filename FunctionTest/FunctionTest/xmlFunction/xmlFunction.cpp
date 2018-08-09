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

bool GetPropertyValueDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen)
{
    if (pszAppendInfo == NULL || piRstBufLen == NULL || *piRstBufLen <= 0)
    {
        return false;
    }

    // <RoadNumber value="0" chnname="车道" />
    // <StreetName value="" chnname="路口名称" />
    try
    {
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
    catch (std::bad_exception& e)
    {
        printf("Tool_GetDataFromAppenedInfo, bad_exception, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::overflow_error& e)
    {
        printf("Tool_GetDataFromAppenedInfo, overflow_error, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::domain_error& e)
    {
        printf("Tool_GetDataFromAppenedInfo, domain_error, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::length_error& e)
    {
        printf("Tool_GetDataFromAppenedInfo, length_error, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::out_of_range& e)
    {
        printf("Tool_GetDataFromAppenedInfo, out_of_range, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::range_error& e)
    {
        printf("Tool_GetDataFromAppenedInfo, range_error, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::runtime_error& e)
    {
        printf("Tool_GetDataFromAppenedInfo, runtime_error, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::logic_error& e)
    {
        printf("Tool_GetDataFromAppenedInfo, logic_error, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::bad_alloc& e)
    {
        printf("Tool_GetDataFromAppenedInfo, bad_alloc, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::exception& e)
    {
        printf("Tool_GetDataFromAppenedInfo, exception, error msg = %s, error code = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (void*)
    {
        printf("Tool_GetDataFromAppenedInfo,  void* exception, error code = %lu.", GetLastError());
        return false;
    }
    catch (...)
    {
        printf("Tool_GetDataFromAppenedInfo,  unknown exception, error code = %lu.", GetLastError());
        return false;
    }
}

bool GetTextNodeFromXML(const char* XmlInfoBuf, size_t xmlLength, const char* NodeName, char* ValueBuf, size_t bufLength)
{
    if (XmlInfoBuf == NULL || NodeName == NULL || ValueBuf == NULL
        || xmlLength <= 0 || bufLength <= 0)
    {
        return false;
    }
    const char* pNodeName = NodeName;
    std::string strEndNodeName;
    strEndNodeName.append("<");
    strEndNodeName.append("/");
    strEndNodeName.append(pNodeName);
    strEndNodeName.append(">");

    char* pchTemp = new char[xmlLength + 1];
    memset(pchTemp, '\0', xmlLength + 1);
    memcpy(pchTemp, XmlInfoBuf, xmlLength);

    std::string strInfo(pchTemp);
    if (pchTemp)
    {
        delete[] pchTemp;
        pchTemp = NULL;
    }
    else
    {
        printf("malloc failed.");
        return false;
    }

    size_t iPos = strInfo.find(strEndNodeName);
    if (std::string::npos == iPos)
    {
        printf("can not find node %s .\n", strEndNodeName.c_str());
        return false;
    }
    std::string strSubBefore = strInfo.substr(0, iPos);
    size_t iPos2 = strSubBefore.rfind(">");
    if (std::string::npos == iPos)
    {
        printf("can not find text '>' .\n");
        return false;
    }

    std::string strValue = strInfo.substr(iPos2 + 1, iPos - iPos2 - 1);
    //printf("find then node ' %s'  text = '%s'", pNodeName, strValue.c_str());

    if (strValue.length() >= bufLength)
    {
        printf("value length %d is larger than buffer length %d.", strValue.length(), bufLength);
        return false;
    }
    else
    {
        memset(ValueBuf, '\0', bufLength);
        memcpy(ValueBuf, strValue.c_str(), strValue.length());

        return true;
    }
}

bool Tool_InsertElementByName(const char* InputInfo, int iXMLType, const char* pName, const char* nodeName, const char* textValue, std::string& outputString)
{
    //注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
    if (InputInfo == NULL || strlen(InputInfo) <= 0)
    {
        return false;
    }
    TiXmlDocument cXmlDoc;
    TiXmlElement* pRootElement = NULL;
    if (iXMLType == 1)
    {
        if (!cXmlDoc.LoadFile(InputInfo))
        {
            printf("parse XML file failed \n");
            return false;
        }
    }
    else if (iXMLType == 2)
    {
        if (!cXmlDoc.Parse(InputInfo))
        {
            printf("parse XML failed \n");
            return false;
        }
    }

    pRootElement = cXmlDoc.RootElement();
    if (NULL == pRootElement)
    {
        printf("no have root Element\n");
        return false;
    }
    else
    {
        TiXmlElement* pTempElement = NULL;
        pTempElement = Tool_ReadElememt(pRootElement, pName);
        if (pTempElement)
        {
            char szText[256] = { 0 };
            sprintf_s(szText, sizeof(szText), "find the Name : %s, Text = %s\n", pTempElement->Value(), pTempElement->GetText());
            OutputDebugStringA(szText);
            TiXmlElement *pTextEle = new TiXmlElement(nodeName);
            TiXmlText *pTextValue = new TiXmlText(textValue);
            pTextEle->LinkEndChild(pTextValue);
            pTempElement->LinkEndChild(pTextEle);

            TiXmlPrinter Xmlprinter;
            cXmlDoc.Accept(&Xmlprinter);
            outputString = Xmlprinter.CStr();
            return true;
        }
        else
        {
            return false;
        }
    }
}

