/* ***********************************************************
Xu Yuhua, 2012-07-08
**************************************************************/
#pragma once
#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "Bitmap.h"
#include "Mosaic1By1.h"
//#include "atlstr.h"


using namespace pool;

struct CutInfo // ͼ��ü���Ϣ
{
	int cutL, cutR, cutU, cutD; // ��ߵĲü���, �ұߵĲü���,�ϱߵĲü���, �±ߵĲü���

	CutInfo(int cutL, int cutR, int cutU, int cutD)
	{
		this->cutL = cutL;
		this->cutR = cutR;
		this->cutU = cutU;
		this->cutD = cutD;
	}

	CutInfo()
	{
		cutL = 20;
		cutR = 25;
		cutU = 20;
		cutD = 100;
	}
};

struct FilePath
{
	char path[200];
};

// ����ڲ�
struct IntrinsicParams
{
	int width, height;
	double cx, cy;	// ����
	double fx, fy;	// ��Ч����
	double k[9];		// ����ϵ��
	double B;
	IntrinsicParams()
	{
		cx = 0;
		cy = 0;
		fx = 0;
		fy = 0;
	}
};

struct UavMatchParam
{
	IntrinsicParams cam; // ����ڲ�

	int minHessian;
	int maxFeatruesNum;
	float matchDist;
	float ransacDist;
	int blending; // 0���ں� 1��Ȩ�ں� 2��Ƶ���ں�
	int downViewConstraint; // 1ʹ��������Լ�� 0��ʹ��������Լ��
	int loadMatchPairs;

	UavMatchParam()
	{
		minHessian = 50;
		maxFeatruesNum = 200;
		matchDist = 0.5f;
		ransacDist = 2.5f;
		blending = 2;
		downViewConstraint = 1;
		loadMatchPairs = 0;
	}
};

struct GPS_Info
{
	int valid; // GPS�����Ƿ���Ч��� 0Ϊ��Ч 1Ϊ��Ч 

	double ax; // ������ ��
	double ay; // ������
	double az; // ƫ���Ƕ�

	double H1; // ���θ߶�
	double H2; // ������߶�
	double B; // γ��
	double L; // ����

	GPS_Info()
	{
		valid = 0;
	}
};

struct GPS_Data
{
	int valid; // GPS�����Ƿ���Ч��� 0Ϊ��Ч 1Ϊ��Ч 

	double ax; // ������
	double ay; // ������
	double az; // ƫ���Ƕ�

	double h; // �߶�
	double x; // γ��
	double y; // ����

	GPS_Data()
	{
		valid = 0;
	}
};

struct Segment
{
	int beg;
	int end;
	Segment(int beg, int end)
	{
		this->beg = beg;
		this->end = end;
	}
	Segment()
	{
		beg = 0;
		end = 0;
	}
};

// ƥ��ĵ��
struct MatchPointPairs
{
	// ��A
	SfPoint ptA;	
	// ��A���ڵ�ͼ������
	int ptA_i;
	// ��A�Ƿ��ǹ̶���
	int ptA_Fixed;

	// ��B
	SfPoint ptB;
	// ��B���ڵ�ͼ������
	int ptB_i;
	// ��B�Ƿ��ǹ̶���
	int ptB_Fixed;

	// ���������ı��
	//int wpIdx; // world Point Index
};

// ƥ��ĵ��
struct MatchPointPairs2
{
	// ��A
	SfPoint ptA;	
	// ��A���ڵ�ͼ������
	int ptA_i;
	// ��A�Ƿ��ǹ̶���
	int ptA_Fixed;

	// ��B
	SfPoint ptB;
	// ��B���ڵ�ͼ������
	int ptB_i;
	// ��B�Ƿ��ǹ̶���
	int ptB_Fixed;

	// ���������ı��
	int wpIdx; // world Point Index

	MatchPointPairs2()
	{
		wpIdx = -1;
	}
};

struct ImagePoint
{
	float x;
	float y;
	int imIdx;
	int id;
};

// �ؽ��ĵ�
struct ReconstructedPoint
{
	float x; // ����������ϵ�е�����x
	float y; // ����������ϵ�е�����y
	float z;
	int idx;
	int fixed;
	vector<ImagePoint> imgPt; // �õ����ܹ۲⵽����ͼ���е����� (���ܻᱻ���ͼ��۲⵽)

	ReconstructedPoint()
	{
		idx = -1;
		fixed = 0;
	}
};

// ƥ��ĵ��
struct MatchPointPairs_CV32F
{
	// ��A
	CvPoint3D32f ptA;	
	// ��A���ڵ�ͼ������
	int ptA_i;
	// ��A�Ƿ��ǹ̶���
	int ptA_Fixed;

	// ��B
	CvPoint3D32f ptB;
	// ��B���ڵ�ͼ������
	int ptB_i;
	// ��B�Ƿ��ǹ̶���
	int ptB_Fixed;
};

struct ImageTransform
{
	ProjectMat h;
	int fixed;
};

struct MatchNeighbourError
{
	double error;
	int iImage;
	int nMatch;
	bool   operator <  (const MatchNeighbourError& match)  const   //��������ʱ����д�ĺ���
	{   
		return error<match.error;
	}
};

struct Translation32F
{
	float dx,dy;
	int fixed;
};

struct ImageTransform64F
{
	ProjectMat64F h;
	int fixed;
};

//// �����̬
//struct CameraPose
//{
//	// λ��
//	CvPoint3D32f pos;
//	// �Ƕ�
//	float AX, AY, AZ;
//	// ��̬�Ƿ���Ч, 1Ϊ��Ч
//	int isValid;
//	float R[9];
//	float T[3];
//
//};

// �����̬
struct CameraPose64F
{
	// λ��
	CvPoint3D64f pos;
	// �Ƕ�
	float AX, AY, AZ;
	// ��̬�Ƿ���Ч, 1Ϊ��Ч
	int isValid;
	double R[9]; // ��ת����
	double T[3]; // ƽ�ƾ���
	double k[2]; // ����ϵ��
	double F; // ��Ч����
};

// ͼ��������̬��Ϣ
struct ImagePoseInfo
{
	//ͼ��ָ��
	IplImage* pImg;

	//�����̬
	CameraPose64F camPose;

	int fixed;

	ImagePoseInfo()
	{
		fixed = 0;
	}
};

// �������ͼ��
struct RectifiedImage
{
	ProjectMat64F H;
	double imgCxPys,imgCyPys; // ͼ���������������(m)
	double imgCxPix,imgCyPix; // ͼ�������ͼ������(pixel)
};

struct MatchEdge
{
	int idxImg1, idxImg2;
};

// ��֡��������
int TrackMultiFrames(vector<MatchPointPairs> &matched, int nImages);

// ʹ��bundler��������
// vecMatchPoints--------�������
int ImportBundlerOut(char* pPath, vector<CameraPose64F> &camPose, vector<CvPoint3D64f> &points,
					 vector<MatchPointPairs> &vecMatchPoints);

int ImportVisualSFM_Match(char* pPath, vector<MatchPointPairs> &vecMatchPoints, float cx, float cy);

int ImportVisualSFM_Match2(char* pPath, vector<MatchPointPairs> &vecMatchPoints, float cx, float cy);

// ʹ��cmvs�������� wu-changchang
int ImportCMVSOut(char* pPath, vector<CameraPose64F> &camPose, vector<CvPoint3D64f> &points,
				  vector<MatchPointPairs> &vecMatchPoints);

// ��ȡlist�е�ͼ�������
int ReadImageList(char* pPath, vector<string> &vecList);

template<class T1, class T2, class T3>
void ApplyProject9(const T1 h[9], const T2 xSrc, const T2 ySrc, T3 &xDst, T3 &yDst)
{
	xDst = (h[0]*xSrc + h[1]*ySrc + h[2]) / (h[6]*xSrc + h[7]*ySrc + h[8]);
	yDst = (h[3]*xSrc + h[4]*ySrc + h[5]) / (h[6]*xSrc + h[7]*ySrc + h[8]);
}

// ��Ӧ���� + ƽ��(dx,dy)
template<class T1>
void HomographyPlusTranslation(T1 H[9], T1 dx, T1 dy)
{
	H[0] = H[0] + dx * H[6];
	H[1] = H[1] + dx * H[7];
	H[2] = H[2] + dx * H[8];

	H[3] = H[3] + dy * H[6];
	H[4] = H[4] + dy * H[7];
	H[5] = H[5] + dy * H[8];

	//H[6] = H[6];
	//H[7] = H[7];
	//H[8] = H[8];
}

// �����������̬��ͼ�����ƴ��
class CMosaicByPose
{
public:
	CMosaicByPose();

	~CMosaicByPose();

private:
	// ����ڲ�
	IntrinsicParams m_inParams;
public:

	// Bundler's out
	// �������������ÿ��ͼ��ĵ�Ӧ�任����
	// �����ڽ���ƽ�泡����ͼ��ƴ��
	int CamPose2Homo3(ImagePoseInfo *pImagePose, int nImages, vector<CvPoint3D64f> &vecPoints, 
		vector<ProjectMat64F> &homos, vector<MatchPointPairs> vecMatchPairs);

	// û��λ����Ϣ��ͼ��ƥ��
	int MosaicWithoutPose(ImagePoseInfo *pImgPoses, const int nImages, int &numMosaiced);


	// ������SURFͼ���������Ӧ
	// ֡��ƥ��
	int GetMatchedPairsSingleStripSurf(const ImagePoseInfo *pImgPoses, const int nImages, 
										vector<MatchPointPairs> &vecMatchPairs,
										int &nSuccess);

	// ����ƥ��
	// û��λ��������Ϣ
	// one-to-all
	int GetMatchedPairsOneToAllSurf(const ImagePoseInfo *pImgPoses, const int nImages, 
									vector<MatchPointPairs> &vecMatchPairs,
									int &nSuccess);

	int GetMatchedPairsOneToAllSIFT_MultiThread();

	// ������ͼ���������Ӧ
	int GetMatchedPairsSingleStrip(const ImagePoseInfo *pImgPoses, const int nImages, 
										vector<MatchPointPairs> &vecMatchPairs, int &nFixedImages);
	
	// �ຽ��ͼ���������Ӧ
	int GetMatchedPairsMultiStrip(const ImagePoseInfo *pImgPoses, const int nImages, 
										vector<MatchPointPairs> &vecMatchPairs, int &nFixedImages);

	int GetMatchedPairsMultiStripSurf(const ImagePoseInfo *pImgPoses, const int nImages, 
										vector<MatchPointPairs> &vecMatchPairs, int &nFixedImages);

	// ������ͼ����зֶ�
	int SegmentSequenceImages(ImagePoseInfo* pImagePoses, int nImages, 
								RectifiedImage* pRectified, vector<Segment> &vecSegments);

	// ��ÿ��ͼ�����ƴ�Ӽ���
	int MosaicBySegment(ImagePoseInfo *pImgPoses, const int nImages, ImageTransform* pImageTransformInit,
						vector<ImageTransform> &vecImageTransformRefined);
private:
	double m_heightNorm;// �淶������߶�
public:
	double HeightNorm() const { return m_heightNorm; }
	void HeightNorm(double val) { m_heightNorm = val; }
public:
	IntrinsicParams InParams() const { return m_inParams; }
	void InParams(IntrinsicParams val) { m_inParams = val; }

public:

	// �����������һ�ε����任����
	int AdjustByFeaturePairs(ImagePoseInfo *pImgPoses, 
				vector<ImageTransform> &vecTrans, const vector<MatchPointPairs> &vecMatchedPoints, int nImages, int adjust);

	int Mosaic(ImagePoseInfo *pImages, int nImage);

	int Mosaic2( ImagePoseInfo *pImgPoses, int nImages, vector<MatchPointPairs> vecMatchedPoints);

public:
	// ��GPS��Ϣ��ͼ����׼, ����ͼ��ƴ��
	int MosaicByPose_Image(ImagePoseInfo *pImgPoses, const int nImages);

	int MosaicByBundler(ImagePoseInfo *pImgPoses, const int nImages, 
		vector<CvPoint3D64f> &vecPoints, vector<MatchPointPairs> vecMatchPairs);

private:
	// ������У��
	int RectifyImage2(const ImagePoseInfo *pImagePose, ProjectMat64F &H, double &imgCxPix, double &imgCyPix);

	// ������У��
	int RectifyImage3(const ImagePoseInfo *pImagePose, ProjectMat64F &H, double &imgCxPix, double &imgCyPix);

	// ͼ����Ƕ
	int MosaicImagesByPoses(const ImagePoseInfo *pImgPoses, const int nImages, const RectifiedImage* pRectified);

	// �������̬�õ�ÿ��ͼ���ƴ�ӱ任����
	int MosaicImagesByPoses(const ImagePoseInfo *pImgPoses, const int nImages, const RectifiedImage* pRectified,
							vector<ImageTransform> &vecTrans);

	int MosaicImagesRefined(const ImagePoseInfo *pImgPoses, const int nImages, const ImageTransform* pRectified);

	int MosaicImagesRefined(const ImagePoseInfo *pImgPoses, const int nImages, const ImageTransform64F* pRectified);

	int MergeImagesRefined(ImagePoseInfo *pImgPoses, const int nImages, const ImageTransform* pRectified);

private:
	// ƴ�ӽ��ͼ
	IplImage* m_pMosaicResult;
public:
	IplImage* GetMosaicResult() const { return m_pMosaicResult; }
	
private:
	// ����GPS�͹ߵ����ݼ���ͼ��ı任����
	int CalImgTransformByPosIMU(const ImagePoseInfo *pImgPoses, const int nImages, const RectifiedImage* pRectified,
								vector<ProjectMat> &vecImagesTranform);
	
	// ��ͼ����׼�ķ���������GPS/IMU����õ���ͼ��任����
	int AdjustmentByImage(const ImagePoseInfo *pImgPoses, const int nImages, ImageTransform* pTransformInit,
						vector<ImageTransform> &vecTransformRefined);

	// QuadraticProgram����
	int QuadraticProgram(const ImagePoseInfo *pImgPoses,
						MatchPointPairs* pMatchPairs, int nPairs,
						const ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);

	// ��������
	int BundleAdjustment(MatchPointPairs* pMatchPairs, int nPairs,
						const ImageTransform* pImagesTransform0, int nImages,
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);

	// ������������
	int BundleAdjustmentSparse(MatchPointPairs* pMatchPairs, int nPairs,
						const ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);

	// ��������---ƽ��ģ��
	int SBA_Translation(MatchPointPairs* pMatchPairs, int nPairs,
						const Translation32F* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<Translation32F> &vecTransformRefined);

	// ��Լ����������������
	int BundleAdjustmentSparseConstraint(MatchPointPairs* pMatchPairs, int nPairs,
						const ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);

	// ��rotԼ���ķ�������������
	// wRotΪrotԼ����Ȩ��
	int SparseAffineRotConstraint(MatchPointPairs* pMatchPairs, int nPairs,
						const ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages, float weight, 
						vector<ImageTransform> &vecTransformRefined);

	// ������ϡ���������� + rotԼ�� (�ο�ͼ��Ĳ��������Ż�)
	int SparseNonlinearRotConstraint(MatchPointPairs* pMatchPairs, int nPairs,
						ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages, float weight,
						vector<ImageTransform> &vecTransformRefined,
						int label[10000]);

	// ������ϡ���������� + rotԼ�� - v2 (�ο�ͼ��Ĳ���Ҳ���Ż�)
	int SparseNonlinearRotConstraint_V2(MatchPointPairs* pMatchPairs, int nPairs,
						ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages, int refImgIdx0,
						int weight,
						vector<ImageTransform> &vecTransformRefined,
						int label[10000]);

	// Lagrange����
	int BundleAdjustmentLagrange(const ImagePoseInfo *pImgPoses,
						MatchPointPairs* pMatchPairs, int nPairs,
						const ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);

	int BundleAdjustmentFixPoint(MatchPointPairs* pMatchPairs, int nPairs,
								const ImageTransform* pImagesTransform0, int nImages, 
								int nFixedImages,
								vector<ImageTransform> &vecTransformRefined);

	// �̶���ͼ������ĵ㲻��, ����4�����ɶȿɱ�, �̶������ĵ�, ֱ�Ӽ������A
	int BundleAdjustmentLS(const ImagePoseInfo *pImgPoses,
						MatchPointPairs* pMatchPairs, int nPairs,
						const ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);

	// ��������������
	int BundleAdjustmentNonlinear(MatchPointPairs* pMatchPairs, int nPairs,
						ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);

	// ������ϡ����������
	int BANonlinearSparse(MatchPointPairs* pMatchPairs, int nPairs,
						ImageTransform* pImagesTransform0, int nImages, 
						int nFixedImages,
						vector<ImageTransform> &vecTransformRefined);


private:
	CMosaic mosaic;

	vector<RectifiedImage> vecRectified;

public:
	int m_blending;
	int m_maxFeatureNum;
	int m_minHessian;
	float m_matchDist;
	float m_ransacDist;
	int m_downViewConstraint; // ʹ��������Լ��
	int m_fLoadMatchPairs;
	float m_scale;

	ImagePoseInfo *m_pImgPoses;
	
	int m_nImages;

	int m_nMatchThread;

	int m_idxMatchThread;

	int m_nMatchThreadOver; // ������ȡ��ƥ�䵱ǰ��״̬(����˼����߳�)

	int m_width;
	
	int m_height;

	void MatchThreadOver_Plus();

private:
	CMutexLock m_lock;

private:
	vector<MatchPointPairs> m_vecMatchPairs;
public:
	void PushMatchPairs(const vector<MatchPointPairs> &src);
};

// ��AVI��Ƶת����BMP
int ConvertVideo2Bmp(char* pVideoPath, 
					 int imageInterval, 
					 CutInfo cut,
					 vector<FilePath> &vecImagePathList,
					 int indexStartFrame, 
					 int indexEndFrame);

// ��AVI��Ƶת����BMP
int ConvertVideo2Bmp(char* pVideoPath, 
					 string pImagesDirectory,
					 int imageInterval, // ͼ����
					 CutInfo cut);

int GetInnerPoints(SfPoint* &pInner1, SfPoint* &pInner2, int &nInner);

// ��ʾ����ͼƥ���
int ShowMultiViewMatchPairs(ImagePoseInfo* pImgs, int nImages, const vector<MatchPointPairs> &vecPairs);

IplImage* ShowMatchPairs(IplImage* pImgA, IplImage* pImgB, const vector<MatchPointPairs> &vecPairs);

// Bundler's out
// �������������ÿ��ͼ��ĵ�Ӧ�任����
// �����ڽ���ƽ�泡����ͼ��ƴ��
int CamPose2Homo(const ImagePoseInfo *pImagePose, int nImages, const vector<CvPoint3D64f> &vecPoints, 
				 vector<ProjectMat64F> &homos);

// Bundler's out
// �������������ÿ��ͼ��ĵ�Ӧ�任����
// �����ڽ���ƽ�泡����ͼ��ƴ��
int CamPose2Homo2(const ImagePoseInfo *pImagePose, int nImages, const vector<CvPoint3D64f> &vecPoints, 
				  vector<ProjectMat64F> &homos);

// ���˻�ͼ��ƴ��ͼ��
// ����ֵ����, 0Ϊ�ɹ�, -1�����������, -2ƴ��ʧ��
// pImages------------ԭʼͼ��
// nImages------------ͼ�������
// pGpsDatas----------GPS����
// gpsValid-----------gps�����Ƿ���Ч��1-valid, 0-invalid
// pMosaicResult------ƴ�ӵĽ��, ���ڴ��ɳ����Զ�����, ���û��ͷ�
// numMosaiced--------ƴ�ӳɹ�ͼ����, ��Ȼ, numMosaiced<=nImages
extern "C" __declspec(dllexport)
int MosaicVavImages(IplImage** pImages, 
					int nImages, 
					GPS_Info *pGpsDatas, 
					int gpsValid, 
					UavMatchParam surfParam,
					IplImage* &pMosaicResult,
					int &numMosaiced,float scale);

// ����: AVI��Ƶ����ͼ��ƴ��
// pVideoPath---------------avi��Ƶ������ַ,  XXX.avi
// pGpsDataPath-------------gps��������·��,  XXX.txt, GPS��Ϣ��Чʱ, ��NULL
// mosaicParam--------------ͼ��ƴ�Ӳ���,
// maxOnceMosaicNum---------ÿ��ƴ����ദ���ͼ����, Ĭ��100
// imageInterval------------ͼ��������, Ĭ��5
// cut----------------------�ü���Ϣ, (��������AVI��Ƶȡ 20, 25, 20, 100)
// mosaicResultDirectory----ͼ��ƴ�ӽ������Ŀ¼, ʾ�� "c:\\temp"
// indexStartFrame----------ָ����ƴ����ʼ֡��, indexStartFrame>=0, �����ָ��, �븳-1
// indexEndFrame------------ָ����ƴ��β֡��, indexEndFrame<��Ƶ��֡��, �����ָ��, �븳-1
// �����ָ��ƴ�ӵ���ʼ֡�ͺ�β֡��, �����Ƶ�е�����֡����ƴ��
extern "C" __declspec(dllexport)
int MosaicUavVideo( char* pVideoPath, 
					char* pGpsDataPath,
					UavMatchParam mosaicParam,
					int maxOnceMosaicNum,
					int imageInterval,
					CutInfo cut,
					string mosaicResultDirectory,
					int indexStartFrame, 
					int indexEndFrame,
					float scale);