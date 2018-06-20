#ifndef DIRFUNCTION_H
#define  DIRFUNCTION_H
#include <string>
#include <list>

#ifdef USE_MFC
//删除指定文件夹
//************************************
// Method:        DeleteDirectory
// Describe:        删除指定目录,改函数需在MFC环境下使用
// FullName:      DeleteDirectory
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * strDirName ；要删除的目录名
//************************************
bool DeleteDirectory(char* strDirName);

//删除指定目录中以 'yyyy-mm-dd' 方式命名的文件夹，其中限定的条件为时间早于指定天数
//************************************
// Method:        CirclelaryDelete
// Describe:       循环删除函数，只能删除特定命名的文件夹
// FullName:      CirclelaryDelete
// Access:          public 
// Returns:        int
// Returns Describe:    
// Parameter:    char * folderPath :需要监控的路径
// Parameter:    int iBackUpDays :文件保留的天数
//************************************
int CirclelaryDelete(char* folderPath, int iBackUpDays);

#endif // USE_MFC

//************************************
// Method:        Tool_ReadKeyValueFromConfigFile
// Describe:        从指定路径的ini配置文件中获取节点的值 
// FullName:      Tool_ReadKeyValueFromConfigFile
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * FileName  : ini文件路径
// Parameter:    const char * nodeName  : 节点名称
// Parameter:    const char * keyName  : 属性名称
// Parameter:    char * keyValue            ：需要传入的数据缓冲区
// Parameter:    int bufferSize                 ：说明缓冲区大小
//************************************
void Tool_ReadKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

//************************************
// Method:        g_WriteKeyValueFromConfigFile
// Describe:        将指定节点的属性值写入ini配置文件中
// FullName:      g_WriteKeyValueFromConfigFile
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * FileName : ini文件路径
// Parameter:    const char * nodeName  : 节点名称
// Parameter:    const char * keyName  : 属性名称
// Parameter:    char * keyValue  ：需要传入的数据缓冲区
// Parameter:    int bufferSize  ：说明缓冲区大小
//************************************
void Tool_WriteKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

//************************************
// Method:        Tool_IsFileExist
// Describe:        检查指定文件是否存在
// FullName:      Tool_IsFileExist
// Access:          public 
// Returns:        bool
// Returns Describe: 存在时返回true, 否则返回false
// Parameter:    const char * FilePath  文件的绝对路径
//************************************
bool Tool_IsFileExist(const char* FilePath);


//************************************
// Method:        Tool_MakeDir
// Describe:        创建指定路径, 如D:\\tempDir\\
// FullName:      Tool_MakeDir
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    const char * chImgPath :传入需要创建的路径(注:不检查路径的有效性)
//************************************
bool Tool_MakeDir(const char* chImgPath);

//************************************
// Method:        Tool_GetFileSize
// Describe:        获取指定路径的文件大小
// FullName:      Tool_GetFileSize
// Access:          public 
// Returns:        long 
// Returns Describe: 返回文件占用的硬盘空间大小
// Parameter:    const char * FileName ，文件完整路径
//************************************
size_t Tool_GetFileSize(const char *FileName);

//************************************
// Method:        Tool_ExcuteShellCMD
// Describe:        执行指定的 CMD 命令
// FullName:      Tool_ExcuteShellCMD
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    char * pChCommand
//************************************
void Tool_ExcuteShellCMD(char* pChCommand);

//************************************
// Method:        ExcuteCMD
// Describe:        执行指定的 CMD 命令
// FullName:      ExcuteCMD
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    char * pChCommand
//************************************
void ExcuteCMD(char* pChCommand);

//************************************
// Method:        Tool_SaveFileToDisk
// Describe:        保存数据到指定路径的文件中
// FullName:      Tool_SaveFileToDisk
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * chImgPath      :需要保存的文件路径
// Parameter:    void * pImgData        :需要保存到文件的二进制数据
// Parameter:    DWORD dwImgSize    ：数据长度
//************************************
bool Tool_SaveFileToDisk(char* chImgPath, void* pImgData, size_t iImgSize);

#ifdef WIN32
//************************************
// Method:        GetSoftVersion
// Describe:        获取windows系统下可执行程序的版本信息
// FullName:      GetSoftVersion
// Access:          public 
// Returns:        std::string
// Returns Describe: 可执行程序的版本信息
// Parameter:    const char * exepath  :可执行程序路径
//************************************
std::string Tool_GetSoftVersion(const char* exepath);
#endif

//************************************
// Method:        Tool_checkIP
// Describe:        检查IP是否合法
// FullName:      Tool_checkIP
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    const char * p: 传入的ip地址
//************************************
int Tool_checkIP(const char* p);

//************************************
// Method:        Tool_pingIPaddress
// Describe:        通过ping 命令检查指定ip的网络是否连通
// FullName:      Tool_pingIPaddress
// Access:          public 
// Returns:        bool
// Returns Describe: true表示连通，false表示不连通
// Parameter:    const char * IpAddress :合法的IP地址
//************************************
bool Tool_pingIPaddress(const char* IpAddress);

//************************************
// Method:        Tool_getFiles
// Describe:        获取指定路径下的所有文件名
// FullName:      Tool_getFiles
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const std::string & path  :输入参数，路径名称，为绝对值路径
// Parameter:    std::list<std::string> & files     :输出参数，文件名列表
//************************************
void Tool_getFiles(const std::string& path, std::list<std::string>& files);


//************************************
// Method:        Tool_GetFileInfo
// Describe:        获取指定文件的二进制内容，并返回实际大小
// FullName:      Tool_GetFileInfo
// Access:          public 
// Returns:        bool :获取成功时返回 true, 获取失败时返回 false
// Returns Describe:
// Parameter:    const char * FileName  ：文件名称
// Parameter:    void * infoBuf         ：传入的数据缓冲区
// Parameter:    size_t & bufLength    :输入时为缓冲区大小，输出时为内容实际大小
//************************************
bool Tool_GetFileInfo(const char* FileName, void* infoBuf, size_t& bufLength);
#endif