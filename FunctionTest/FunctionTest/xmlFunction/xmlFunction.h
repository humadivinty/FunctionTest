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
bool GetPropertyValueDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen);

//************************************
// Method:        GetTextNodeFromXML
// Describe:        获取XML 节点的字符节点内容 如 <PlateName>  无车牌</PlateName> 中的无车牌
// FullName:      GetTextNodeFromXML
// Access:          public 
// Returns:        bool
// Returns Describe: 成功返回 true ， 失败返回 false
// Parameter:    const char * XmlInfoBuf：  传入的XML字符串
// Parameter:    size_t xmlLength ： XML 字符串长度
// Parameter:    const char * NodeName ： 节点名
// Parameter:    char * ValueBuf ：输入输出参数， 值的缓冲区
// Parameter:    size_t bufLength：输入参数， 缓冲区大小
//************************************
bool GetTextNodeFromXML(const char* XmlInfoBuf, size_t xmlLength, const char* NodeName, char* ValueBuf, size_t bufLength);

//************************************
// Method:        Tool_InsertElementByName
// Describe:        向指定XML 的指定节点下，插入text 节点
// FullName:      Tool_InsertElementByName
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    const char * InputInfo ：xml 文件内容
// Parameter:    int iXMLType   ：XML内容的类型，值为1是表示文件路径，值为2时表示文件内容
// Parameter:    const char * pName     ：指定节点的名称
// Parameter:    const char * nodeName:要插入的节点名称
// Parameter:    const char * textValue ：要插入的节点的值
// Parameter:    std::string & outputString ：插入后最终的XML内容
//************************************
bool Tool_InsertElementByName(const char* InputInfo, int iXMLType, const char* pName,
    const char* nodeName, const char* textValue,
    std::string& outputString);

#endif