#pragma once

//#include "ati.h"
#include "Bitmap.h"

// ����ͼ�����Ϣ��
//bool CalInformationEntropy(IplImage* pSrcImage, float &fInfoEntropy);

// ����ͼ�����Ϣ��
bool CalInformationEntropy(pool::BitmapImage* pSrcImage, float &fInfoEntropy);

// �������ܣ���ͼ��ֱ��ͼ
int CalHistogram(pool::BitmapImage *im, float hist[256]);

//// ����դ���еķ���
//bool CalStdInGrid(IplImage* pSrc, 
//				 const int x, const int y, int gridSize, 
//				 float &fStdR, float &fStdG, float &fStdB);

// ��ֱ��ͼ�� ��ֵ �� ����
int GetMeanDeviation2(float *Hist, float &mean, float &deviation);

// ��ֱ��ͼ�� ��ֵ �� ����
int GetMeanDeviation(float *Hist, int maxLevel, float &mean, float &deviation);

// ��ֱ��ͼ�� ��ֵ �� ����
int GetMeanDeviation(float *Hist, float &mean, float &deviation);

// ��ֱ��ͼ�� ��ֵ �� ����
int GetMeanVariance(float *Hist, int maxLevel, float &mean, float &deviation);

// ���ֵ �� ��׼��
template<class T, class T2>
int GetMeanAndSTD(std::vector<T>vecData, T2 &mean,T2 &stdValue)
{
	// std---��׼��
	int ndata = (int)vecData.size();

	float N=0, M, D, acc;

	for(int u=0; u<ndata; u++)
	{
		N+=vecData[u]; // �ܺ�
	}

	M = N/ndata; // ��ֵ

	acc=0;
	for(int u=0;u<ndata;u++)
	{
		acc+=(vecData[u]-M)*(vecData[u]-M); // �󷽲�
	}
	D=acc/ndata; // ����

	mean=M;
	stdValue = sqrt(D); // ���׼��
	return 1;
}

// ���ֵ �� ��׼��
template<class T1, class T2>
int GetMeanAndSTD(const T1 *pdata, const int ndata,T2 &mean, T2 &fStd)
{
	// std---��׼��

	T2 N=0, M = 0, D = 0;
	T2 acc = 0;

	for(int u=0; u<ndata; u++)
	{
		N += (T2)pdata[u]; // �ܺ�
	}

	M = N/ndata; // ��ֵ

	acc = 0;
	for(int u=0;u<ndata;u++)
	{
		acc += (pdata[u]-M)*(pdata[u]-M); // �󷽲�
	}
	D = acc/ndata; // ����

	mean = M;
	fStd = sqrt(D); // ���׼��
	return 1;
}
 
// ���ֵ �� ��׼��  (����Ȩ)
// sum(pWeight)Ӧ�õ���1
template<class T1, class T2>
int GetMeanAndSTD_Weight(const T1 *pdata, const int ndata, T2 &mean, T2 &fStd, T2 *pWeight0)
{
	if(pdata==NULL)
		return -1;

	T2* pWeight = new T2[ndata];
	memcpy( pWeight, pWeight0, ndata * sizeof(T2) );

	// 
	T2 sumOfWeight = 0;
	Accumulate( pWeight, ndata, sumOfWeight );

	// ���ֵ
	mean = 0;
	for(int u=0; u<ndata; u++)
	{
		pWeight[u] /= sumOfWeight; // Ȩ�ع�һ��
		mean += pWeight[u] * pdata[u];
	}

	T2 acc = 0;
	for(int u=0; u<ndata; u++)
	{
		acc += (pdata[u]-mean) * (pdata[u]-mean) * pWeight[u];
	}

	fStd = sqrt(acc); // ��׼��

	delete pWeight; pWeight = NULL;

	return 1;
}

// ���ֵ �� ��׼��
template<class T1>
int GetMeanAndSTD2(const T1 *pdata, const int ndata,float &mean, float &fStd)
{
	// std---��׼��

	float N=0, M = 0, D = 0;
	float acc = 0;

	for(int u=0; u<ndata; u++)
	{
		N += pdata[u]; // �ܺ�
	}

	M = N/ndata; // ��ֵ

	acc = 0;
	for(int u=0;u<ndata;u++)
	{
		acc += (pdata[u]-M)*(pdata[u]-M); // �󷽲�
	}
	D = acc/ndata; // ����

	mean = M;
	fStd = sqrt(D); // ���׼��
	return 1;
}

template<class T1>
bool Nomalize(T1* pWeight, int nOverlapData)
{
	if(pWeight==NULL)
		return false;

	T1 sumOfWeight = 0;
	Accumulate( pWeight, nOverlapData, sumOfWeight );

	if(sumOfWeight<=0)
		return false;

	for(int i=0; i<nOverlapData; i++)
	{
		pWeight[i] /= sumOfWeight;
	}

	return true;
}

// ���ɸ�˹����,��ֵΪ0������Ϊ1
float GenerateGaussNoise();