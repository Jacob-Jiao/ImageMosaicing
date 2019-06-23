/*

*/
#pragma once

#include "Bitmap.h"
//#include "ati.h"


//// ��ֵ�˲�
//int MeanSmooth( IplImage* pSrc, IplImage* pDst,  int radius);

// ��˹�˲�
//bool GaussFilter(pool::BitmapImage *pSrc, pool::BitmapImage *pDst, const float fSigma = 0.8f, int winSize = 5);

// *********************************************************************************
// �������ܣ���˹�˲�
// winSize����˹���ڵĿ��
// *********************************************************************************
bool GaussFilter_Int(pool::BitmapImage *pSrc, pool::BitmapImage *pDst, float fSigma = 0.8f, int winSize = 5);

// *********************************************************************************
// �������ܣ���˹�˲�
// winSize����˹���ڵĿ��
// *********************************************************************************
bool GaussFilter2( const unsigned char *pSrcData, const int wSrc, const int hSrc, const int wsSrc, 
				 unsigned char *pDstData,
				 const float fSigma = 0.8f, const int winSize = 5);

// *********************************************************************************
// �������ܣ���˹�˲� ��Maskͼ��
// winSize����˹���ڵĿ��
// ����pSrc==pDst
// *********************************************************************************
bool GaussFilterMask(const pool::BitmapImage *pSrc, 
					 pool::BitmapImage *pMaskImage,
					 pool::BitmapImage *pDst, const float fSigma = 0.8f, int winSize = 5);

// ��ֵ�˲�
bool MeanFilter(pool::BitmapImage *pSrc, pool::BitmapImage *pDst, int winSize);

// *********************************************************************************
// ��ֵ�˲�
// *********************************************************************************
bool MeanFilterSSE(const pool::BitmapImage *pSrc, pool::BitmapImage *pDst, int winSize);

// 1D��˹ƽ��
int GaussSmoothData1D(float* srcData, float* dstData, int N, float sigma, int winR);