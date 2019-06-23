#pragma once

#include "Bitmap.h"
#include <string>
#include "usingCV24.h"

// model�ľ�ֵ�ͱ�׼����֪
bool NccMatch2D(pool::BitmapImage *left, pool::BitmapImage *right,
				float meanM, float stdM, 
				int validWidth,
				int du, int dv,
				float &fScore);

bool CalNCCScore(pool::BitmapImage *left, pool::BitmapImage *right,
				 int dx, int dy,
				 float &fScore,
				 int minOverlapPointsNum);

// model�ľ�ֵ�ͷ�����֪
bool CalNCCScore(pool::BitmapImage *model, pool::BitmapImage *target,
				 float meanM, float stdM,
				 int dx, int dy,
				 float &fScore);

// ����ƥ���
// strMethod = "SAD" �� "NCC"
bool CalMatchScore(pool::BitmapImage *left, pool::BitmapImage *right,
				   int du, int dv,
				   float &fScore,
				   int minOverlapPointsNum,
				   string strMethod);

// strMethod = "SAD" �� "NCC"
// right-templateImg
bool CalMatchScore2(pool::BitmapImage *left, pool::BitmapImage *right,
					int dx, int dy,
					float &fScore,
					string strMethod);

// ����ƥ���
bool CalMatchScore_Circular(pool::BitmapImage *left, pool::BitmapImage *right,
							float &fScore );

// ���ܣ���Ȩ2D-NCC
// ÿһ�е�Ȩ����ͬ
bool NCC_2D_Weight(pool::BitmapImage *left, pool::BitmapImage *right,
				   int du, int dv,
				   float &fScore,
				   float* pWeightColLeft = NULL, float *pWeightColRight = NULL,
				   pool::BitmapImage* pSignT = NULL, pool::BitmapImage* pSignMWiden = NULL) ;

// ���ܣ���Ȩ2D-NCC-For Polar
// ÿһ�е�Ȩ����ͬ
bool NCC_2D_Weight_For_Polar(pool::BitmapImage *left, pool::BitmapImage *right,
							 int du, int dv,
							 int validWidth,
							 float &fScore,
							 const float* pWeightCol = NULL);

// ����ƥ���
// strMethod = "SAD" �� "NCC"
bool CalMatchScore_BPC(const pool::BitmapImage *left, const pool::BitmapImage *right,
					   int *pSumTable2WidenM, int* pSubTable2T,
					   const int du, const int dv,
					   float &fScore,
					   const float meanLeft, const float stdLeft, 
					   const float meanRight, const float stdRight );

// ����ƥ���
// strMethod = "NCC"
bool CalMatchScore(const pool::BitmapImage *left, const pool::BitmapImage *right,
				   const int du, const int dv,
				   float &fScore,
				   const float meanLeft, const float stdLeft, 
				   const float meanRight, const float stdRight );

// ����ƥ���
// "NCC"
// nDataLeft * 2 = nDataRight
//bool CalMatchScore1D(int* left, int *right, int nDataLeft, int nDataRight, 
//					 const int du,
//					 float &fScore,
//					 const float meanLeft, const float stdLeft, 
//					 const float meanRight, const float stdRight );

// ����ƥ���
// "NCC"
// nDataLeft * 2 = nDataRight
//bool CalMatchScore1D(float* left, float *right, int nDataLeft, int nDataRight, 
//					 const int du,
//					 float &fScore,
//					 const float meanLeft, const float stdLeft, 
//					 const float meanRight, const float stdRight,
//					 float* pWeight = NULL);


// ����ƥ���
// ����steger�ķ���
// left-----
// const Gradient32F* pGad1, ��һ�����ݶ�
// const Gradient32F* pGad2, ��һ�����ݶ�
bool CalStegerMatchScore(int wT, int hT, int wsT,
						 int wM, int hM, int wsM, 
						 const pool::Gradient32F* pGad1, const pool::Gradient32F* pGad2, 
						 const int du, const int dv,
						 float &fScore,
						 const int minOverlapPointsNum );


bool Projection1D( pool::BitmapImage* pSrc, int *pProjection, string axis = "X", pool::InterestRegion2 *pROI = NULL );

bool Projection1D( pool::BitmapImage* pSrc, float *pProjection, string axis = "X", pool::InterestRegion2 *pROI = NULL );

// һάͶӰ
// pWeightCol----ÿһ�е�Ȩ��
bool Projection1D_Weighted( pool::BitmapImage* pSrc, int *pProjection, string axis /*= "X" */, 
						   float* pWeightCol, pool::InterestRegion2 *pROI = NULL );

// һάͶӰ
// pWeightCol----ÿһ�е�Ȩ��
bool Projection1D_Weighted( pool::BitmapImage* pSrc, float *pProjection, string axis /*= "X" */, 
						   float* pWeightCol, pool::InterestRegion2 *pROI = NULL, int shift = 0 );

void CalOverlapRegion(int widthLeft, int heightLeft, 
					  int widthR, int heightR,
					  int dy, int dx,
					  int &xLeftBeg, int &yLeftBeg, int &xLeftEnd, int &yLeftEnd);


bool CalSumTable2( pool::BitmapImage* pSrc, int* pSubTable2 );

bool CalSumTable( pool::BitmapImage* pSrc, int* pSubTable );

bool CalSumTable2(IplImage* pSrc, int* pSubTable2 );

bool CalSumTable(IplImage* pSrc, int* pSubTable );

// ����space time stereo
bool CalSumTable(IplImage** pSrc, int* pSubTable, int nImages );

// ����space time stereo
bool CalSumTable2(IplImage** pSrc, int* pSubTable2, int nImages );

bool GetSum( const int* pSumTable, int w, int h, int ws, 
			pool::InterestRegion2 &roi, int &sum );

// ����ͼ�����������
bool CalIncrementSign(const pool::BitmapImage* pSrc, pool::BitmapImage* pSign);

// ����ͼ�����������
template<class T1>
bool CalIncrementSign1D(const T1 *pData, int nData, unsigned char* pSign)
{
	if( (pData==NULL) || (pSign==NULL) )
		return false;

	memset(pSign, 0, nData);

	T1 lastVal = pData[0];
	// �ȴ����1��

	for(int x=1; x<nData; x++)
	{
		if(pData[x]>=lastVal)
		{
			pSign[x] = 255;
		}
		lastVal = pData[x];
	}
	return true;
}

// ������������ͼ�񣬼�������ģ��
bool CalBasicMask(const pool::BitmapImage* pSign1, const pool::BitmapImage* pSign2, 
				  pool::BitmapImage* pMask, float &validRatio);

// ������������ͼ�񣬼�������ģ��
// pSign2�ĸ߶���pSign1�ĸ߶ȵ�2��
bool CalBasicMask(const pool::BitmapImage* pSign1, const pool::BitmapImage* pSign2, 
				  int dy,
				  pool::BitmapImage* pMask, float &validRatio);

// ������������ͼ�񣬼�������ģ��
bool CalBasicMask2(const pool::BitmapImage* pSign1, const pool::BitmapImage* pSign2, 
				  pool::BitmapImage* pMask, float &validRatio);