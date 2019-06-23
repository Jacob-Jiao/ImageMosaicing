#pragma once

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "Bitmap.h"

#if defined (_WINDLL) || defined(_CONSOLE) || defined(_WINDOWS)
	#ifndef MV_MODULE_TYPE2
		#define MV_MODULE_TYPE2 _declspec(dllexport)
	#else
		#define MV_MODULE_TYPE2 _declspec(dllimport)
	#endif
#else
	#define MV_MODULE_TYPE2 
#endif

struct MosaicImageInfo
{
	IplImage* pSrc;
	ProjectMat T;
	int success;
	int emergency;
	MosaicImageInfo()
	{
		pSrc = NULL;
		success = 0;
		emergency = 0;
	}
};


// ͼ��ƴ�Ӳ���
struct MosaicParams
{
	// ���ͽ�����ͼ��������mergeImagesNumʱ, ƴ��һ����ͼ
	int mergeImagesNum; 

	// �����쳣���ʱ, ��ǰ���ڽ���imagesNumEmergency��ͼƬƴ��һ��ͼ��
	int imagesNumEmergency; 

	// ������������
	int maxFeatruesNum;

	// ƥ�䴰�ڵİ뾶
	int winR; 

	// �޶������뾶
	int searchR;

	// ��Сƥ��÷�
	float minScore;

	// ransac������ֵ
	float ransacDist;

	// ͼ���ںϷ���,0��������, 1���Լ�Ȩ�ں�, 2������˹�������ں�
	int blendingType;

	// �ο���׼, 0�Ե�һ��ͼ��Ϊ��׼, 1�����Զ�ѡ���׼
	int referenceType;

	int wResDynamic;

	int hResDynamic;

	MosaicParams()
	{
		this->mergeImagesNum = 10; // 
		this->imagesNumEmergency = 15;//
		this->maxFeatruesNum = 400;
		this->winR = 13;
		this->searchR = -1;
		this->minScore = 0.7f;
		this->ransacDist = 2.0f;
		this->blendingType = 1;
		this->referenceType = 1;
		this->wResDynamic = 768;
		this->hResDynamic = 1024;
	}
};

class MV_MODULE_TYPE2 CMosaic
{
public:
	// ����ƴ�Ӳ���
	void SetParams(MosaicParams param);

	// ��ն�̬ƴ�ӳ��е�����
	void ClearPool();

	//******************************************************************************
	// ���ܣ�ͼ��ƴ��, ʵʱ����Ч��, ÿ��һ��ͼ�񶼸���ƴ��ͼ
	// ����ֵ����: 0ƴ�ӳɹ�, -1�����������, -2ƴ��ʧ��
	// pSrcΪ��ƴ�ӵ�ͼ��
	//******************************************************************************
	int MosaicDynamic(const IplImage* pSrc);

	bool UpdateResult() const { return m_bUpdateResult; }
	void UpdateResult(bool val) { m_bUpdateResult = val; }

	// New
	IplImage* GetDynamicMosaicResult(bool bExternal); // �ⲿ����ʱ, bExternal��true
public:
	//******************************************************************************
	// ����: ͼ��ƴ��, ��һ��ƴһ��, �Ƕ���Ч��, ��һ��ʱ��ƴ��һ�Ŵ�ͼ
	// ����ֵ����: 0ƴ�ӳɹ�, -1�����������, -2ƴ��ʧ��
	// pSrcΪ��ƴ�ӵ�ͼ��
	// pMosaicResultΪƴ�Ӻ��ͼ��, ���������ⲿ�ͷ�!
	// emergencyΪtrueʱ, ����ƴ��һ��ͼ��
	//******************************************************************************
	int Mosaic(const IplImage* pSrc, IplImage* &pMosaicResult, const bool emergency);

	IplImage* GetMosaicResult(bool bExternal); // �ⲿ����ʱ, bExternal��true
	
	//******************************************************************************
	// ����: �첽��ʽ��ͼ��ƴ��
	// ����ֵ����: 0ƴ�ӳɹ�, -1�����������, -2ƴ��ʧ��
	// pSrcΪ��ƴ�ӵ�ͼ��
	// pMosaicResultΪƴ�Ӻ��ͼ��, ���������ⲿ�ͷ�!
	// emergencyΪtrueʱ, ����ƴ��һ��ͼ��
	//******************************************************************************
	int MosaicAsynchronous(const IplImage* pSrc);//, bool bNewTask);

	// ���ý���ƴ���̱߳�־
	void EndDynamicMosaic(bool val) { m_bEndDynamicMosaic = val; }
	// ��ѯ����ƴ���̱߳�־
	bool EndDynamicMosaic() const { return m_bEndDynamicMosaic; }

	CMosaic();

	~CMosaic();
private:
	// ���ܣ����ɶ�̬ƴ��ͼ
	int GenerateDynamicPana(const IplImage* pImages[], int imagesNum, ProjectMat* pImgT);

	// ������Դ
	void Destroy();

private:
	// ƴ�Ӳ���
	MosaicParams m_params;

	bool m_bEndDynamicMosaic;
private:
	int m_nCorner1, m_nCorner2;
	IplImage* m_pGray1, *m_pGray2;

private:
	// ƴ�ӵĽ��
	IplImage* m_pMosaicResult;

private:
	// ƴ��ͼ����±�־, Ϊtrueʱ, ���Զ�ȡm_pMosaicResult
	bool m_bUpdateResult;

private:
	// �������һ��ͼ��ĵ�Ӧ�任����
	float m_homoToPrev[9];

	pool::IntPoint leftUpPt;

	float* HomoToPrev() { return m_homoToPrev; }
};

void CvImage2BitmapImage(IplImage* pSrc, pool::BitmapImage* pDst);

// ���ܣ�ƴ�Ӷ��ͼƬ
// ԭ��Harris�ǵ� + NCC + RANSAC + ��Ӧ����
// ����ֵ��ƴ�ӵĽ����ͼָ��;  ���ΪNULL����ʾƴ��ʧ��; 
// BitmapImage* pImages[], -----�����ͼ��ָ������
// int imagesNum, --------------��ƴ�ӵ�ͼ��֡��
// int errorCode---------------------������, ���, 0��ʾ�������û���쳣, -1��ʾ����Ĳ�������
// int maxFeatruesNum, ---------���������(Ĭ�ϸ�400)
// int winR, -------------------NCC���ڴ�С(Ĭ�ϸ�15)
// float minScore,--------------NCC��Сƥ��÷�(Ĭ�ϸ�0.7)
// float ransacDist/**/,--------RANSAC��ֵ(Ĭ�ϸ�2 pixel)
// int searchR = -1/**/---------�޶�����ƥ���������Χ�������ȷ������-1��
extern "C" __declspec(dllexport)
IplImage* MosaicMultiImages2(IplImage* pImages[], int imagesNum, 
									 int &errorCode,
									 int maxFeatruesNum, int winR, 
									 float minScore,
									 float ransacDist/**/,
									 int searchR /**/);