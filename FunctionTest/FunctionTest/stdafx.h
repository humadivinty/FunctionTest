// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <windows.h>

#define MAX_IMG_BUF (20*1024*1024)

#define SAFE_DELETT_ARRAY(arg) \
if (arg)                    \
{                             \
    delete[] arg;        \
    arg = NULL;       \
}