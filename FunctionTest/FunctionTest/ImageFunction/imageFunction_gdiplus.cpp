#include "stdafx.h"
#include "imageFunction_gdiplus.h"
#include <stdio.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    Status rStat = GetImageEncoders(num, size, pImageCodecInfo);
    printf("GetImageEncoders status = %d\n", rStat);
    if (rStat == 0)
    {
        for (UINT j = 0; j < num; ++j)
        {
            if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
            {
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return j;  // Success
            }
        }
    }
    else
    {
        printf("GetImageEncoders failed. \n", rStat);
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

int Tool_DrawString_1(const ImgDataStruct dataStruct, const OverlayInfo overlayInfo, UCHAR* destImgBuffer, long& destBufferSize)
{
    if (!dataStruct.srcImgData || dataStruct.srcImgDataLengh <= 0 || !destImgBuffer || destBufferSize <= 0)
    {
        printf("myDrawString, the parameter is invalid, return 1.\n ");
        return 1;
    }
    int iRet = 0;

    UCHAR* pSrcData = dataStruct.srcImgData;
    long srcLength = dataStruct.srcImgDataLengh;

    // 创建流
    IStream *pStreamSrc = NULL;
    // 创建输出流
    IStream* pStreamOut = NULL;
    LARGE_INTEGER liTemp = { 0 };
    ULARGE_INTEGER uLiZero = { 0 };

    HRESULT hr1 = CreateStreamOnHGlobal(NULL, TRUE, &pStreamSrc);
    HRESULT hr2 = CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut);
    if (hr1 != S_OK || hr2 != S_OK)
    {
        printf("CreateStreamOnHGlobal failed, return 2.\n ");
        iRet = 2;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 2;
    }

    // 初始化流
    pStreamSrc->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamSrc->SetSize(uLiZero);

    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamOut->SetSize(uLiZero);

    // 将图像放入流中
    ULONG ulRealSize = 0;
    if (S_OK != pStreamSrc->Write(pSrcData, srcLength, &ulRealSize))
    {
        printf("pStreamSrc failed.\n");
        iRet = 3;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 3;
    }

    // 从流创建位图
    Bitmap bmpSrc(pStreamSrc);
    int iWidth = bmpSrc.GetWidth();
    int iHeight = bmpSrc.GetHeight();

    Gdiplus::Color fontColor(overlayInfo.iColorR, overlayInfo.iColorG, overlayInfo.iColorB);
    Gdiplus::SolidBrush  fontBrush(fontColor);
    Gdiplus::FontFamily  fontFamily(L"Times New Roman");
    Gdiplus::Font        font(&fontFamily, overlayInfo.iFontSize, FontStyleRegular, UnitPixel);

    Gdiplus::RectF  rectfOut;
    {
        //计算消息主题的高度
        Gdiplus::Bitmap bgtest(iWidth, iHeight);
        Gdiplus::Graphics    graphicsTest(&bgtest);
        Gdiplus::RectF rtGdiplus;//计算消息主题的宽度
        rtGdiplus.X = rtGdiplus.Y = 0.0;
        rtGdiplus.Width = (float)iWidth;
        rtGdiplus.Height = -1;
        graphicsTest.MeasureString(overlayInfo.szOverlayString, -1, &font, rtGdiplus, &rectfOut);
        printf("MeasureString width = %f, height = %f\n", rectfOut.Width, rectfOut.Height);
    }

    Gdiplus::Bitmap bmpDst(iWidth, iHeight + (int)(rectfOut.Height));
    Gdiplus::Graphics    graphics(&bmpDst);

    Gdiplus::Status rSata = Gdiplus::Ok;
    Gdiplus::Rect destRect;
    destRect.X = 0;
    destRect.Y = 0;
    destRect.Width = iWidth;
    destRect.Height = bmpDst.GetHeight();
    Gdiplus::SolidBrush myBrush(Gdiplus::Color(255, 255, 0, 0));
    graphics.FillRectangle(&myBrush, destRect);
    destRect.Height = iHeight;
    rSata = graphics.DrawImage(&bmpSrc, destRect);
    if (rSata != Gdiplus::Ok)
    {
        printf("draw image failed.\n");
        iRet = 4;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 4;
    }

    Gdiplus::RectF rectFinal;
    rectFinal.X = 0;
    rectFinal.Y = (float)iHeight;
    rectFinal.Width = rectfOut.Width;
    rectFinal.Height = rectfOut.Height;
#ifdef DEBUG
    graphics.DrawRectangle(&myPen, rectFinal);
#endif

    rSata = graphics.DrawString(overlayInfo.szOverlayString, -1, &font, rectFinal, NULL, &fontBrush);
    if (rSata != Gdiplus::Ok)
    {
        printf("draw string failed.\n");
        iRet = 5;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 5;
    }
    printf("DrawString status = %d\n", rSata);

    CLSID jpgClsid;
    CLSID bmpClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);
    Tool_GetEncoderClsid(L"image/bmp", &bmpClsid);

    // 将位图按照JPG的格式保存到输出流中
    //int iQuality = 80 % 100;
    //EncoderParameters encoderParameters;
    //encoderParameters.Count = 1;
    //encoderParameters.Parameter[0].Guid = EncoderQuality;
    //encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    //encoderParameters.Parameter[0].NumberOfValues = 1;
    //encoderParameters.Parameter[0].Value = &iQuality;
    //bmpDst.Save(pStreamOut, &jpgClsid, &encoderParameters);
    rSata = bmpDst.Save(pStreamOut, &bmpClsid, NULL);
    if (rSata != Gdiplus::Ok)
    {
        printf("save to stream out failed.\n");
        iRet = 6;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 6;
    }

    // 获取输出流大小
    ULARGE_INTEGER libNewPos = { 0 };
    pStreamOut->Seek(liTemp, STREAM_SEEK_END, &libNewPos);      // 将流指针指向结束位置，从而获取流的大小 
    if (destBufferSize < (int)libNewPos.LowPart)                     // 用户分配的缓冲区不足
    {
        destBufferSize = libNewPos.LowPart;
        iRet = 7;
        printf("the buffer size is not enough.\n");

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 7;
    }
    else
    {
        pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);                   // 将流指针指向开始位置
        pStreamOut->Read(destImgBuffer, libNewPos.LowPart, &ulRealSize);           // 将转换后的JPG图片拷贝给用户
        destBufferSize = ulRealSize;
        iRet = 0;
    }

    // 释放内存
    if (pStreamOut != NULL)
    {
        pStreamOut->Release();
        pStreamOut = NULL;
    }

    if (pStreamSrc != NULL)
    {
        pStreamSrc->Release();
        pStreamSrc = NULL;
    }

    return iRet;
}

bool Tool_OverlayStringToImg(unsigned char* pImgsrc, long srcSize, unsigned char* pImgDest, long& DestSize, const wchar_t* DestString, int FontSize, int x, int y, int colorR, int colorG, int colorB, int compressQuality)
{
    if (!pImgsrc || !pImgDest || srcSize <= 0 || DestSize <= 0)
    {
        //printf("传入参数为非法值");
        return false;
    }
    if (wcslen(DestString) <= 0 || x < 0 || y < 0)
    {
        //printf("字符串长度为0");
        return false;
    }

    //构造图像	
    IStream *pSrcStream = NULL;
    IStream *pDestStream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pSrcStream);
    CreateStreamOnHGlobal(NULL, TRUE, &pDestStream);
    if (!pSrcStream || !pDestStream)
    {
        //printf("流创建失败.");
        return false;
    }
    LARGE_INTEGER liTemp = { 0 };
    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pSrcStream->Write(pImgsrc, srcSize, NULL);
    Bitmap bmp(pSrcStream);
    int iImgWith = bmp.GetWidth();
    int iImgHeight = bmp.GetHeight();

    Graphics grp(&bmp);

    SolidBrush brush(Color(colorR, colorG, colorB));
    FontFamily fontFamily(L"宋体");
    //Gdiplus::Font font(&fontFamily, (REAL)FontSize);
    Gdiplus::Font font(&fontFamily, (REAL)FontSize, FontStyleRegular, UnitPixel);

    RectF layoutRect((float)x, (float)y, (float)(iImgWith - x), 0.0);
    RectF FinalRect;
    INT codePointsFitted = 0;
    INT linesFitted = 0;
    int strLenth = wcslen(DestString);
    grp.MeasureString(DestString, strLenth, &font, layoutRect, NULL, &FinalRect, &codePointsFitted, &linesFitted);
    grp.DrawString(DestString, -1, &font, FinalRect, NULL, &brush);
    Gdiplus::Status iState = grp.GetLastStatus();
    if (iState == Ok)
    {
        //printf("字符叠加成功");
    }
    else
    {
        //char chLog[260] = { 0 };
        //sprintf(chLog, "字符叠加失败， 错误码为%d", iState);
        //printf(chLog);
    }

    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);

    // 将位图按照JPG的格式保存到输出流中
    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);
    int iQuality = compressQuality;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmp.Save(pDestStream, &jpgClsid, &encoderParameters);

    ULARGE_INTEGER uiSize;
    pDestStream->Seek(liTemp, STREAM_SEEK_CUR, &uiSize);
    long iFinalSize = (long)uiSize.QuadPart;
    if (iFinalSize <= DestSize)
    {
        pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
        pDestStream->Read(pImgDest, iFinalSize, NULL);
        DestSize = iFinalSize;
    }
    else
    {
        DestSize = 0;
        if (pSrcStream)
        {
            pSrcStream->Release();
            pSrcStream = NULL;
        }
        if (pDestStream)
        {
            pDestStream->Release();
            pDestStream = NULL;
        }
        //printf("传入空间不足，字符叠加失败");
        return false;
    }

    if (pSrcStream)
    {
        pSrcStream->Release();
        pSrcStream = NULL;
    }
    if (pDestStream)
    {
        pDestStream->Release();
        pDestStream = NULL;
    }
    return true;
}

std::wstring Img_string2wstring(std::string strSrc)
{
    std::wstring wstrDst;
    int iWstrLen = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), NULL, 0);
    wchar_t* pwcharBuf = new wchar_t[iWstrLen + sizeof(wchar_t)];   // 多一个结束符
    if (pwcharBuf == NULL || iWstrLen <= 0)
    {
        return L"";
    }
    memset(pwcharBuf, 0, iWstrLen*sizeof(wchar_t)+sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), pwcharBuf, iWstrLen);
    pwcharBuf[iWstrLen] = L'\0';
    wstrDst.append(pwcharBuf);
    delete[] pwcharBuf;
    pwcharBuf = NULL;
    return wstrDst;
}

bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, DWORD *iDstLen, int iDstWidth, int iDstHeight, int compressQuality)
{
    if (pbSrc == NULL || iSrcLen <= 0)
    {
        return false;
    }
    if (pbDst == NULL || iDstLen == NULL || *iDstLen <= 0)
    {
        return false;
    }
    if (iDstWidth <= 0 || iDstHeight <= 0)
    {
        return false;
    }

    // init gdi+
    ULONG_PTR gdiplusToken = NULL;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 创建流
    IStream *pstmp = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pstmp);
    if (pstmp == NULL)
    {
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 初始化流
    LARGE_INTEGER liTemp = { 0 };
    ULARGE_INTEGER uLiZero = { 0 };
    pstmp->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pstmp->SetSize(uLiZero);

    // 将图像放入流中
    ULONG ulRealSize = 0;
    pstmp->Write(pbSrc, iSrcLen, &ulRealSize);

    // 从流创建位图
    Bitmap bmpSrc(pstmp);
    Bitmap bmpDst(iDstWidth, iDstHeight, PixelFormat24bppRGB);

    // 创建画图对象
    Graphics grDraw(&bmpDst);

    // 绘图
    grDraw.DrawImage(&bmpSrc, 0, 0, bmpSrc.GetWidth(), bmpSrc.GetHeight());
    if (Ok != grDraw.GetLastStatus())
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 创建输出流
    IStream* pStreamOut = NULL;
    if (CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut) != S_OK)
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);

    // 初始化输出流
    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamOut->SetSize(uLiZero);

    // 将位图按照JPG的格式保存到输出流中
    int iQuality = compressQuality % 100;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmpDst.Save(pStreamOut, &jpgClsid, &encoderParameters);
    //bmpDst.Save(pStreamOut, &jpgClsid, 0);

    // 获取输出流大小
    bool bRet = false;
    ULARGE_INTEGER libNewPos = { 0 };
    pStreamOut->Seek(liTemp, STREAM_SEEK_END, &libNewPos);      // 将流指针指向结束位置，从而获取流的大小 
    if (*iDstLen < (int)libNewPos.LowPart)                     // 用户分配的缓冲区不足
    {
        *iDstLen = libNewPos.LowPart;
        bRet = false;
    }
    else
    {
        pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);                   // 将流指针指向开始位置
        pStreamOut->Read(pbDst, libNewPos.LowPart, &ulRealSize);           // 将转换后的JPG图片拷贝给用户
        *iDstLen = ulRealSize;
        bRet = true;
    }

    // 释放内存
    if (pstmp != NULL)
    {
        pstmp->Release();
        pstmp = NULL;
    }
    if (pStreamOut != NULL)
    {
        pStreamOut->Release();
        pStreamOut = NULL;
    }

    GdiplusShutdown(gdiplusToken);
    gdiplusToken = NULL;

    return bRet;
}

void Tool_GetRGBDataFromBmpFile(const char* pBmpFile, void* rgbDataBuff, long& bufferLength, int& iwidth, int& iHeight)
{
    BITMAPFILEHEADER bmpHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD paleInfo;

    int iHeadSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD);
    int iDataSize = 0;

    //unsigned char* pImg = new unsigned char[IMG_BUFFER];
    //memset(pImg, 0, IMG_BUFFER);
    FILE* pFile = NULL;
    fopen_s(&pFile, pBmpFile, "rb");
    if (pFile)
    {
        fread(&bmpHeader, 1, sizeof(BITMAPFILEHEADER), pFile);
        fread(&infoHeader, 1, sizeof(BITMAPINFOHEADER), pFile);
        if (infoHeader.biBitCount < 24)
        {
            fread(&paleInfo, 1, sizeof(RGBQUAD), pFile);
        }

        iDataSize = bmpHeader.bfSize - bmpHeader.bfOffBits;

        fread(rgbDataBuff, 1, iDataSize, pFile);

        fclose(pFile);
        pFile = NULL;
    }
    printf("width = %d, height= %d \n", infoHeader.biWidth, infoHeader.biHeight);
    bufferLength = iDataSize;
    iwidth = infoHeader.biWidth;
    iHeight = infoHeader.biHeight;

    UCHAR* pRgbBefore = (unsigned char*)rgbDataBuff;
    unsigned char* pRGB = new unsigned char[bufferLength];

    int ibytePerLine = ((iwidth * 24 + 31) >> 5) << 2; // 图像每行字节对齐

    for (int i = iHeight - 1, j = 0; i >= 0; i--, j++)// 调整顺序
    {
        //        memcpy(RGB + j * iWidth*3, RGB+ iWidth * iHeight * 3 + i * iWidth*3, iWidth*3);
        //memcpy(pRGB + j * ibytePerLine, pRGB + ibytePerLine * iHeight + i * ibytePerLine, ibytePerLine);
        memcpy(pRGB + j * ibytePerLine, pRgbBefore + i * ibytePerLine, ibytePerLine);
    }

    // 顺序调整
    unsigned char temp;
    for (int i = 0; i < iwidth * iHeight * 3; i += 3)
    {
        temp = pRGB[i];
        pRGB[i] = pRGB[i + 2];
        pRGB[i + 2] = temp;
    }
    memcpy(pRgbBefore, pRGB, bufferLength);
    if (pRGB)
    {
        delete[] pRGB;
        pRGB = NULL;
    }
}

void Tool_ConvertRGB2YUV422(unsigned char* RGB, unsigned char* YUV, int iWidth, int iHeight)
{
    if (RGB == NULL || NULL == YUV) {
        return;
    }

    // 转换矩阵
#define MY(a,b,c) (( a*  0.2989  + b*  0.5866  + c*  0.1145))
#define MU(a,b,c) (( a*(-0.1688) + b*(-0.3312) + c*  0.5000 + 128))
#define MV(a,b,c) (( a*  0.5000  + b*(-0.4184) + c*(-0.0816) + 128))
    // 大小判断
#define DY(a,b,c) (MY(a,b,c) > 255 ? 255 : (MY(a,b,c) < 0 ? 0 : MY(a,b,c)))
#define DU(a,b,c) (MU(a,b,c) > 255 ? 255 : (MU(a,b,c) < 0 ? 0 : MU(a,b,c)))
#define DV(a,b,c) (MV(a,b,c) > 255 ? 255 : (MV(a,b,c) < 0 ? 0 : MV(a,b,c)))

    // 变量声明
    int i, x, y, j = 0;
    unsigned char *Y = NULL;
    unsigned char *U = NULL;
    unsigned char *V = NULL;

    int bytePerLine = ((iWidth * 24 + 31) >> 5) << 2; // 图像每行字节对齐

    Y = YUV;
    U = YUV + iWidth * iHeight;
    V = U + ((iWidth * iHeight) >> 2);

    for (y = 0; y < iHeight; y++)
    {
        for (x = 0; x < iWidth; x++)
        {
            j = y * iWidth + x;
            i = j * 3;
            Y[j] = (unsigned char)(DY(RGB[i], RGB[i + 1], RGB[i + 2]));

            if (x % 2 == 1 && y % 2 == 1)
            {
                j = (iWidth >> 1) * (y >> 1) + (x >> 1);
                //上面i仍有效
                U[j] = (unsigned char)
                    ((DU(RGB[i], RGB[i + 1], RGB[i + 2]) +
                    DU(RGB[i - 3], RGB[i - 2], RGB[i - 1]) +
                    DU(RGB[i - bytePerLine], RGB[i + 1 - bytePerLine], RGB[i + 2 - bytePerLine]) +
                    DU(RGB[i - 3 - bytePerLine], RGB[i - 2 - bytePerLine], RGB[i - 1 - bytePerLine])) / 4);

                V[j] = (unsigned char)
                    ((DV(RGB[i], RGB[i + 1], RGB[i + 2]) +
                    DV(RGB[i - 3], RGB[i - 2], RGB[i - 1]) +
                    DV(RGB[i - bytePerLine], RGB[i + 1 - bytePerLine], RGB[i + 2 - bytePerLine]) +
                    DV(RGB[i - 3 - bytePerLine], RGB[i - 2 - bytePerLine], RGB[i - 1 - bytePerLine])) / 4);
            }
        }
    }
}

void Tool_ConverBmpFile2Yuv422(const char* pBmpFile, void* yuvBuff, long& bufferLength, int& Width, int& Height)
{
    long iBufferLength = IMG_BUFFER;
    int iWidth = 0, iHeight = 0;
    unsigned char* pRgbBuf = new unsigned char[iBufferLength];
    unsigned char* pyuvBuf = (unsigned char*)yuvBuff;
    memset(pRgbBuf, 0, iBufferLength);
    Tool_GetRGBDataFromBmpFile(pBmpFile, pRgbBuf, iBufferLength, iWidth, iHeight);
    iWidth = iWidth > 0 ? iWidth : -iWidth;
    iHeight = iHeight > 0 ? iHeight : -iHeight;

    Tool_ConvertRGB2YUV422(pRgbBuf, pyuvBuf, iWidth, iHeight);

    int ibytePerLine = ((iWidth * 24 + 31) >> 5) << 2;
    long iyuvLength = ibytePerLine * iHeight;
    iyuvLength = iyuvLength > 0 ? iyuvLength : (-iyuvLength);

    bufferLength = iyuvLength;
    Width = iWidth;
    Height = iHeight;

    if (pRgbBuf)
    {
        delete[] pRgbBuf;
        pRgbBuf = NULL;
    }
}

const int BIN_BIT_COUNT = 8;
const int BIN_WIDTH = 112;
const int BIN_HEIGHT = 20;
const int BIN_STRIDE = BIN_WIDTH / BIN_BIT_COUNT;
const int BIN_BYTE_COUNT = BIN_HEIGHT * BIN_STRIDE;
void Tool_RgbBin2ByteBin(PBYTE m_rgbBinImage, PBYTE pbByteBinImage)
{
    for (int y = 0; y < BIN_HEIGHT; y++)
    {
        BYTE *pSrc = &m_rgbBinImage[(BIN_HEIGHT - 1) * BIN_STRIDE];
        BYTE *pDest = pbByteBinImage;
        for (int y = 0; y < BIN_HEIGHT; y++)
        {
            memcpy(pDest, pSrc, BIN_STRIDE);
            pSrc -= BIN_STRIDE;
            pDest += BIN_STRIDE;
        }
        //字节中前后bit位互换
        for (int i = 0; i < BIN_BYTE_COUNT; i++)
        {
            BYTE bTemp = pbByteBinImage[i];
            BYTE bTemp2 = 0;
            bTemp2 |= (bTemp & 0x80) >> 7;
            bTemp2 |= (bTemp & 0x40) >> 5;
            bTemp2 |= (bTemp & 0x20) >> 3;
            bTemp2 |= (bTemp & 0x10) >> 1;
            bTemp2 |= (bTemp & 0x08) << 1;
            bTemp2 |= (bTemp & 0x04) << 3;
            bTemp2 |= (bTemp & 0x02) << 5;
            bTemp2 |= (bTemp & 0x01) << 7;
            pbByteBinImage[i] = bTemp2;
        }
    }
}

BOOL Tool_BinImage2BitmapData(int iCX, int iCY, BYTE* pbByteBinImage, char* pbBitmapData, int* piBitmapDataLen)
{
    BOOL bSuccess = TRUE;
    int iByteBinImageLen = BIN_BYTE_COUNT;
    if (iCX != BIN_WIDTH || iCY != BIN_HEIGHT || pbByteBinImage == NULL || piBitmapDataLen == NULL)
    {
        bSuccess = FALSE;
    }
    else
    {
        int iStride = ((BIN_STRIDE + 3) >> 2) << 2;
        int iBmpDataSize = iStride * BIN_HEIGHT;

        BITMAPFILEHEADER bmfHdr;
        bmfHdr.bfType = 0x4d42;
        bmfHdr.bfSize = iBmpDataSize + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);
        bmfHdr.bfReserved1 = 0;
        bmfHdr.bfReserved2 = 0;
        bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);

        BITMAPINFO *pInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD));
        pInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        pInfo->bmiHeader.biWidth = BIN_WIDTH;
        pInfo->bmiHeader.biHeight = BIN_HEIGHT;
        pInfo->bmiHeader.biPlanes = 1;
        pInfo->bmiHeader.biBitCount = 1;
        pInfo->bmiHeader.biCompression = BI_RGB;
        pInfo->bmiHeader.biSizeImage = 0;
        pInfo->bmiHeader.biXPelsPerMeter = 0;
        pInfo->bmiHeader.biYPelsPerMeter = 0;
        pInfo->bmiHeader.biClrUsed = 0;
        pInfo->bmiHeader.biClrImportant = 0;

        pInfo->bmiColors[0].rgbBlue = 0;
        pInfo->bmiColors[0].rgbGreen = 0;
        pInfo->bmiColors[0].rgbRed = 0;
        pInfo->bmiColors[0].rgbReserved = 0;
        pInfo->bmiColors[1].rgbBlue = 255;
        pInfo->bmiColors[1].rgbGreen = 255;
        pInfo->bmiColors[1].rgbRed = 255;
        pInfo->bmiColors[1].rgbReserved = 0;

        BYTE *pBmpData = (BYTE*)malloc(iBmpDataSize);
        memset(pBmpData, 0, iBmpDataSize);
        BYTE *pSrc = pbByteBinImage;
        BYTE *pDest = pBmpData;
        for (int i = 0; i < BIN_HEIGHT; i++)
        {
            memcpy(pDest, pSrc, BIN_STRIDE);
            pDest += iStride;
            pSrc += BIN_STRIDE;
        }

        memcpy(pbBitmapData, &bmfHdr, sizeof(BITMAPFILEHEADER));
        pbBitmapData += sizeof(BITMAPFILEHEADER);
        *piBitmapDataLen += sizeof(BITMAPFILEHEADER);

        memcpy(pbBitmapData, pInfo, sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD));
        pbBitmapData += sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);
        *piBitmapDataLen += sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);

        memcpy(pbBitmapData, pBmpData, iBmpDataSize);
        pbBitmapData += iBmpDataSize;
        *piBitmapDataLen += iBmpDataSize;

        free(pBmpData);
        free(pInfo);
    }

    return bSuccess;
}

void Tool_Bin2BMP(PBYTE pbBinData, PBYTE pbBmpData, INT& nBmpLen)
{
    PBYTE pbByteBinImage[BIN_BYTE_COUNT] = { 0 };
    Tool_RgbBin2ByteBin(pbBinData, (PBYTE)pbByteBinImage);

    int iBitmapDataLen = 0;
    Tool_BinImage2BitmapData(BIN_WIDTH, BIN_HEIGHT, (PBYTE)pbByteBinImage, (char*)pbBmpData, &iBitmapDataLen);
    nBmpLen = iBitmapDataLen;
}


static inline int SetInt32ToByteArray(BYTE *pbDest, INT32 i32Src)
{
    memcpy(pbDest, &i32Src, sizeof(i32Src));
    return sizeof(i32Src);
}
static inline int SetInt16ToByteArray(BYTE *pbDest, INT16 i16Src)
{
    memcpy(pbDest, &i16Src, sizeof(i16Src));
    return sizeof(i16Src);
}
bool Tool_Yuv4222BmpFile(BYTE* pbDest, int iDestBufLen, int* piDestLen, BYTE* pbSrc, int iSrcWidth, int iSrcHeight)
{
    DWORD32 dwOffSet = 54;
    DWORD32 dwRBGStride = ((iSrcWidth * 24 + 31) >> 5) << 2;
    DWORD32 dwRgbSize = dwRBGStride * iSrcHeight;
    *piDestLen = dwOffSet + dwRgbSize;
    if (*piDestLen > iDestBufLen)
    {
        *piDestLen = 0;
        return false;
    }

    *(pbDest++) = 'B';										//BMP文件标志.
    *(pbDest++) = 'M';
    pbDest += SetInt32ToByteArray(pbDest, *piDestLen);		//BMP文件大小.
    pbDest += SetInt32ToByteArray(pbDest, 0);				//reserved
    pbDest += SetInt32ToByteArray(pbDest, dwOffSet);		//文件头偏移量.
    pbDest += SetInt32ToByteArray(pbDest, 0x28);			//biSize.
    pbDest += SetInt32ToByteArray(pbDest, iSrcWidth);		//biWidth.
    pbDest += SetInt32ToByteArray(pbDest, iSrcHeight);		//biHeight.
    pbDest += SetInt16ToByteArray(pbDest, 1);				//biPlanes.
    pbDest += SetInt16ToByteArray(pbDest, 24);				//biBitCount.
    pbDest += SetInt32ToByteArray(pbDest, 0);				//biCompression.
    pbDest += SetInt32ToByteArray(pbDest, dwRgbSize);		//biSizeImage.
    pbDest += SetInt32ToByteArray(pbDest, 0);				//biXPerMeter.
    pbDest += SetInt32ToByteArray(pbDest, 0);				//biYPerMeter.
    pbDest += SetInt32ToByteArray(pbDest, 0);				//biClrUsed.
    pbDest += SetInt32ToByteArray(pbDest, 0);				//biClrImportant.
    Tool_Yuv422ToRgb(pbDest, pbSrc, iSrcWidth, iSrcHeight, dwRBGStride);
    return true;
}

bool Tool_Yuv422ToRgb(BYTE *pbDest, BYTE *pbSrc, int iSrcWidth, int iSrcHeight, int iBGRStride)
{
    bool fBottomUp = true;
    if (iSrcHeight < 0)
    {
        iSrcHeight = -iSrcHeight;
        fBottomUp = false;
    }
    int x, y;
    unsigned char *pY = reinterpret_cast<unsigned char*>(pbSrc);
    unsigned char *pCb = reinterpret_cast<unsigned char*>(pbSrc)+iSrcWidth * iSrcHeight;
    unsigned char *pCr = reinterpret_cast<unsigned char*>(pbSrc)+iSrcWidth * iSrcHeight + (iSrcWidth >> 1) * iSrcHeight;
    for (y = 0; y < iSrcHeight; y++)
    for (x = 0; x < iSrcWidth; x++)
    {
        int iY = *(pY + y * iSrcWidth + x);
        int iCb = *(pCb + y * (iSrcWidth >> 1) + (x >> 1));
        int iCr = *(pCr + y * (iSrcWidth >> 1) + (x >> 1));
        int iR = static_cast<int>(1.402 * (iCr - 128) + iY);
        int iG = static_cast<int>(-0.34414 * (iCb - 128) - 0.71414 * (iCr - 128) + iY);
        int iB = static_cast<int>(1.772 * (iCb - 128) + iY);
        if (iR > 255)
            iR = 255;
        if (iR < 0)
            iR = 0;
        if (iG > 255)
            iG = 255;
        if (iG < 0)
            iG = 0;
        if (iB > 255)
            iB = 255;
        if (iB < 0)
            iB = 0;
        if (fBottomUp)
        {
            pbDest[(iSrcHeight - y - 1) * iBGRStride + x * 3] = iB;
            pbDest[(iSrcHeight - y - 1) * iBGRStride + x * 3 + 1] = iG;
            pbDest[(iSrcHeight - y - 1) * iBGRStride + x * 3 + 2] = iR;
        }
        else
        {
            pbDest[y * iBGRStride + x * 3] = iB;
            pbDest[y * iBGRStride + x * 3 + 1] = iG;
            pbDest[y * iBGRStride + x * 3 + 2] = iR;
        }
    }
    return true;
}
