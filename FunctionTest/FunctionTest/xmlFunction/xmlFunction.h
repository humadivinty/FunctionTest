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
bool GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen);

#endif