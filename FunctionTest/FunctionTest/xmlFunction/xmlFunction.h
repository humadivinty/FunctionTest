#ifndef XMLFUNCTION
#define XMLFUNCTION

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include <string>

//************************************
// Method:        ReadElememt
// Describe:       SelectElementByNameʹ�õ��ڲ������� �����ǣ���ָ���ڵ��У����Ҳ�����ָ�����ƵĽڵ�
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
// Describe:       �Ӹ�����xml�в���ָ�����ֵĽڵ�
// FullName:      SelectElementByName
// Access:          public 
// Returns:        TiXmlElement
// Returns Describe:
// Parameter:    const char * InputInfo �� xml�ļ���·��
// Parameter:    char * pName   ����Ҫ���ҵĽڵ�Ľڵ�����
// Parameter:    int iXMLType   ��˵������ InputInfo�����ͣ�1Ϊ�ļ����ͣ� 2Ϊ�ַ������ݣ� �ַ�����Ҫ�ԡ�\0����β
//************************************
TiXmlElement SelectElementByName(const char* InputInfo, char* pName, int iXMLType);


//************************************
// Method:        GetDataFromAppenedInfo
// Describe:       �豸�޶�������ֻ������PCC200�ĸ�����Ϣ��ȡ,��ȡ������Ϣ�У�ָ���ڵ������ֵ
// FullName:      GetDataFromAppenedInfo
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * pszAppendInfo ��pcc200 �������ĸ�����Ϣ������ϢΪxml��ʽ
// Parameter:    std::string strItemName    ��xml�Ľڵ���
// Parameter:    char * pszRstBuf               :���ݻ�����
// Parameter:    int * piRstBufLen              :���ݻ���������
//************************************
bool GetPropertyValueDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen);

//************************************
// Method:        GetTextNodeFromXML
// Describe:        ��ȡXML �ڵ���ַ��ڵ����� �� <PlateName>  �޳���</PlateName> �е��޳���
// FullName:      GetTextNodeFromXML
// Access:          public 
// Returns:        bool
// Returns Describe: �ɹ����� true �� ʧ�ܷ��� false
// Parameter:    const char * XmlInfoBuf��  �����XML�ַ���
// Parameter:    size_t xmlLength �� XML �ַ�������
// Parameter:    const char * NodeName �� �ڵ���
// Parameter:    char * ValueBuf ��������������� ֵ�Ļ�����
// Parameter:    size_t bufLength����������� ��������С
//************************************
bool GetTextNodeFromXML(const char* XmlInfoBuf, size_t xmlLength, const char* NodeName, char* ValueBuf, size_t bufLength);

//************************************
// Method:        Tool_InsertElementByName
// Describe:        ��ָ��XML ��ָ���ڵ��£�����text �ڵ�
// FullName:      Tool_InsertElementByName
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    const char * InputInfo ��xml �ļ�����
// Parameter:    int iXMLType   ��XML���ݵ����ͣ�ֵΪ1�Ǳ�ʾ�ļ�·����ֵΪ2ʱ��ʾ�ļ�����
// Parameter:    const char * pName     ��ָ���ڵ������
// Parameter:    const char * nodeName:Ҫ����Ľڵ�����
// Parameter:    const char * textValue ��Ҫ����Ľڵ��ֵ
// Parameter:    std::string & outputString ����������յ�XML����
//************************************
bool Tool_InsertElementByName(const char* InputInfo, int iXMLType, const char* pName,
    const char* nodeName, const char* textValue,
    std::string& outputString);

#endif