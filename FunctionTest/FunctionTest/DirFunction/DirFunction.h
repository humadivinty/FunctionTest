#ifndef DIRFUNCTION_H
#define  DIRFUNCTION_H
#include <string>
#include <list>

#ifdef USE_MFC
//ɾ��ָ���ļ���
//************************************
// Method:        DeleteDirectory
// Describe:        ɾ��ָ��Ŀ¼,�ĺ�������MFC������ʹ��
// FullName:      DeleteDirectory
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * strDirName ��Ҫɾ����Ŀ¼��
//************************************
bool DeleteDirectory(char* strDirName);

//ɾ��ָ��Ŀ¼���� 'yyyy-mm-dd' ��ʽ�������ļ��У������޶�������Ϊʱ������ָ������
//************************************
// Method:        CirclelaryDelete
// Describe:       ѭ��ɾ��������ֻ��ɾ���ض��������ļ���
// FullName:      CirclelaryDelete
// Access:          public 
// Returns:        int
// Returns Describe:    
// Parameter:    char * folderPath :��Ҫ��ص�·��
// Parameter:    int iBackUpDays :�ļ�����������
//************************************
int CirclelaryDelete(char* folderPath, int iBackUpDays);

#endif // USE_MFC

//************************************
// Method:        Tool_ReadKeyValueFromConfigFile
// Describe:        ��ָ��·����ini�����ļ��л�ȡ�ڵ��ֵ 
// FullName:      Tool_ReadKeyValueFromConfigFile
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * FileName  : ini�ļ�·��
// Parameter:    const char * nodeName  : �ڵ�����
// Parameter:    const char * keyName  : ��������
// Parameter:    char * keyValue            ����Ҫ��������ݻ�����
// Parameter:    int bufferSize                 ��˵����������С
//************************************
void Tool_ReadKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

//************************************
// Method:        g_WriteKeyValueFromConfigFile
// Describe:        ��ָ���ڵ������ֵд��ini�����ļ���
// FullName:      g_WriteKeyValueFromConfigFile
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * FileName : ini�ļ�·��
// Parameter:    const char * nodeName  : �ڵ�����
// Parameter:    const char * keyName  : ��������
// Parameter:    char * keyValue  ����Ҫ��������ݻ�����
// Parameter:    int bufferSize  ��˵����������С
//************************************
void Tool_WriteKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

//************************************
// Method:        Tool_IsFileExist
// Describe:        ���ָ���ļ��Ƿ����
// FullName:      Tool_IsFileExist
// Access:          public 
// Returns:        bool
// Returns Describe: ����ʱ����true, ���򷵻�false
// Parameter:    const char * FilePath  �ļ��ľ���·��
//************************************
bool Tool_IsFileExist(const char* FilePath);


//************************************
// Method:        Tool_MakeDir
// Describe:        ����ָ��·��, ��D:\\tempDir\\
// FullName:      Tool_MakeDir
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    const char * chImgPath :������Ҫ������·��(ע:�����·������Ч��)
//************************************
bool Tool_MakeDir(const char* chImgPath);

//************************************
// Method:        Tool_GetFileSize
// Describe:        ��ȡָ��·�����ļ���С
// FullName:      Tool_GetFileSize
// Access:          public 
// Returns:        long 
// Returns Describe: �����ļ�ռ�õ�Ӳ�̿ռ��С
// Parameter:    const char * FileName ���ļ�����·��
//************************************
size_t Tool_GetFileSize(const char *FileName);

//************************************
// Method:        Tool_ExcuteShellCMD
// Describe:        ִ��ָ���� CMD ����
// FullName:      Tool_ExcuteShellCMD
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    char * pChCommand
//************************************
void Tool_ExcuteShellCMD(char* pChCommand);

//************************************
// Method:        ExcuteCMD
// Describe:        ִ��ָ���� CMD ����
// FullName:      ExcuteCMD
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    char * pChCommand
//************************************
void ExcuteCMD(char* pChCommand);

//************************************
// Method:        Tool_ExcuteCMDbyCreateProcess
// Describe:        ͨ�������ӽ�����ִ�� cmd ����
// FullName:      Tool_ExcuteCMDbyCreateProcess
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    const char * CmdName: ������������ע���� ���ͷҪ���� /c
//************************************
bool Tool_ExcuteCMDbyCreateProcess(const char* CmdName);

//************************************
// Method:        Tool_DeleteFileByCMD
// Describe:        ͨ������ɾ��ָ��·���µ��ļ�
// FullName:      Tool_DeleteFileByCMD
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * chFileName
//************************************
void Tool_DeleteFileByCMD(const char* chFileName);

//************************************
// Method:        Tool_SaveFileToDisk
// Describe:        �������ݵ�ָ��·�����ļ���
// FullName:      Tool_SaveFileToDisk
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * chImgPath      :��Ҫ������ļ�·��
// Parameter:    void * pImgData        :��Ҫ���浽�ļ��Ķ���������
// Parameter:    DWORD dwImgSize    �����ݳ���
//************************************
bool Tool_SaveFileToDisk(char* chImgPath, void* pImgData, size_t iImgSize);

#ifdef WIN32
//************************************
// Method:        GetSoftVersion
// Describe:        ��ȡwindowsϵͳ�¿�ִ�г���İ汾��Ϣ
// FullName:      GetSoftVersion
// Access:          public 
// Returns:        std::string
// Returns Describe: ��ִ�г���İ汾��Ϣ
// Parameter:    const char * exepath  :��ִ�г���·��
//************************************
std::string Tool_GetSoftVersion(const char* exepath);
#endif

//************************************
// Method:        Tool_checkIP
// Describe:        ���IP�Ƿ�Ϸ�
// FullName:      Tool_checkIP
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    const char * p: �����ip��ַ
//************************************
int Tool_checkIP(const char* p);

//************************************
// Method:        Tool_pingIPaddress
// Describe:        ͨ��ping ������ָ��ip�������Ƿ���ͨ
// FullName:      Tool_pingIPaddress
// Access:          public 
// Returns:        bool
// Returns Describe: true��ʾ��ͨ��false��ʾ����ͨ
// Parameter:    const char * IpAddress :�Ϸ���IP��ַ
//************************************
bool Tool_pingIPaddress(const char* IpAddress);

//************************************
// Method:        Tool_getFiles
// Describe:        ��ȡָ��·���µ�ָ����׺�������ļ�·��
// FullName:      Tool_getFiles
// Access:          public 
// Returns:        std::list<std::string> �� �ļ�·���б�
// Returns Describe:
// Parameter:    const std::string & path  :���������·�����ƣ�Ϊ����ֵ·��
// Parameter:     const std::string& filter     :�����������׺�� ��: *.dat, *.xml��
//************************************
std::list<std::string> Tool_getFilesPath(const std::string& cate_dir, const std::string& filter);


//************************************
// Method:        Tool_GetFileInfo
// Describe:        ��ȡָ���ļ��Ķ��������ݣ�������ʵ�ʴ�С
// FullName:      Tool_GetFileInfo
// Access:          public 
// Returns:        bool :��ȡ�ɹ�ʱ���� true, ��ȡʧ��ʱ���� false
// Returns Describe:
// Parameter:    const char * FileName  ���ļ�����
// Parameter:    void * infoBuf         ����������ݻ�����
// Parameter:    size_t & bufLength    :����ʱΪ��������С�����ʱΪ����ʵ�ʴ�С
//************************************
bool Tool_LoadFileDataToBuffer(const char* FileName, void* infoBuf, size_t& bufLength);

//************************************
// Method:        Tool_GetCurrentTime
// Describe:        ��ȡ����ʱ�䣬ʱ�侫ȷ������
// FullName:      Tool_GetCurrentTime
// Access:          public 
// Returns:        SYSTEMTIME
// Returns Describe:
//************************************
SYSTEMTIME Tool_GetCurrentTime();

//************************************
// Method:        Tool_GetCurrentPath
// Describe:        ��ȡ��ǰ��������Ŀ¼
// FullName:      Tool_GetCurrentPath
// Access:          public 
// Returns:        const TCHAR*
// Returns Describe:
//************************************
const TCHAR* Tool_GetCurrentPath();

//************************************
// Method:        Tool_SafeCloseThread
// Describe:        ��ȫ�ر��߳̾��,��ֹ�ȴ��ڼ���߳�����
// FullName:      Tool_SafeCloseThread
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    HANDLE & threadHandle
//************************************
int Tool_SafeCloseThread(HANDLE& threadHandle);

#endif