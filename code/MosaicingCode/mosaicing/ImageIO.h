#pragma once

#include "Bitmap.h"
//#include "atlstr.h"
#include "usingCV24.h"

using namespace pool;

//��ʾͼƬ
int ShowImage(pool::BitmapImage* pSrc, char win_name[]);

bool CreateFolder(std::string   strFolderPath);

// ��ȡraw8ͼ������
int ReadRaw8Image(char* path, IplImage* pImage);

// ��ȡraw8ͼ������
int ReadRaw16Image(char* path, unsigned short* pImage, int width, int height);

// ��ȡraw12ͼ������
// pImage---16λͼ��
int ReadRaw16ImageV2(char* path, unsigned short* pImage16, int width, int height);
