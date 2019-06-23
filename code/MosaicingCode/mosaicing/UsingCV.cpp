#pragma once

#include "usingCV24.h"

// ��ֱ��ͼ ��ɫ
void PlotHistogram2(float hist[256], char aName[200], const string &color )
{	
	if(hist==NULL) 
	{
		return ;
	}

	IplImage *src=NULL;
	src = cvCreateImage(cvSize(280,390), 8, 3);

	int u,v;
	int height=src->height;
	int width=src->width;

	int widthstep=src->widthStep;

	int red = 0, blue =0, green = 0;
	if(color=="red")
	{
		red = 255;
	}
	else if(color=="green")
	{
		green = 255;
	}
	else if(color=="blue")
	{
		blue = 255;
	}

	for(u=0;u<src->height;u++)
	{
		char* pDataRow=src->imageData + u*widthstep;
		for(v=0;v<src->width;v++)
		{
			*( pDataRow+3*v)=(char)255;
			*(pDataRow +3*v+1)=(char)255;
			*(pDataRow +3*v+2)=(char)255;
		}
	}

	int recHeight = 350;

	int cross_len;
	cross_len = 5;
	CvPoint pt1,pt2;
	float max=0;
	for(u=0;u<256;u++)
	{
		if(hist[u]>max)
		{
			max = hist[u];
		}
	}
	for(u=0; u<255; u++) 
	{
		pt1.x = u;
		pt1.y = 350-(int)( (float)300*hist[u]/max );

		//pt2.x = u;
		//pt2.y = 350;
		pt2.x = u+1;
		pt2.y = 350-(int)( (float)300*hist[u+1]/max );

		cvLine(src, pt1, pt2, cvScalar(blue, green, red), 1);	  
	}
	pt1.x =0;
	pt1.y =0;

	pt2.x = 255;
	pt2.y = 0;
	cvLine(src, pt1, pt2, cvScalar(0,0,0), 1);

	pt1.x =0;
	pt1.y =0;

	pt2.x = 0;
	pt2.y = recHeight;
	cvLine(src, pt1, pt2, cvScalar(0,0,0), 1);

	pt1.x =255;  pt1.y =0;

	pt2.x = 255; pt2.y = recHeight;
	cvLine(src, pt1, pt2, cvScalar(0,0,0), 1);

	pt1.x =0;
	pt1.y =recHeight;
	pt2.x = 255;
	pt2.y = recHeight;
	cvLine(src, pt1, pt2, cvScalar(0,0,0), 1);

	CvFont font;

	float hscale = 0.3f;
	float vscale = 0.3f;
	int linewidth = 1;
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX | CV_FONT_HERSHEY_SIMPLEX,hscale,vscale,0,linewidth);

	char text1[20];
	for(u=10; u<255; u)
	{
		sprintf_s(text1,"%d",u);
		pt2.x =pt1.x =u;
		pt1.y =350; pt2.y = 350+5;
		cvLine(src, pt1, pt2, cvScalar(0,0,0), 1);
		cvPutText(src,text1,cvPoint(u,360),&font,cvScalar(0,0,0));
		u += 20;
	}

#ifdef _DEBUG
#endif
	ShowImageCv(src, aName); 
	cvReleaseImage(&src);;

	return;
}

void CvGetImageSize(const IplImage* pSrc, int &width, int &height, int &widthStep)
{
	if(pSrc==NULL)
		return;
	
	width = pSrc->width;
	height = pSrc->height;
	widthStep = pSrc->widthStep;
}

// ��ʼ��ʱ
void StartCalTime(float &t)
{
	t=(float)cvGetTickCount();//��ʱ��ʼ
}


// ֹͣ��ʱ�������ʱ���
void StopCalTime(float &t, char* textout)
{
	float tnow;
	tnow= (float)cvGetTickCount()-t;//��ʱֹͣ,�����ʱ��

	tnow= (float)(tnow/((float)cvGetTickFrequency()*1000.0));

	printf( textout );
	printf( "  ʱ�䣺%.1f ms\n", tnow );
}

//��ʾͼƬ
int ShowImageCv(IplImage* src,char win_name[])
{
	if(src==NULL) 
	{
		return 0;
	}

	cvNamedWindow( win_name, CV_WINDOW_NORMAL );
	cvShowImage( win_name, src );

	return 1;
}