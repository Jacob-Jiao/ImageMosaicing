

// ͼ�񼸺�
#pragma once
#include "Bitmap.h"
#include "usingCV24.h"

struct ScaledImage
{
	pool::BitmapImage* pImage; // ��ǰͼ��ָ��
	float scale; // ��ǰͼ���Ӧ�ĳ߶�
	int validWidth;  // ��ǰͼ�����Ч���
};
 
//���ܣ�Χ��������ת
pool::BitmapImage* RotateImageCenter2( pool::BitmapImage *pSrc, const float fAngle);

//���ܣ�Χ��������ת (���ξ��) 
// Yuhua Xu, 2016-11-10
IplImage* RotateImageCenterCubic( IplImage *pSrc, const float fAngle);

IplImage* RotateImageCenterBilinear( IplImage *pSrc, const float fAngle);

// ���ξ����ֵ����
float S3Fun(float x);

// ˫���Բ�ֵ
bool Resize(pool::BitmapImage *pSrcImage, pool::BitmapImage *pDstImage);

// �������ܣ���ת ͼ�����������ƫ��
int RotateImageOffsetCenter(pool::BitmapImage *src, float angle, float &duR, float &dvR);

// ����ͼ�������
int CreateImagesPyramid(const pool::BitmapImage* pSrcImage, int dwPyramidLevel,
						pool::BitmapImage* apPyramidImages[10]);

// ������˹������
int BuildGaussPyramid(const pool::BitmapImage* pSrcImage, int dwPyramidLevel, float sigma, 
					  pool::BitmapImage* pGaussPyramid[10]);

// ����������˹������
int BuildLaplacianPyramid(const pool::BitmapImage* pSrcImage, const int pyramidLevel, const float sigma, 
						  pool::BitmapImage* apPyramidImages[10]);

bool ReleasePyramid(pool::BitmapImage* apPyramidImages[], int nLevel);

// ͼ��ƽ��2011-01-17
bool Translation( const pool::BitmapImage*  pSrcImage,  pool::BitmapImage* _OUT pDstImage, 
				  const float  deltaX,  const float  deltaY);

// ͼ��ƽ��2011-01-17
bool Translation( const IplImage*  pSrcImage, IplImage* _OUT pDstImage, 
				 const float  deltaX,  const float  deltaY);

// ͼ��ƽ��x
bool TranslationX( const IplImage*  pSrcImage, IplImage* _OUT pDstImage, 
				  const float  deltaX);

// ���²���, 2���߶�
bool DownSampleByScaleTwo(const pool::BitmapImage* pSrc, pool::BitmapImage*pDst);

// mirror, 0ΪX������, 1ΪY������
bool ImageMirror( const pool::BitmapImage* pSrcImage, pool::BitmapImage* pDstImage, int mirrorType);

// mirror, 0ΪX������, 1ΪY������
bool ImageMirror( pool::BitmapImage* pSrcImage, int mirrorType);

// �Ҷ�ͼת��ɫͼ
bool GrayToColor(const pool::BitmapImage* pSrcGray, pool::BitmapImage* pDstColor);

// �������ܣ���ͼ���������, ͼ��ĳߴ粻��
// �任ǰ�����ĵ㲻��
bool ScalingImage(pool::BitmapImage* pSrc, pool::BitmapImage* pDst, float scale);

// �������ܣ��Խ�����������ͼ���������
// pImagePyramid---ͼ��Ľ�����
// nScaledImages---�߶ȱ任���ͼ��ĸ���
// nPyramid--------����Ҫ����3
bool ScalingImage(pool::BitmapImage* pImagePyramid[], int nPyramid, 
				  float minScale, float maxScale,
				  ScaledImage** &pScaledImages, int &nScaledImages);

// �������ܣ�����ԭ�ߴ�ͼ���polar-image, ���ɳ߶ȿռ��е�������polar-image
bool ScalingPolarImage(pool::BitmapImage* pSrcPolar, 
					   float minScale, float maxScale,
					   ScaledImage** &pScaledImages, int &nScaledImages);

// ��ͼ����ת180��
pool::BitmapImage* Rotate180(pool::BitmapImage* pSrc);

// ��ͼ��������ת90��
// 
int Rotate90Right(IplImage* pSrc, IplImage* pDst);

// ��ͼ��������ת90��
// y = x2
// x = w-1-y2
int Rotate90Left(IplImage* pSrc, IplImage* pDst);

// ����任(��ͼ�������Ϊ�任����)
bool AffineTransform(const pool::BitmapImage* pSrc, pool::BitmapImage* dst, 
					 float a, float b, float c, float d);

// �������ܣ���ͼƬ�н�ȡͼƬ���������� �Խǵ�
// �·������ڴ�
IplImage* CutPatchImage(IplImage* src,
						pool::IntPoint startp, pool::IntPoint endp);

int WriteBackToImageRegion(IplImage* segPatch, IplImage* dst, int shiftX, int shiftY);

// �������ε������߼������ǵ������Ƕ�
void AngleFrom3Edges(double a, double b, double c,
					   double &A, double &B, double &C);

// ��תƽ�� 
// ��ƽ�������ת��ʹ����X0Yƽ��ƽ��
// ƽ��Ϊax + by + cz + d = 0;
// AZΪ��Z����ת�ĽǶ�
// AYΪ��Y����ת�ĽǶ�
// ����Z����ת, ����Y����ת
int RotatePlane(double a, double b, double c, 
				   double &AZ, double &AY);

// ����Zת, ��Y, ���X
void GetR_ZYX(double ax, double ay, double az, double R[9]);

int Top2Down(IplImage* src);