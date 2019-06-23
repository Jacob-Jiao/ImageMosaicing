//#include "stdafx.h"

#include "ImageIO.h"

#include "Bitmap.h"
#include "MemoryPool.h"
#include <fstream>
#include <direct.h>  
#include <io.h>
#include <Windows.h>

// �ڴ��
CMemoryPool s_memPool;

bool CreateFolder(string   strFolderPath /*= _T("c:\\test")*/)
{
	_mkdir(strFolderPath.c_str());

	return true;
}

// ��ȡϵͳʱ�� ��-��-��-ʱ-��-��
int GetSystemTime(string &strTime)
{
	char chTime[200];

	SYSTEMTIME sys;    

	GetLocalTime( &sys );    

	sprintf_s( chTime,"%4d-%02d-%02d %02d-%02d-%02d %03d",
		sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, sys.wMilliseconds);

	strTime=chTime;

	return 1;
}

// ��ȡͼ��ĸߡ���
int GetImageSize(const pool::BitmapImage *imSrc, 
				 int &w,int &h,int &ws)
{

	if(imSrc==NULL) 
	{
		return 0;
	}

	w = imSrc->width;
	h = imSrc->height;
	ws = imSrc->widthStep;

	return 1;
}


// �·������ڴ�
pool::BitmapImage* CreateBitmap8U( int width, int height, int nChannels)
{
	if( ( width<=0 ) || ( height<=0 ) || ( nChannels<=0 ) )
		return false;

	//pool::BitmapImage* pDstImage = (pool::BitmapImage*)s_memPool.New( sizeof(BitmapImage) );
	pool::BitmapImage* pDstImage = new BitmapImage();
		 

	pDstImage->width = width;
	pDstImage->height = height;
	pDstImage->widthStep = (width*nChannels + 3)/4*4;
	pDstImage->nChannels = nChannels;

	//pDstImage->imageData = (unsigned char*)s_memPool.New(pDstImage->widthStep*pDstImage->height);
	pDstImage->imageData = new unsigned char[pDstImage->widthStep*pDstImage->height];

	return pDstImage;
}

// 
void ReleaseBitmap8U(pool::BitmapImage* &pDstImage)
{
	if(pDstImage==NULL)
		return;

	//s_memPool.Delete( pDstImage->imageData );
	delete[] pDstImage->imageData; pDstImage->imageData = NULL;

	//s_memPool.Delete( pDstImage );
	delete pDstImage; 
	
	pDstImage = NULL;
}


// �·������ڴ�Ŀ���
pool::BitmapImage* CloneBitmap8U(const pool::BitmapImage* pSrcImage )
{
	if(pSrcImage==NULL)
		return false;

	//pool::BitmapImage* pDstImage = (pool::BitmapImage*)s_memPool.New( sizeof(BitmapImage) );
	pool::BitmapImage* pDstImage = new BitmapImage();

	pDstImage->width = pSrcImage->width;
	pDstImage->height = pSrcImage->height;
	pDstImage->widthStep = pSrcImage->widthStep;
	pDstImage->nChannels = pSrcImage->nChannels;

	//pDstImage->imageData = (unsigned char*)s_memPool.New(pDstImage->widthStep*pDstImage->height);
	pDstImage->imageData = new unsigned char[pDstImage->widthStep*pDstImage->height];

	memcpy( pDstImage->imageData, pSrcImage->imageData, pSrcImage->widthStep*pSrcImage->height );

	return pDstImage;
}

pool::BitmapImage* CutPatchImage(const pool::BitmapImage *src,
								 const InterestRegion2 &roi)
{
	pool::IntPoint startp, endp;
	startp.x = roi.xbeg;
	startp.y = roi.ybeg;
	endp.x = roi.xend;
	endp.y = roi.yend;

	return CutPatchImage(src, startp, endp);
}

// �������ܣ���ͼƬ�н�ȡͼƬ���������� �Խǵ�
// �·������ڴ�
pool::BitmapImage* CutPatchImage(const pool::BitmapImage *src,
				   pool::IntPoint startp, pool::IntPoint endp)
{
	if(src==NULL)
	{ 
		std::cout<<"Error:Input source image wrong in CutPatchImage()!"<<std::endl;
		return NULL;
	}

	int width = 0, height = 0, widthstep = 0;
	GetImageSize( src, width, height, widthstep );

	// �жϣ���ֹ����
	if( (startp.y<0) || (startp.y>height-1) )
		return false;
	if( (startp.x<0) || (startp.x>width-1) ) 
		return false;
	if( (endp.y>height-1) || (endp.y<0) ) 
		return false;
	if( (endp.x>width-1) || (endp.x<0) )
		return false;
	// �߽��жϽ���......

	int newHeight = 0, newWidth = 0;
	newHeight = endp.y-startp.y+1;
	newWidth = endp.x-startp.x+1;

	if( (newWidth<=0) || (newHeight<=0) ) 
		return false;

	int nChannels = src->nChannels;

	pool::BitmapImage* dst = NULL;
	dst = CreateBitmap8U( newWidth, newHeight, src->nChannels );

	if(dst->imageData==NULL)
		return false;

	int newwidthstep = dst->widthStep;

	int uStart,uEnd;
	int vStart,vEnd;
	uStart = startp.y;
	uEnd = endp.y;
	vStart=startp.x;
	vEnd=endp.x;

	if(nChannels==1)
	{
		int i = 0;
		for(int u=uStart; u<=uEnd; u++) 
		{
			unsigned char* pDstRow = dst->imageData + i*newwidthstep;
			unsigned char* pSrcRow = src->imageData + u*widthstep;
			int j=0;
			for(int v=vStart; v<=vEnd; v++) 
			{
				*(pDstRow + j)=(unsigned char)*(pSrcRow + v);
				j++;			
			}
			i++;
		}
	}
	else if(nChannels==3)
	{
		int i = 0;
		for(int u=uStart; u<=uEnd; u++) 
		{
			unsigned char* pDstRow = dst->imageData + i * newwidthstep;
			unsigned char* pSrcRow = src->imageData + u * widthstep;
			int j=0;
			for(int v=vStart; v<=vEnd; v++) 
			{
				*(pDstRow + 3*j)=(unsigned char)*(pSrcRow + 3*v);
				*(pDstRow + 3*j + 1)=(unsigned char)*(pSrcRow + 3*v + 1);
				*(pDstRow + 3*j + 2)=(unsigned char)*(pSrcRow + 3*v + 2);
				j++;			
			}
			i++;
		}
	}

	return dst;
}

// �������ܣ�д���ݵ�ͼ���е�ĳ����������   
// �Խǵ㣺startp   endp
int WriteBackToImageRegion(pool::BitmapImage *src, pool::BitmapImage *seg, 
						   const InterestRegion2 &roi)
{
	pool::IntPoint startp, endp;
	startp.x = roi.xbeg;
	startp.y = roi.ybeg;
	endp.x = roi.xend;
	endp.y = roi.yend;

	return WriteBackToImageRegion(src, seg, startp, endp);
}

// �������ܣ�д���ݵ�ͼ���е�ĳ����������   
// �Խǵ㣺startp   endp
int WriteBackToImageRegion(pool::BitmapImage *src, pool::BitmapImage *seg, 
						   pool::IntPoint startp, pool::IntPoint endp)
{
	if( (src==NULL) || (seg==NULL) )
	{
		return 0;
	}

	int width = 0, height = 0, widthstep = 0;
	GetImageSize( src, width, height, widthstep );

	int widthSeg = 0,heightSeg = 0, widthStepSeg = 0;
	GetImageSize( seg, widthSeg, heightSeg, widthStepSeg );

	// �߽��ж�
	if( (startp.x<0) || (startp.x>width-1) || (startp.y<0) || (startp.y>height-1) )
	{
		startp.x=0;
	}
	if( (endp.y<0) || (endp.y>height-1) || (endp.x<0) || (endp.x>width-1) )
	{
		endp.y = 0;
	}
	// �߽紦�����......

	if(src->nChannels==1)
	{
		int i=0;
		for(int u=startp.y;u<=endp.y;u++) 
		{
			int j=0;
			unsigned char *pSrcRow = src->imageData + u*widthstep;
			unsigned char *pSecRow = seg->imageData + i*widthStepSeg;
			for(int v=startp.x;v<=endp.x;v++) 
			{
				*(pSrcRow + v) = *(pSecRow + j);	
				j++;
			}
			i++;
		}
	}
	else if(src->nChannels==3)
	{
	}

	return 1;
}

//��ʾͼƬ
int ShowImage(pool::BitmapImage* pSrc,char win_name[])
{

	if(pSrc==NULL) 
	{
		return 0;
	}

#ifdef _DEBUG
	//int w, h, ws;
	//GetImageSize( pSrc, w, h, ws );

	//IplImage* pSrcShow = NULL;
	//pSrcShow = cvCreateImage( cvSize(w,h), 8, 1 );
	//memcpy( (unsigned char*)pSrcShow->imageData, pSrc->imageData, ws*h );

	//cvNamedWindow( win_name, CV_WINDOW_AUTOSIZE );
	////cvNamedWindow( win_name, 0 );
	//cvShowImage( win_name, pSrcShow );

	//cvReleaseImage(&pSrcShow);
#endif

	return 1;
}


// ��ȡraw8ͼ������
int ReadRaw8Image(char* path, IplImage* pImage)
{
	if(pImage==NULL)
		return -1;
	if(path==NULL)
		return -1;

	int width = pImage->width;
	int height = pImage->height;

	ifstream infile(path, ios::binary);
	if(!infile)
	{
		return -1;
	}

	infile.read((char*)pImage->imageData, width*height);
}

// ��12λ����תΪ16λ����
int ConvertData12To16(BYTE* pRaw12,BYTE* pRaw16,int w,int h)
{
		int size = w*h*2;

		int j = 0;
		int i = 0;

		while (j < size)
		{
			pRaw16[j++] = pRaw12[i];
			pRaw16[j++] = pRaw12[i+2] << 4;
			pRaw16[j++] = pRaw12[i + 1];
			pRaw16[j++] = pRaw12[i+2] & 0xf0;
			i+=3;
		}

	return 0;
}

// ��ȡraw12ͼ������
// pImage---16λͼ��
int ReadRaw16Image(char* path, unsigned short* pImage16, int width, int height)
{
	if(pImage16==NULL)
		return -1;
	if(path==NULL)
		return -1;

	ifstream infile(path, ios::binary);

	if(!infile)
	{
		return -1;
	}

	int size16 = width*height*2;
	
	infile.read((char*)pImage16, size16);

	for(int r=0; r<height; r++)
	{
		for(int c=0; c<width; c++)
		{
			unsigned short val = *(pImage16 + r*width + c);

			unsigned  short val1, val2;

			val1 = val<<4;
			val2 = val>>12;
			val = val1 | val2;

			val = val>>4;

			*(pImage16 + r*width + c) = val;
		}
	}

	return 0;
}

// ��ȡraw12ͼ������
// pImage---16λͼ�� ��4λ�Ѿ�����
int ReadRaw16ImageV2(char* path, unsigned short* pImage16, int width, int height)
{
	if(pImage16==NULL)
		return -1;
	if(path==NULL)
		return -1;

	ifstream infile(path, ios::binary);

	if(!infile)
	{
		return -1;
	}

	int size16 = width*height*2;

	infile.read((char*)pImage16, size16);

	for(int r=0; r<height; r++)
	{
		for(int c=0; c<width; c++)
		{
			unsigned short val = *(pImage16 + r*width + c);

			unsigned  short val1, val2;

			//val1 = val<<4;
			//val2 = val>>12;
			//val = val1 | val2;

			//val = val>>4;

			val = val>>4;

			*(pImage16 + r*width + c) = val;
		}
	}

	return 0;
}