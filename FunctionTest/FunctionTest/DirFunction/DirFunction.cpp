#include "stdafx.h"
#include "DirFunction.h"
#include <string>
#include<shellapi.h>
#ifdef WIN32
#include <io.h>
#pragma comment(lib, "version.lib")  
#endif
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#ifdef USE_MFC
bool DeleteDirectory(char* strDirName)
{
    CFileFind tempFind;

    char strTempFileFind[MAX_PATH];

    sprintf_s(strTempFileFind, sizeof(strTempFileFind), "%s//*.*", strDirName);

    BOOL IsFinded = tempFind.FindFile(strTempFileFind);

    while (IsFinded)
    {
        IsFinded = tempFind.FindNextFile();

        if (!tempFind.IsDots())
        {
            char strFoundFileName[MAX_PATH];

            //strcpy(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));
            strcpy_s(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

            if (tempFind.IsDirectory())
            {
                char strTempDir[MAX_PATH];

                sprintf_s(strTempDir, sizeof(strTempDir), "%s//%s", strDirName, strFoundFileName);

                DeleteDirectory(strTempDir);
            }
            else
            {
                char strTempFileName[MAX_PATH];

                sprintf_s(strTempFileName, sizeof(strTempFileName), "%s//%s", strDirName, strFoundFileName);

                DeleteFile(strTempFileName);
            }
        }
    }

    tempFind.Close();

    if (!RemoveDirectory(strDirName))
    {
        return FALSE;
    }

    return TRUE;
}

int CirclelaryDelete(char* folderPath, int iBackUpDays)
{
    printf("进入环覆盖线程主函数,开始查找制定目录下的文件夹");
    char myPath[MAX_PATH] = { 0 };
    //sprintf(myPath, "%s\\*", folderPath);
    sprintf_s(myPath, sizeof(myPath), "%s\\*", folderPath);

    CTime tmCurrentTime = CTime::GetCurrentTime();
    CTime tmLastMonthTime = tmCurrentTime - CTimeSpan(iBackUpDays, 0, 0, 0);
    int Last_Year = tmLastMonthTime.GetYear();
    int Last_Month = tmLastMonthTime.GetMonth();
    int Last_Day = tmLastMonthTime.GetDay();
    //cout<<Last_Year<<"-"<<Last_Month<<"-"<<Last_Day<<endl;

    CFileFind myFileFind;
    BOOL bFinded = myFileFind.FindFile(myPath);
    char DirectoryName[MAX_PATH] = { 0 };
    while (bFinded)
    {
        bFinded = myFileFind.FindNextFileA();
        if (!myFileFind.IsDots())
        {
            sprintf_s(DirectoryName, sizeof(DirectoryName), "%s", myFileFind.GetFileName().GetBuffer());
            if (myFileFind.IsDirectory())
            {
                int iYear, iMonth, iDay;
                iYear = iMonth = iDay = 0;
                //sscanf(DirectoryName,"%d-%d-%d",&iYear, &iMonth, &iDay);
                sscanf_s(DirectoryName, "%d-%d-%d", &iYear, &iMonth, &iDay);
                if (iYear == 0 && iMonth == 0 && iDay == 0)
                {
                    continue;
                }
                if (iYear < Last_Year)
                {
                    sprintf_s(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                    printf("delete the DirectoryB :%s\n", DirectoryName);
                    DeleteDirectory(DirectoryName);

                    char chLog[MAX_PATH] = { 0 };
                    sprintf_s(chLog, sizeof(chLog), "年份小于当前年份，删除文件夹%s", DirectoryName);
                    printf(chLog);
                }
                else if (iYear == Last_Year)
                {
                    if (iMonth < Last_Month)
                    {
                        sprintf_s(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                        printf("delete the DirectoryB :%s\n", DirectoryName);
                        DeleteDirectory(DirectoryName);

                        char chLog[MAX_PATH] = { 0 };
                        sprintf_s(chLog, sizeof(chLog), "月份小于上一月，删除文件夹%s", DirectoryName);
                        printf(chLog);
                    }
                    else if (iMonth == Last_Month)
                    {
                        if (iDay < Last_Day)
                        {
                            sprintf_s(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                            printf("delete the DirectoryB :%s\n", DirectoryName);
                            DeleteDirectory(DirectoryName);

                            char chLog[MAX_PATH] = { 0 };
                            sprintf_s(chLog, sizeof(chLog), "日号小于指定天数，删除文件夹%s", DirectoryName);
                            printf(chLog);
                        }
                    }
                }
            }
        }
    }
    myFileFind.Close();
    printf("查询结束，退出环覆盖线程主函数..");
    return 0;
}

#endif
void Tool_ReadKeyValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize)
{
    if ((int)strlen(keyValue) > bufferSize)
    {
        return;
    }
    char FileName[MAX_PATH];
    GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);

    PathRemoveFileSpecA(FileName);
    char szFinalIniFileName[MAX_PATH] = { 0 };
    //char iniDeviceInfoName[MAX_PATH] = { 0 };
    sprintf_s(szFinalIniFileName, sizeof(szFinalIniFileName), "%s\\%s", FileName, IniFileName);

    GetPrivateProfileStringA(nodeName, keyName, "0", keyValue, bufferSize, szFinalIniFileName);

    WritePrivateProfileStringA(nodeName, keyName, keyValue, szFinalIniFileName);
}

void Tool_WriteKeyValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize)
{
    if (strlen(keyValue) > (size_t)bufferSize)
    {
        return;
    }
    char szFileName[MAX_PATH];
    GetModuleFileName(NULL, szFileName, MAX_PATH - 1);

    PathRemoveFileSpec(szFileName);
    char szFinalIniFileName[MAX_PATH] = { 0 };
    //char iniDeviceInfoName[MAX_PATH] = { 0 };
    strcat_s(szFinalIniFileName, szFileName);
    strcat_s(szFinalIniFileName, IniFileName);


    //GetPrivateProfileStringA(nodeName, keyName, "172.18.109.97", keyValue, bufferSize, iniFileName);

    WritePrivateProfileStringA(nodeName, keyName, keyValue, szFinalIniFileName);
}

bool Tool_IsFileExist(const char* FilePath)
{
    if (FilePath == NULL)
    {
        return false;
    }
    FILE* tempFile = NULL;
    bool bRet = false;
    //tempFile = fopen(FilePath, "r");
    fopen_s(&tempFile, FilePath, "r");
    if (tempFile)
    {
        bRet = true;
        fclose(tempFile);
        tempFile = NULL;
    }
    return bRet;
}

size_t Tool_GetFileSize(const char *FileName)
{
    //FILE* tmpFile = fopen(FileName, "rb");
    FILE* tmpFile = NULL;
    fopen_s(&tmpFile, FileName, "rb");
    if (tmpFile)
    {
        fseek(tmpFile, 0, SEEK_END);
        long fileSize = ftell(tmpFile);
        fclose(tmpFile);
        tmpFile = NULL;
        return fileSize;
    }
    else
    {
        //"open file failed.";
        return 0;
    }
}

void Tool_ExcuteShellCMD(char* pChCommand)
{
    if (NULL == pChCommand)
    {
        return;
    }
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);
}

void ExcuteCMD(char* pChCommand)
{
#ifdef WIN32

    if (NULL == pChCommand)
    {
        return;
    }
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);

#endif // WIN32
}

bool Tool_SaveFileToDisk(char* chImgPath, void* pImgData, size_t iImgSize)
{
    printf("begin SaveImgToDisk");
    if (NULL == pImgData)
    {
        printf("end1 SaveImgToDisk");
        return false;
    }
    bool bRet = false;
    size_t iWritedSpecialSize = 0;
    if (NULL != strstr(chImgPath, ".\\") && NULL != strstr(chImgPath, "./"))
    {
        std::string tempFile(chImgPath);
        size_t iPosition = tempFile.rfind("\\");
        std::string tempDir = tempFile.substr(0, iPosition + 1);
        if (!MakeSureDirectoryPathExists(tempDir.c_str()))
        {
            char chLogBuff[MAX_PATH] = { 0 };
            //sprintf_s(chLogBuff, "%s save failed", chImgPath);
            sprintf_s(chLogBuff, "path %s create failed.\n", chImgPath);
            printf(chLogBuff);
            return false;
        }
    }

    FILE* fp = NULL;
    //fp = fopen(chImgPath, "wb+");
    fopen_s(&fp, chImgPath, "wb+");
    if (fp)
    {
        //iWritedSpecialSize = fwrite(pImgData, dwImgSize , 1, fp);
        iWritedSpecialSize = fwrite(pImgData, 1, iImgSize, fp);
        fclose(fp);
        fp = NULL;
        bRet = true;
    }
    else
    {
        printf("create file failed.\n");
        bRet = false;
    }
    if (iWritedSpecialSize == iImgSize)
    {
        char chLogBuff[MAX_PATH] = { 0 };
        //sprintf_s(chLogBuff, "%s save success", chImgPath);
        sprintf_s(chLogBuff, "%s save success\n", chImgPath);
        printf(chLogBuff);
    }

    printf("end SaveImgToDisk.\n");
    return bRet;
}

#ifdef WIN32
std::string Tool_GetSoftVersion(const char* exepath)
{
    std::string strVersionInfo = "";
    if (!exepath)
        return strVersionInfo;
    if (_access(exepath, 0) != 0)
        return strVersionInfo;
    UINT infoSize = GetFileVersionInfoSize(exepath, 0);
    if (infoSize != 0) {
        strVersionInfo.resize(infoSize, 0);
        char *pBuf = NULL;
        pBuf = new char[infoSize];
        VS_FIXEDFILEINFO *pVsInfo;
        if (GetFileVersionInfo(exepath, 0, infoSize, pBuf)) {
            if (VerQueryValue(pBuf, "\\", (void **)&pVsInfo, &infoSize))
            {
                //sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
                sprintf_s(pBuf, infoSize, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
                strVersionInfo = pBuf;
            }
        }
        delete[] pBuf;
    }
    return strVersionInfo;
}
#endif

int Tool_checkIP(const char* p)
{
    int n[4];
    char c[4];
    //if (sscanf(p, "%d%c%d%c%d%c%d%c",
    //	&n[0], &c[0], &n[1], &c[1],
    //	&n[2], &c[2], &n[3], &c[3])
    //	== 7)
    if (sscanf_s(p, "%d%c%d%c%d%c%d%c",
        &n[0], &c[0], 1,
        &n[1], &c[1], 1,
        &n[2], &c[2], 1,
        &n[3], &c[3], 1)
        == 7)
    {
        int i;
        for (i = 0; i < 3; ++i)
        if (c[i] != '.')
            return 0;
        for (i = 0; i < 4; ++i)
        if (n[i] > 255 || n[i] < 0)
            return 0;
        if (n[0] == 0 && n[1] == 0 && n[2] == 0 && n[3] == 0)
        {
            return 0;
        }
        return 1;
    }
    else
        return 0;
}

bool Tool_pingIPaddress(const char* IpAddress)
{
    //FILE* pfile;
    //char chBuffer[1024] = {0};
    char chCMD[256] = { 0 };
    sprintf_s(chCMD, "ping %s -n 1", IpAddress);
    //std::string strPingResult;
    //pfile = _popen(chCMD, "r");
    //if (pfile != NULL)
    //{
    //	while(fgets(chBuffer, 1024, pfile) != NULL)
    //	{
    //		strPingResult.append(chBuffer);
    //	}
    //}
    //else
    //{
    //	printf("popen failed. \n");
    //	return false;
    //}
    //_pclose(pfile);
    //printf("%s", strPingResult.c_str());
    //if (std::string::npos != strPingResult.find("TTL") || std::string::npos != strPingResult.find("ttl"))
    //{
    //	return true;
    //}
    //else
    //{
    //	return false;
    //}


    char pbuf[1024]; // 缓存  
    DWORD len;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HANDLE hRead1 = NULL, hWrite1 = NULL;  // 管道读写句柄  
    BOOL b;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE; // 管道句柄是可被继承的  
    saAttr.lpSecurityDescriptor = NULL;

    // 创建匿名管道，管道句柄是可被继承的  
    b = CreatePipe(&hRead1, &hWrite1, &saAttr, 1024);
    if (!b)
    {
        //MessageBox(hwnd, "管道创建失败。","Information",0);  
        printf("管道创建失败\n");
        return false;
    }

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWrite1; // 设置需要传递到子进程的管道写句柄  


    // 创建子进程，运行ping命令，子进程是可继承的  
    if (!CreateProcess(NULL, chCMD, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        //itoa(GetLastError(), pbuf, 10); 
        sprintf_s(pbuf, "%d", GetLastError());
        //MessageBox(hwnd, pbuf,"Information",0);
        printf("%s\n", pbuf);
        CloseHandle(hRead1);
        hRead1 = NULL;
        CloseHandle(hWrite1);
        hWrite1 = NULL;
        return false;
    }

    // 写端句柄已被继承，本地则可关闭，不然读管道时将被阻塞  
    CloseHandle(hWrite1);
    hWrite1 = NULL;

    // 读管道内容，并用消息框显示  
    len = 1000;
    DWORD l;

    std::string strInfo;
    while (ReadFile(hRead1, pbuf, len, &l, NULL))
    {
        if (l == 0) break;
        pbuf[l] = '\0';
        //MessageBox(hwnd, pbuf, "Information",0);  
        //printf("Information2:\n%s\n", pbuf);
        strInfo.append(pbuf);
        len = 1000;
    }

    //MessageBox(hwnd, "ReadFile Exit","Information",0);  
    printf("finish ReadFile buffer = %s\n", strInfo.c_str());
    CloseHandle(hRead1);
    hRead1 = NULL;

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    pi.hThread = NULL;
    CloseHandle(pi.hProcess);
    pi.hProcess = NULL;

    if (std::string::npos != strInfo.find("TTL") || std::string::npos != strInfo.find("ttl"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Tool_getFiles(const std::string& path, std::list<std::string>& files)
{
    long hFile = 0;
    struct _finddata_t fileInfo;
    std::string strPath;
    hFile = _findfirst(strPath.assign(path).append("\\*").c_str(), &fileInfo);
    if (hFile != -1)
    {
        do
        {
            if (fileInfo.attrib & _A_SUBDIR)
            {
                if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
                {
                    Tool_getFiles(strPath.assign(path).append("\\").append(fileInfo.name), files);
                }
            }
            else
            {
                files.push_back(strPath.assign(path).append("\\").append(fileInfo.name));
            }
        } while (_findnext(hFile, &fileInfo) == 0);
        _findclose(hFile);
    }
}

bool Tool_GetFileInfo(const char* FileName, void* infoBuf, size_t& bufLength)
{
    if (!Tool_IsFileExist(FileName))
    {
        printf("文件路径不存在.\n");
        bufLength = 0;
        return false;
    }
    if (infoBuf == NULL || bufLength <= 0)
    {
        printf("参数错误.\n");
        return false;
    }

    size_t iFileSize = Tool_GetFileSize(FileName);
    if (iFileSize > bufLength)
    {
        bufLength = iFileSize;
        printf("传入缓冲区的长度不足.\n");
        return false;
    }
    bufLength = iFileSize;
    FILE* pFile = NULL;
    fopen_s(&pFile, FileName, "rb");
    if (pFile)
    {
        size_t iReadSize = fread(infoBuf, 1, iFileSize, pFile);
        fclose(pFile);
        pFile = NULL;
        if (iReadSize != iFileSize)
        {
            printf("文件读取错误，大小不一致.\n");
            return false;
        }
        else
        {
            return true;
        }
    }
    printf("文件打开失败.\n");
    return false;
}
