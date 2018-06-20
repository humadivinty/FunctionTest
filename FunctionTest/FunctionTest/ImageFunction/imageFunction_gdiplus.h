#ifndef IMGFUNCTION_GDIPLUS
#define IMGFUNCTION_GDIPLUS

#include<windows.h>
#include <string>
#include <gdiplus.h>

//BMP�ļ����:
/*
λͼ�ļ�ͷBitMapFileHeader
λͼ��ϢͷBitMapInfoHeader
��ɫ��palette
ʵ�ʵ�λͼ����imageData
*/

//λͼ�ļ�ͷ��Ҫ������
//typedef struct tagBITMAPFILEHEADER 
//{
//    WORD bfType; /* ˵���ļ������� */
//    DWORD bfSize; /* ˵���ļ��Ĵ�С�����ֽ�Ϊ��λ */
//    WORD bfReserved1; /* ����������Ϊ0 */
//    WORD bfReserved2; /* ����������Ϊ0 */
//    DWORD bfOffBits; /* ˵����BITMAPFILEHEADER�ṹ��ʼ��ʵ�ʵ�ͼ������֮����ֽ�ƫ���� */
//} BITMAPFILEHEADER;
//
//
////λͼ��Ϣͷ��Ҫ������
//typedef struct tagBITMAPINFOHEADER {
//    DWORD biSize; /* ˵���ṹ�������ֽ��� */
//    LONG biWidth; /* ������Ϊ��λ˵��ͼ��Ŀ�� */
//    LONG biHeight; /* ������Ϊ��λ˵��ͼ��ĸ��� */
//    WORD biPlanes; /* ˵��λ����������Ϊ1 */
//    WORD biBitCount; /* ˵��λ��/���أ�1��2��4��8��24 */
//    DWORD biCompression; /* ˵��ͼ���Ƿ�ѹ����ѹ������BI_RGB��BI_RLE8��BI_RLE4��BI_BITFIELDS */
//    DWORD biSizeImage; /* ���ֽ�Ϊ��λ˵��ͼ���С��������4��������*/
//    LONG biXPelsPerMeter; /*Ŀ���豸��ˮƽ�ֱ��ʣ�����/�� */
//    LONG biYPelsPerMeter; /*Ŀ���豸�Ĵ�ֱ�ֱ��ʣ�����/�� */
//    DWORD biClrUsed; /* ˵��ͼ��ʵ���õ�����ɫ�������Ϊ0������ɫ��Ϊ2��biBitCount�η� */
//    DWORD biClrImportant; /*˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��*/
//} BITMAPINFOHEADER;
//
////��ɫ��ʵ������һ�����飬����������Ԫ����λͼ�����е���ɫ����ͬ��
////������biClrUsed��biBitCount�ֶΡ�������ÿ��Ԫ�ص�������һ��RGBQUAD�ṹ�����ɫ�޵�ɫ�岿�֡�
//typedef struct tagRGBQUAD {
//    BYTE rgbBlue; /*ָ����ɫ����*/
//    BYTE rgbGreen; /*ָ����ɫ����*/
//    BYTE rgbRed; /*ָ����ɫ����*/
//    BYTE rgbReserved; /*������ָ��Ϊ0*/
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
    int iColorAlpha;        //͸����  0-255 ����0Ϊȫ͸��
    int iColorR;               //��ɫ���� 0-255
    int iColorG;               //��ɫ���� 0-255
    int iColorB;               //��ɫ���� 0-255

    tag_COLOR_INFO() :
        iColorAlpha(0),
        iColorR(255),
        iColorG(255),
        iColorB(255)
    {  }
}COLOR_INFO;

typedef struct tag_PositionInfo
{
    int iPosX;          //�ַ����ӵ��󶥵� x ���꣬ ��Χ�Ǵ��ڵ���0��
    int iPosY;          //�ַ����ӵ��󶥵� y ���� �� ��ֵΪ-1ʱ�� ��ͼƬ���Ϸ�֮���������� ; ֵΪ-2ʱ�� ��ͼƬ�·�֮���������ӣ� ����������ֵ��-2����, Ĭ��Ϊ-2                            

    tag_PositionInfo() :
        iPosX(0),
        iPosY(-2)
    {};

}PositionInfo;

typedef struct _OverlayInfo
{
    WCHAR* szOverlayString;     //��Ҫ���ӵ��ַ���
    int itextLength;                    //�ַ�������
    int iFontSize;                        //�����ַ��Ĵ�С
    COLOR_INFO st_fontColor;    //������ɫ
    COLOR_INFO st_backgroundColor;  //������ɫ
    PositionInfo st_FontPosition;    //�����ַ���λ��

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
// Describe:        ��ȡ��ӦͼƬ��ʽ�� CLSID 
// FullName:      Tool_GetEncoderClsid
// Access:          public 
// Returns:        int
// Returns Describe: �ɹ�����0�� ʧ�ܷ��� -1
// Parameter:    const WCHAR * format  , ͼƬ��ʽ�����ֱ�������bmp��ʽΪ "image/bmp", jpg��ʽΪ "image/jpeg"
// Parameter:    CLSID * pClsid 
//************************************
int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

//************************************
// Method:        myDrawString
// Describe:        ��ͼƬ�����ַ����ӣ���������ΪͼƬ���·����½�һ����ɫ����������߶�Ϊ�ַ����߶ȣ����ΪͼƬ���
// FullName:      myDrawString
// Access:          public 
// Returns:        int
// Returns Describe: ����ֵ0������ɹ���1���������� 2��������ʧ��;  3,д����ʧ�ܣ� 4,��ͼʧ�ܣ�5�������ַ���ʧ�ܣ�6��ͼƬ���ݱ��浽��ʧ�ܣ�7������ͼƬ��������С���㣻
// Parameter:    const ImgDataStruct dataStruct
// Parameter:    const OverlayInfo overlayInfo
// Parameter:    UCHAR * destImgBuffer  ͼƬ�����������ͼƬΪbmp��ʽ��rgb24
// Parameter:    long & destBufferSize �� ������������� ����ͼƬ���������ȣ����ʵ��ͼƬ��С
//************************************
int Tool_DrawString_1(const ImgDataStruct dataStruct, const OverlayInfo overlayInfo, UCHAR* destImgBuffer, size_t& destBufferSize);

//************************************
// Method:        Tool_OverlayStringToImg
// Describe:        ��ָ��ͼƬ��ָ��λ�ã�����ָ���ַ����������ⴴ������
// FullName:      Tool_OverlayStringToImg
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    unsigned char  * pImgsrc :����ͼƬ����
// Parameter:    long srcSize                      ������ͼƬ���ݳ���
// Parameter:    unsigned char  * pImgDest �����ͼƬ������
// Parameter:    long & DestSize                �������������������������
// Parameter:    const wchar_t * DestString :��Ҫ���Ƶ��ַ���
// Parameter:    int FontSize                       ���ַ����ӵ������С
// Parameter:    int x                                  ���ַ����ӵ���ʼλ�� x ����
// Parameter:    int y                                  ���ַ����ӵ���ʼλ�� y ����
// Parameter:    int colorR                         �������ַ�����ɫ�ĺ�ɫ����
// Parameter:    int colorG                         �������ַ�����ɫ����ɫ����
// Parameter:    int colorB                          �������ַ�����ɫ����ɫ����
// Parameter:    int compressQuality          ��ͼƬѹ����������Χ��1��100
//************************************
bool Tool_OverlayStringToImg(unsigned char* pImgsrc, long srcSize,
    unsigned char* pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality);

//************************************
// Method:        Img_string2wstring
// Describe:        �ַ���ת����������ָ���ַ���ת��Ϊ���ַ���
// FullName:      Img_string2wstring
// Access:          public 
// Returns:        std::wstring : ���������ת����Ŀ��ַ���
// Returns Describe:
// Parameter:    std::string strSrc �� ��������� ��ת�����ַ���
//************************************
std::wstring Img_string2wstring(std::string strSrc);

//************************************
// Method:        Tool_Img_ScaleJpg
// Describe:        ��ָ��ͼƬ�������ź�ѹ������
// FullName:      Tool_Img_ScaleJpg
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    PBYTE pbSrc  ��ԴͼƬ����
// Parameter:    int iSrcLen       :ԴͼƬ���ݳ���
// Parameter:    PBYTE pbDst    �����ͼƬ������
// Parameter:    DWORD * iDstLen    :�����������������ʱΪ���������ȣ����Ϊʵ��ͼƬ��С
// Parameter:    int iDstWidth          :ָ����Ҫ���ŵĿ��
// Parameter:    int iDstHeight         :ָ����Ҫ���ŵĸ߶�
// Parameter:    int compressQuality    :���ͼƬ��ѹ����������Χ�� 1 �� 100
//************************************
bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, DWORD *iDstLen, int iDstWidth, int iDstHeight, int compressQuality);

//************************************
// Method:        ConvertRGB2YUV422
// Describe:        ��RGB����ת����YUV422 ����
// FullName:      ConvertRGB2YUV422
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    unsigned char * RGB     : RGB����
// Parameter:    unsigned char * YUV    ��yuv���ݻ����С (ע: һ��YUV ���ݵĴ�СΪ, ��24λbmpͼΪ�� length�ֽ� = ((iWidth*24 +31 ) /8)*height  ) 
// Parameter:    int iWidth     : RGB ͼƬ���
// Parameter:    int iHeight   :RGBͼƬ�߶�
//************************************
void Tool_ConvertRGB2YUV422(unsigned char* RGB, unsigned char* YUV, int iWidth, int iHeight);

//************************************
// Method:        Tool_GetRGBDataFromBmpFile
// Describe:        ��bmp�ļ��ж�ȡ rgb ����
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
// Describe:        ��bmp�ļ��ж�ȡ rgb ����ת����YUV422
// FullName:      Tool_ConverBmpFile2Yuv422
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    const char * pBmpFile
// Parameter:    void * yuvBuff     :YUV���ݻ�����
// Parameter:    long & bufferLength    :������������� ����ʱΪYUV���ݻ�������С�� ���ʱΪʵ��YUV���ݴ�С  
// Parameter:    int & Width        :��������� yuvͼƬ���
// Parameter:    int & Height       :��������� YUVͼƬ�߶�
//************************************
void Tool_ConverBmpFile2Yuv422(const char* pBmpFile, void* yuvBuff, long& bufferLength, int& Width, int& Height);

//************************************
// Method:        Tool_RgbBin2ByteBin
// Describe:       ��PCC200�豸����Ķ�ֵͼ���ݷ�ת
// FullName:      Tool_RgbBin2ByteBin
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE m_rgbBinImage ԭ��ֵͼ����
// Parameter:    PBYTE pbByteBinImage :��ת��Ķ�ֵͼ����
//************************************
void Tool_RgbBin2ByteBin(PBYTE m_rgbBinImage, PBYTE pbByteBinImage);

//************************************
// Method:        BinImage2BitmapData
// Describe:        ��PCC200�豸����Ķ�ֵͼ����ת���� bmp ����
// FullName:      BinImage2BitmapData
// Access:          public 
// Returns:        BOOL
// Returns Describe:
// Parameter:    int iCX        :��ֵͼ��� ,�豸����Ķ�ֵͼ���Ϊ 112
// Parameter:    int iCY        :��ֵͼ�߶ȣ��豸����Ķ�ֵͼ�߶�Ϊ 20
// Parameter:    BYTE * pbByteBinImage :��ֵͼ����
// Parameter:    char * pbBitmapData      :BMPͼƬ���ݻ���
// Parameter:    int * piBitmapDataLen    :Bmp�����С
//************************************
BOOL Tool_BinImage2BitmapData(int iCX, int iCY, BYTE* pbByteBinImage, char* pbBitmapData, int* piBitmapDataLen);


//************************************
// Method:        Tool_Bin2BMP
// Describe:        ��PCC200�豸����Ķ�ֵͼ����ת���� bmp ����, �ú����Ƕ� Tool_BinImage2BitmapData ����һ���װ
// FullName:      Tool_Bin2BMP
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE pbBinData        :��ֵͼ����
// Parameter:    PBYTE pbBmpData        :BMPͼƬ���ݻ���
// Parameter:    INT & nBmpLen          :Bmp�����С
//************************************
void Tool_Bin2BMP(PBYTE pbBinData, PBYTE pbBmpData, INT& nBmpLen);

// ����:	ת��PCC200�����YUV422Сͼ���ݵ�RGB
// ����:	pbDest			���RGB���ݵĻ�����ָ��;
//			pbSrc			����YUV���ݵĻ�����ָ��;
//			iSrcWidth		ͼ����;
//			iSrcHeight		ͼ��߶�;
//			iBGRStride		RGB����ÿ�еĲ���;
// ����ֵ:  ����S_OK, ��ʾ�����ɹ�,
//          ����E_POINTER, �����а����зǷ���ָ��;
//          ����E_FAIL, ��ʾδ֪�Ĵ����²���ʧ��;
bool Tool_Yuv422ToRgb(BYTE *pbDest, BYTE *pbSrc, int iSrcWidth, int iSrcHeight, int iBGRStride);

// ����:	ת��PCC200�����YUV422Сͼ���ݳ�BMP��ʽ���÷����Ƕ� Tool_Yuv422ToRgb �Ľ�һ����װ
// ����:	pbDest			���BMP���ݵĻ�����ָ��;
//			iDestBufLen		�����������С
//			piDestLen		ʵ��������ݴ�С
//			pbSrc			����YUV���ݵĻ�����ָ��;
//			iSrcWidth		ͼ����;
//			iSrcHeight		ͼ��߶�;
// ����ֵ:  ����S_OK, ��ʾ�����ɹ�,
//          ����E_POINTER, �����а����зǷ���ָ��;
//          ����E_FAIL, ��ʾδ֪�Ĵ����²���ʧ��;
bool Tool_Yuv4222BmpFile(BYTE* pbDest, int iDestBufLen, int* piDestLen, BYTE* pbSrc, int iSrcWidth, int iSrcHeight);

//************************************
// Method:        Tool_CalculateStringWithAndHeiht
// Describe:        ��������ַ���ָ��ͼƬ��ռ�õĳ��Ⱥ͸߶�
// FullName:      Tool_CalculateStringWithAndHeiht
// Access:          public 
// Returns:        bool
// Returns Describe:
// Parameter:    char * overlayString ; �������, ���ӵ��ַ���
// Parameter:    int imageWidth;  ���������ͼƬ�߶�
// Parameter:    int imageHeight;  �������,ͼƬ���
// Parameter:    int fontSize;  �������,�����С
// Parameter:    float & stringWidth;��������� �����ַ�ռ�ÿ��
// Parameter:    float & stringHeight; ��������������ַ�ռ�ø߶�
//************************************
bool Tool_CalculateStringWithAndHeight(const char* overlayString, const int imageWidth, const int imageHeight, const int fontSize, float& stringWidth, float& stringHeight);

#endif

