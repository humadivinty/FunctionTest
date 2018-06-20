// FunctionTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ImageFunction/imageFunction_gdiplus.h"
#include "DirFunction/DirFunction.h"


int _tmain(int argc, _TCHAR* argv[])
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    float fStringWidth = 0.0, fStringHeight = 0.0;
    Tool_CalculateStringWithAndHeight("hello.", 1920, 1080, 32, fStringWidth, fStringHeight);
    printf(" 'hello' width = %f, height = %f\n", fStringWidth, fStringHeight);

    Tool_CalculateStringWithAndHeight("hello.\t", 1920, 1080, 32, fStringWidth, fStringHeight);
    printf(" 'hello\t' width = %f, height = %f\n", fStringWidth, fStringHeight);

    Tool_CalculateStringWithAndHeight("\t", 1920, 1080, 32, fStringWidth, fStringHeight);
    printf(" '\t' width = %f, height = %f\n", fStringWidth, fStringHeight);

    Tool_CalculateStringWithAndHeight("hello.\tdfgsdfg", 1920, 1080, 32, fStringWidth, fStringHeight);
    printf(" 'hello.\tdfgsdfg' width = %f, height = %f\n", fStringWidth, fStringHeight);


    Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}

