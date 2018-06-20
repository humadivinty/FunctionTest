#ifndef IMGFUNCTION_GDIPLUS
#define IMGFUNCTION_GDIPLUS

#include<windows.h>
#include <string>
#include <gdiplus.h>

//BMP文件组成:
/*
位图文件头BitMapFileHeader
位图信息头BitMapInfoHeader
调色板palette
实际的位图数据imageData
*/

//位图文件头主要包括：
//typedef struct tagBITMAPFILEHEADER 
//{
//    WORD bfType; /* 说明文件的类型 */
//    DWORD bfSize; /* 说明文件的大小，用字节为单位 */
//    WORD bfReserved1; /* 保留，设置为0 */
//    WORD bfReserved2; /* 保留，设置为0 */
//    DWORD bfOffBits; /* 说明从BITMAPFILEHEADER结构开始到实际的图像数据之间的字节偏移量 */
//} BITMAPFILEHEADER;
//
//
////位图信息头主要包括：
//typedef struct tagBITMAPINFOHEADER {
//    DWORD biSize; /* 说明结构体所需字节数 */
//    LONG biWidth; /* 以像素为单位说明图像的宽度 */
//    LONG biHeight; /* 以像素为单位说明图像的高速 */
//    WORD biPlanes; /* 说明位面数，必须为1 */
//    WORD biBitCount; /* 说明位数/像素，1、2、4、8、24 */
//    DWORD biCompression; /* 说明图像是否压缩及压缩类型BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS */
//    DWORD biSizeImage; /* 以字节为单位说明图像大小，必须是4的整数倍*/
//    LONG biXPelsPerMeter; /*目标设备的水平分辨率，像素/米 */
//    LONG biYPelsPerMeter; /*目标设备的垂直分辨率，像素/米 */
//    DWORD biClrUsed; /* 说明图像实际用到的颜色数，如果为0，则颜色数为2的biBitCount次方 */
//    DWORD biClrImportant; /*说明对图像显示有重要影响的颜色索引的数目，如果是0，表示都重要。*/
//} BITMAPINFOHEADER;
//
////调色板实际上是一个数组，它所包含的元素与位图所具有的颜色数相同，
////决定于biClrUsed和biBitCount字段。数组中每个元素的类型是一个RGBQUAD结构。真彩色无调色板部分。
//typedef struct tagRGBQUAD {
//    BYTE rgbBlue; /*指定蓝色分量*/
//    BYTE rgbGreen; /*指定绿色分量*/
//    BYTE rgbRed; /*指定红色分量*/
//    BYTE rgbReserved; /*保留，指定为0*/
//} RGBQUAD;

#define  IMG_BUFFER (10*1024*1024)

typedef struct _ImgDataStruct{
    UCHAR* srcImgData;
    long srcImgDataLengh;
    _ImgDataStruct() :
        srcImgData(NULL),
        srcImgDataLengh(0)
    {
    }

    ~_ImgDataStruct()
    {
        //if (srcImgData)
        //{
        //    delete[] srcImgData;
        //    srcImgData = NULL;
        //}
        srcImgDataLengh = 0;
    }
}ImgDataStruct;

typedef struct tag_COLOR_INFO
{
    int iColorAlpha;        //透明度  0-255 其中0为全透明
    int iColorR;               //红色分量 0-255
    int iColorG;               //绿色分量 0-255
    int iColorB;               //蓝色分量 0-255

    tag_COLOR_INFO() :
        iColorAlpha(0),
        iColorR(255),
        iColorG(255),
        iColorB(255)
    {  }
}COLOR_INFO;

typedef struct tag_PositionInfo
{
    int iPosX;          //字符叠加的左顶点 x 坐标， 范围是大于等于0，
    int iPosY;          //字符叠加的左顶点 y 坐标 ， 当值为-1时， 在图片的上方之外的区域叠加 ; 值为-2时， 在图片下方之外的区域叠加， 其他负数的值按-2处理, 默认为-2                            

    tag_PositionInfo() :
        iPosX(0),
        iPosY(-2)
    {};

}PositionInfo;

typedef struct _OverlayInfo
{
    WCHAR* szOverlayString;     //需要叠加的字符串
    int itextLength;                    //字符串长度
    int iFontSize;                        //叠加字符的大小
    COLOR_INFO st_fontColor;    //字体颜色
    COLOR_INFO st_backgroundColor;  //背景颜色
    PositionInfo st_FontPosition;    //叠加字符的位置

    _OverlayInfo() :
        szOverlayString(NULL),
        itextLength(0),
        iFontSize(32)
    {
    }
    ~_OverlayInfo()
    {
        szOverlayString = NULL;
    }

}OverlayInfo;

//************************************
// Method:        Tool_GetEncoderClsid
// Describe:        获取对应图片格式的 CLSID 
// FullName:      Tool_GetEncoderClsid
// Access:          public 
// Returns:        int
// Returns Describe: 成功返回0， 失败返回 -1
// Parameter:    const WCHAR * format  , 图片格式的文字表述，如bmp格式为 "image/bmp", jpg格式为 "image/jpeg"
// Parameter:    CLSID * pClsid 
//************************************
int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

//************************************
// Method:        myDrawString
// Describe:        对图片进行字符叠加，叠加区域为图片最下方，新建一块蓝色的区域，区域高度为字符串高度，宽度为图片宽度
// FullName:      myDrawString
// Access:          public 
// Returns:        int
// Returns Describe: 返回值0，处理成功；1，参数错误； 2，创建流失败;  3,写入流失败； 4,画图失败；5，绘制字符串失败；6，图片数据保存到流失败；7，传入图片缓存区大小不足；
// Parameter:    const ImgDataStruct dataStruct
// Parameter:    const OverlayInfo overlayInfo
// Parameter:    UCHAR * destImgBuffer  图片缓冲区，输出图片为bmp格式，rgb24
// Parameter:    long & destBufferSize ， 输入输出参数， 传入图片缓冲区长度，输出实际图片大小
//************************************
int Tool_DrawString_1(const ImgDataStruct dataStruct, const OverlayInfo overlayInfo, UCHAR* destImgBuffer, size_t& destBufferSize);

//************************************
// Method:        Tool_OverlayStringToImg
// Describe:        在指定图片的指定位置，绘制指定字符串，不另外创建区域
// FullName:      Tool_OverlayStringToImg
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    unsigned char  * pImgsrc :传入图片数据
// Parameter:    long srcSize                      ：传入图片数据长度
// Parameter:    unsigned char  * pImgDest ：输出图片缓冲区
// Parameter:    long & DestSize                ：输入输出参数，缓冲区长度
// Parameter:    const wchar_t * DestString :需要绘制的字符串
// Parameter:    int FontSize                       ：字符叠加的字体大小
// Parameter:    int x                                  ：字符叠加的起始位置 x 坐标
// Parameter:    int y                                  ：字符叠加的起始位置 y 坐标
// Parameter:    int colorR                         ：叠加字符的颜色的红色分量
// Parameter:    int colorG                         ：叠加字符的颜色的绿色分量
// Parameter:    int colorB                          ：叠加字符的颜色的蓝色分量
// Parameter:    int compressQuality          ：图片压缩质量，范围从1到100
//************************************
bool Tool_OverlayStringToImg(unsigned char* pImgsrc, long srcSize,
    unsigned char* pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality);

//************************************
// Method:        Img_string2wstring
// Describe:        字符串转换函数，将指定字符串转换为宽字符串
// FullName:      Img_string2wstring
// Access:          public 
// Returns:        std::wstring : 输出参数，转换后的宽字符串
// Returns Describe:
// Parameter:    std::string strSrc ： 输入参数， 待转换的字符串
//************************************
std::wstring Img_string2wstring(std::string strSrc);

//************************************
// Method:        Tool_Img_ScaleJpg
// Describe:        对指定图片进行缩放和压缩处理
// FullName:      Tool_Img_ScaleJpg
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    PBYTE pbSrc  ：源图片数据
// Parameter:    int iSrcLen       :源图片数据长度
// Parameter:    PBYTE pbDst    ：输出图片缓冲区
// Parameter:    DWORD * iDstLen    :输入输出函数，输入时为缓冲区长度，输出为实际图片大小
// Parameter:    int iDstWidth          :指定需要缩放的宽度
// Parameter:    int iDstHeight         :指定需要缩放的高度
// Parameter:    int compressQuality    :输出图片的压缩质量，范围从 1 到 100
//************************************
bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, DWORD *iDstLen, int iDstWidth, int iDstHeight, int compressQuality);

//************************************
// Method:        ConvertRGB2YUV422
// Describe:        把RGB数据转换成YUV422 数据
// FullName:      ConvertRGB2YUV422
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    unsigned char * RGB     : RGB数据
// Parameter:    unsigned char * YUV    ：yuv数据缓存大小 (注: 一般YUV 数据的大小为, 以24位bmp图为例 length字节 = ((iWidth*24 +31 ) /8)*height  ) 
// Parameter:    int iWidth     : RGB 图片宽度
// Parameter:    int iHeight   :RGB图片高度
//************************************
void Tool_ConvertRGB2YUV422(unsigned char* RGB, unsigned char* YUV, int iWidth, int iHeight);

//************************************
// Method:        Tool_GetRGBDataFromBmpFile
// Describe:        从bmp文件中读取 rgb 数据
// FullName:      Tool_GetRGBDataFromBmpFile
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * pBmpFile
// Parameter:    void * rgbDataBuff
// Parameter:    long & bufferLength
// Parameter:    int & iwidth
// Parameter:    int & iHeight
//************************************
void Tool_GetRGBDataFromBmpFile(const char* pBmpFile, void* rgbDataBuff, long& bufferLength, int& iwidth, int& iHeight);

//************************************
// Method:        Tool_ConverBmpFile2Yuv422
// Describe:        从bmp文件中读取 rgb 数据转换成YUV422
// FullName:      Tool_ConverBmpFile2Yuv422
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * pBmpFile
// Parameter:    void * yuvBuff     :YUV数据缓冲区
// Parameter:    long & bufferLength    :输入输出参数， 输入时为YUV数据缓冲区大小， 输出时为实际YUV数据大小  
// Parameter:    int & Width        :输出参数， yuv图片宽度
// Parameter:    int & Height       :输出参数， YUV图片高度
//************************************
void Tool_ConverBmpFile2Yuv422(const char* pBmpFile, void* yuvBuff, long& bufferLength, int& Width, int& Height);

//************************************
// Method:        Tool_RgbBin2ByteBin
// Describe:       将PCC200设备输出的二值图数据翻转
// FullName:      Tool_RgbBin2ByteBin
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE m_rgbBinImage 原二值图数据
// Parameter:    PBYTE pbByteBinImage :翻转后的二值图数据
//************************************
void Tool_RgbBin2ByteBin(PBYTE m_rgbBinImage, PBYTE pbByteBinImage);

//************************************
// Method:        BinImage2BitmapData
// Describe:        将PCC200设备输出的二值图数据转换成 bmp 数据
// FullName:      BinImage2BitmapData
// Access:          public 
// Returns:        BOOL
// Returns Describe:
// Parameter:    int iCX        :二值图宽度 ,设备输出的二值图宽度为 112
// Parameter:    int iCY        :二值图高度，设备输出的二值图高度为 20
// Parameter:    BYTE * pbByteBinImage :二值图数据
// Parameter:    char * pbBitmapData      :BMP图片数据缓冲
// Parameter:    int * piBitmapDataLen    :Bmp缓冲大小
//************************************
BOOL Tool_BinImage2BitmapData(int iCX, int iCY, BYTE* pbByteBinImage, char* pbBitmapData, int* piBitmapDataLen);


//************************************
// Method:        Tool_Bin2BMP
// Describe:        将PCC200设备输出的二值图数据转换成 bmp 数据, 该函数是对 Tool_BinImage2BitmapData 的又一层封装
// FullName:      Tool_Bin2BMP
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE pbBinData        :二值图数据
// Parameter:    PBYTE pbBmpData        :BMP图片数据缓冲
// Parameter:    INT & nBmpLen          :Bmp缓冲大小
//************************************
void Tool_Bin2BMP(PBYTE pbBinData, PBYTE pbBmpData, INT& nBmpLen);

// 描述:	转换PCC200输出的YUV422小图数据到RGB
// 参数:	pbDest			输出RGB数据的缓冲区指针;
//			pbSrc			输入YUV数据的缓冲区指针;
//			iSrcWidth		图像宽度;
//			iSrcHeight		图像高度;
//			iBGRStride		RGB数据每行的步长;
// 返回值:  返回S_OK, 表示操作成功,
//          返回E_POINTER, 参数中包含有非法的指针;
//          返回E_FAIL, 表示未知的错误导致操作失败;
bool Tool_Yuv422ToRgb(BYTE *pbDest, BYTE *pbSrc, int iSrcWidth, int iSrcHeight, int iBGRStride);

// 描述:	转换PCC200输出的YUV422小图数据成BMP格式，该方法是对 Tool_Yuv422ToRgb 的进一步封装
// 参数:	pbDest			输出BMP数据的缓冲区指针;
//			iDestBufLen		输出缓冲区大小
//			piDestLen		实际输出数据大小
//			pbSrc			输入YUV数据的缓冲区指针;
//			iSrcWidth		图像宽度;
//			iSrcHeight		图像高度;
// 返回值:  返回S_OK, 表示操作成功,
//          返回E_POINTER, 参数中包含有非法的指针;
//          返回E_FAIL, 表示未知的错误导致操作失败;
bool Tool_Yuv4222BmpFile(BYTE* pbDest, int iDestBufLen, int* piDestLen, BYTE* pbSrc, int iSrcWidth, int iSrcHeight);

//************************************
// Method:        Tool_CalculateStringWithAndHeiht
// Describe:        计算叠加字符在指定图片中占用的长度和高度
// FullName:      Tool_CalculateStringWithAndHeiht
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * overlayString ; 输入参数, 叠加的字符串
// Parameter:    int imageWidth;  输入参数，图片高度
// Parameter:    int imageHeight;  输入参数,图片宽度
// Parameter:    int fontSize;  输入参数,字体大小
// Parameter:    float & stringWidth;输出参数， 叠加字符占用宽度
// Parameter:    float & stringHeight; 输出参数，叠加字符占用高度
//************************************
bool Tool_CalculateStringWithAndHeight(const char* overlayString, const int imageWidth, const int imageHeight, const int fontSize, float& stringWidth, float& stringHeight);

#endif

