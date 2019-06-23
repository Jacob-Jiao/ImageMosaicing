
#pragma once
//#include <afx.h>
//#include "stdafx.h"
#include "math.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include "MemoryPool.h"
#include "Point.h"
#include "Basic.h"

using namespace std;

#define _OUT 
#define _IN

// �������ʾֱ��
struct PolarLine
{
	float p;
	float th;

	PolarLine()
	{
		p=0;
		th=0;
	}
};

struct LineABC
{
	float a, b, c;

	LineABC()
	{
		a = b = c;
	}
};

struct ProjectMat
{
	float m[9];
};

struct ProjectMat64F
{
	double m[9];
};

namespace pool
{
	const float pi = 3.1415926f;
	const float PI_32F = 3.14159265358979f;
	const double PI_64F = 3.14159265358979;
	const float PI = 3.1415926f;
	const float PI_HALF = 3.1415926f/2;
	const float fRadToDegree = 180/pi;
	//typedef unsigned char UCHAR;

	struct PixelRGB
	{
		int r,g,b;
	};

	struct AngleOfPoint
	{
		float angle;
		int index;
		int fValid;

		bool   operator <  (const AngleOfPoint& rhs)  const   //��������ʱ����д�ĺ���
		{   
			return angle<rhs.angle; 
		}
	};

	// �ݶ�
	struct Gradient32F
	{
		float gx;
		float gy;
		float w; // Ȩ��
		int seq; 
	};

	struct Color
	{
		int b, g, r, a;

		Color(int b, int g, int r)
		{	
			this->b = b;
			this->g = g;
			this->r = r;
		}

		Color()
		{

		}
	};

	struct BitmapImage
	{
		unsigned char* imageData; 
		int width; // ���ؿ��
		int height;// ���ظ߶�
		int widthStep; // ������ 4�ı���
		int nChannels; // ͨ���� �Ҷ�ͼΪ1, ��ɫͼΪ3
		BitmapImage()
		{
			imageData = NULL;
			width=0;
			height=0;
			widthStep=0;
			nChannels=0;
		}
		BitmapImage(unsigned char* pImgData, int width, int height, int widthStep, int nChannels)
		{
			this->imageData = pImgData;
			this->width = width;
			this->height = height;
			this->widthStep = widthStep;
			this->nChannels = nChannels;
		}
	};

	// "��"���νṹ��
	struct BlockInf {
		int xbeg; // x������꣨�����Խǵ㣩
		int xend; // x�յ�����
		int ybeg; // y�������
		int yend; // y�յ�����
		BlockInf()
		{
			xbeg=0;
			xend=0;
			ybeg=0;
			yend=0;
		}
	};

	struct InterestRegion32F
	{
		float xbeg; // ���x
		float xend; // �յ�x
		float ybeg; // ���y
		float yend; // �յ�y
	};

	struct MatchPoint
	{
		float x;
		float y;
		float fMatchScore;
	};

	//struct AngleOfPoint
	//{
	//	float angle;
	//	int index;
	//	int fValid;

	//	bool   operator <  (const AngleOfPoint& rhs)  const   //��������ʱ����д�ĺ���
	//	{   
	//		return angle<rhs.angle; 
	//	}
	//};
}

// ��ȡͼ��ĸߡ���
int GetImageSize(const pool::BitmapImage *imSrc,int &w,int &h,int &ws);

// �·������ڴ�
pool::BitmapImage* CreateBitmap8U( int width, int height, int nChannels);

// �·������ڴ�
extern "C" __declspec(dllexport)
void ReleaseBitmap8U(pool::BitmapImage* &pDstImage);

void SaveBitmap(char* path, pool::BitmapImage* &pDstImage);

// �·������ڴ�Ŀ���
pool::BitmapImage* CloneBitmap8U(const pool::BitmapImage* pSrcImage );

// �������ܣ���ͼƬ�н�ȡͼƬ���������� �Խǵ�
// �·������ڴ�
pool::BitmapImage* CutPatchImage(const pool::BitmapImage *src,
								pool::IntPoint startp, pool::IntPoint endp);

pool::BitmapImage* CutPatchImage(const pool::BitmapImage *src,
								 const pool::InterestRegion2 &roi);

// �������ܣ�д���ݵ�ͼ���е�ĳ����������   
// �Խǵ㣺startp   endp
int WriteBackToImageRegion(pool::BitmapImage *src, pool::BitmapImage *seg, 
						   pool::IntPoint startp, pool::IntPoint endp);

// �������ܣ�д���ݵ�ͼ���е�ĳ����������   
// �Խǵ㣺startp   endp
int WriteBackToImageRegion(pool::BitmapImage *src, pool::BitmapImage *seg, const pool::InterestRegion2 &roi);

bool IsBitmapValid(const pool::BitmapImage *srcImage);

// ������ͼ������
bool ZeroImage(pool::BitmapImage *pSrcImage);

void DestroyMemoryPool();

// ��ȡCPU����
int GetCpuNum();

// ��֤�Ƿ�����Ч����
int InValidDate(int deadLine_year, int deadLine_month);

// ����16λͼ���Ԫ��
inline ushort GetImgVal16(char* imageData, int x, int y, int ws)
{
	return *(ushort*)(imageData + y*ws + x*2);
}

// ���vector�ڴ�
template < class T >
void ClearVectorMemory(std::vector< T >& vt)
{
	std::vector< T > vtTemp;
	veTemp.swap(vt);
}