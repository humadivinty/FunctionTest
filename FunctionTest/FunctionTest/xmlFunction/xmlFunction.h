#ifndef XMLFUNCTION
#define XMLFUNCTION

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include <string>

//************************************
// Method:        ReadElememt
// Describe:       SelectElementByName使用的内部函数， 功能是，在指定节点中，查找并返回指定名称的节点
// FullName:      ReadElememt
// Access:          public 
// Returns:        TiXmlElement*
// Returns Describe:
// Parameter:    TiXmlElement * InputElement
// Parameter:    char * pName
//************************************
TiXmlElement* ReadElememt(TiXmlElement* InputElement, char* pName);

//************************************
// Method:        SelectElementByName
// Describe:       从给定的xml中查找指定名字的节点
// FullName:      SelectElementByName
// Access:          public 
// Returns:        TiXmlElement
// Returns Describe:
// Parameter:    const char * InputInfo ： xml文件的路径
// Parameter:    char * pName   ：需要查找的节点的节点名称
// Parameter:    int iXMLType   ：说明传入 InputInfo的类型，1为文件类型， 2为字符串内容， 字符串需要以‘\0’结尾
//************************************
TiXmlElement SelectElementByName(const char* InputInfo, char* pName, int iXMLType);


//************************************
// Method:        GetDataFromAppenedInfo
// Describe:       设备限定函数，只适用于PCC200的附加信息获取,获取附加信息中，指定节点的属性值
// FullName:      GetDataFromAppenedInfo
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * pszAppendInfo ：pcc200 相机输出的附加信息，该信息为xml格式
// Parameter:    std::string strItemName    ：xml的节点名
// Parameter:    char * pszRstBuf               :数据缓冲区
// Parameter:    int * piRstBufLen              :数据缓冲区长度
//************************************
bool GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen);

#endif