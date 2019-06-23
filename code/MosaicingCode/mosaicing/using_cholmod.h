#pragma once

#include "usingCV24.h"

#include "cholmod.h"

#pragma comment (lib, "libamd.lib")
#pragma comment (lib, "libcamd.lib")
#pragma comment (lib, "libcblas.lib")
#pragma comment (lib, "libccolamd.lib")
#pragma comment (lib, "libcerbla.lib")
#pragma comment (lib, "libcholmod.lib")
#pragma comment (lib, "libcolamd.lib")
#pragma comment (lib, "libgoto_CHOLMOD.lib")
#pragma comment (lib, "libmetis_CHOLMOD.lib")

struct SparseMatElem
{
	int iRow;
	double val;
	bool   operator <  (const SparseMatElem& elem)  const   //��������ʱ����д�ĺ���
	{   
		return iRow<elem.iRow; 
	}

	SparseMatElem(int iRow, double val)
	{
		this->iRow = iRow;
		this->val = val;
	}
	SparseMatElem()
	{

	}
};

// ϡ�������б�ʾ
// 3 2 4 6
// 4 5 7
//   4 1
//   2
struct SparseMatCol
{
	vector<SparseMatElem> matCol;
};

namespace pool
{
	struct SparseMatrix
	{
		vector<SparseMatCol> mat;
		int col;
		int row;

		void Clear()
		{
			for(int i=0; i<col; i++)
			{
				mat[i].matCol.clear();
			}
		}
	};
	
	
}

int cvPrintMatrix(CvMat* pSrc, char* pName, int syme = 0);

void TestCholmod();

cholmod_dense* cholmod_FeedDenseMat(CvMat* pA, cholmod_common &c);

cholmod_sparse* cholmod_FeedSparseMat(CvMat* pA, int symmetric, cholmod_common &c);

int SolveSparseSystem(CvMat* pA, CvMat* pB, CvMat* pX);

// ���ܣ����ϡ������ϵͳ
int SolveSparseSystem2(pool::SparseMatrix pA, pool::SparseMatrix pB, CvMat* pX);

// ���ܣ����ϡ������ϵͳ
// pAΪ�ԳƵķ���
int SolveSparseSystem3(pool::SparseMatrix pA, pool::SparseMatrix pB, CvMat* pX);

// ���ܣ����ϡ������ϵͳ
// pEye-----��λ��
int SolveSparseSystemLM(pool::SparseMatrix pA, pool::SparseMatrix pB, pool::SparseMatrix pEye, CvMat* pX, double lambda);