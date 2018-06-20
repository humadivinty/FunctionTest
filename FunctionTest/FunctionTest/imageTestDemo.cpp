#include "stdafx.h"
#include "imageTestDemo.h"
#include "ImageFunction/imageFunction_gdiplus.h"
#include "DirFunction/DirFunction.h"


void test_Tool_DrawString_1_Demo()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    char* szImgName = "./1.jpg";
    unsigned char* pSrcBuf = new unsigned char[MAX_IMG_BUF];
    size_t iSrcBufLength = MAX_IMG_BUF;
    memset(pSrcBuf, 0, iSrcBufLength);
    if (!Tool_GetFileInfo(szImgName, pSrcBuf, iSrcBufLength))
    {
        printf("获取文件内容失败.\n");
        return ;
    }
    unsigned char* pDestBuf = new unsigned char[MAX_IMG_BUF];
    size_t iDestBufLength = MAX_IMG_BUF;
    memset(pDestBuf, 0, iDestBufLength);

    ImgDataStruct dataStruct;
    dataStruct.srcImgData = pSrcBuf;
    dataStruct.srcImgDataLengh = iSrcBufLength;

    OverlayInfo overlayInfo1;
    overlayInfo1.st_backgroundColor.iColorAlpha = 255;
    overlayInfo1.st_backgroundColor.iColorR = 0;
    overlayInfo1.st_backgroundColor.iColorG = 0;
    overlayInfo1.st_backgroundColor.iColorB = 128;

    overlayInfo1.iFontSize = 32;
    overlayInfo1.st_fontColor.iColorAlpha = 128;
    overlayInfo1.st_fontColor.iColorR = 0;
    overlayInfo1.st_fontColor.iColorG = 255;
    overlayInfo1.st_fontColor.iColorB = 0;

    overlayInfo1.st_FontPosition.iPosX = 0;
    overlayInfo1.st_FontPosition.iPosY = 0;

    overlayInfo1.szOverlayString = L"heasdfadlfjadlf;\tjasdlfjasdfl;\nkjasdf";

    //Tool_SaveFileToDisk("./dest1.bmp", pSrcBuf, iSrcBufLength);
    int iRet = Tool_DrawString_1(dataStruct, overlayInfo1, pDestBuf, iDestBufLength);
    if (iRet == 0)
    {
        Tool_SaveFileToDisk("./dest.bmp", pDestBuf, iDestBufLength);
    }
    else
    {
        printf("Tool_DrawString_1 failed, error code = %d\n", iRet);
    }

    SAFE_DELETT_ARRAY(pSrcBuf);
    SAFE_DELETT_ARRAY(pDestBuf);

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return;
}
