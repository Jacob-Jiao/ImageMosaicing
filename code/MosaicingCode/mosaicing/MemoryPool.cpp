#include "MemoryPool.h"&
#include "math.h"
#include <iostream>

// ����
std::vector<MemoryBlockHeader*> CMemoryPool::m_internalPool;
int CMemoryPool::m_branchNumber = 0;

static int s_DeleteNum = 0;

static int s_NewNum = 0;
								// 0  1   2   3     4    5
const ulong aBlockSize[100] = {16, 32, 64, 128, 256, 512, 
			// 6    7       8       9       10      11        12          13     14         15
			1024, 1024*2, 1024*4, 1024*8, 1024*16, 1024*32, 1024*64, 1024*128, 1024*256, 1024*512, 
			//  16       17            18           19            20            21
			1024*1024, 1024*1024*2, 1024*1024*4, 1024*1024*8, 1024*1024*16, 1024*1024*32 };

// ��ɼ��
int MV_MODULE_TYPE PermissionCheck()
{
	// ���߻�ȡ������к�
	//GetCameraSerialNumbersOnline(vector<string> &serialNumbers);

	// ��ȡ��Ȩ���к�

	// ƥ��

	return 1;
}
const ulong SMemoryBlockHeadSize = sizeof(MemoryBlockHeader);

// (5)��С�ڴ�鳤�ȣ�16bytes - 8bytes, 
const unsigned long MIN_MEMORY_STACK_BLOCK_USER =  16 - SMemoryBlockHeadSize;

// (5)��С�ڴ�鳤�ȣ�16bytes, 
const unsigned long MIN_MEMORY_STACK_BLOCK =  16;

// (6)�ڴ��ߴ�����ȼ�
const ulong MAX_BLOCK_SIZE_LEVEL = 19;

// (7)���ǹ��������ڴ�鳤��,���������ƣ��ڴ��ֹͣ����, ��Ϊֱ����ϵͳ������ͷ�
const unsigned long MAX_MEMORY_STACK_BLOCK_SIZE  = aBlockSize[MAX_BLOCK_SIZE_LEVEL];

const float fInvlog2 = 1/log(2.0f);

#ifdef _DEBUG
#endif
// �ڴ�ע�����, �����ڴ�й©
static std::vector<MemoryRegistrator> s_regVector;


// �ڴ�ع���
CMemoryPool::CMemoryPool()
{
	//CWinThread *pPrsThread = ::AfxBeginThread(MosaicImagesThread, this);
}

// ��ʼ���ڴ��
void CMemoryPool::InitialPool()
{
	if((int)m_internalPool.size()==0)
	{	
		ulong currentBlockSize = MIN_MEMORY_STACK_BLOCK;
		//// ����
		//m_lock.Lock();
		ulong MAX_MEMORY_STACK_BLOCK_SIZE2  = aBlockSize[MAX_BLOCK_SIZE_LEVEL];
		while(currentBlockSize<=MAX_MEMORY_STACK_BLOCK_SIZE2)
		{
			MemoryBlockHeader* pHead = NULL;

			m_internalPool.push_back(pHead);
			currentBlockSize *= 2;

			m_branchNumber++;
		}	
		
		//// ����
		//m_lock.Unlock();
	}
}

CMemoryPool::~CMemoryPool()
{
	
}

#ifdef _DEBUG
// ע�����
void CMemoryPool::RegNew(void* pAddress)
{
	//MemoryRegistrator regTemp;
	//regTemp.id = s_NewNum;
	//regTemp.pAddress = pAddress;
	//s_regVector.push_back(regTemp);
}
// ע���ͷ�
void CMemoryPool::RegDelete(void* pAddress)
{
	//for(int i=0; i<(int)s_regVector.size(); i++)
	//{
	//	if(s_regVector[i].pAddress==pAddress)
	//	{
	//		s_regVector[i] = s_regVector[s_regVector.size()-1];
	//		s_regVector.pop_back();		
	//	}
	//}
}


// ��ӡй©��Ϣ
void CMemoryPool::PrintLeakInfo()
{
	for(int i=0; i<(int)s_regVector.size(); i++)
	{
		std::cout<<"Leak id "<<s_regVector[i].id<<" address "<<s_regVector[i].pAddress<<std::endl;
	}
}
#endif

// ����2��n�η���ֵ
unsigned long CMemoryPool::Pow2(unsigned long n)
{
	return 1<<n;
}

// ����2��n�η���ֵ������n
unsigned long CMemoryPool::Pow2Inv(unsigned long val)
{
	unsigned long n = 0;
	unsigned long valTemp = val;
	while(valTemp>1)
	{
		valTemp /= 2;
		n++;
	}
	return n;
}

// ������ʵ��ڴ��ߴ�
unsigned long CMemoryPool::CalValidBlockSize(unsigned long userNeedSize, ulong &nLevel)
{
	if(userNeedSize<=MIN_MEMORY_STACK_BLOCK_USER)
	{
		nLevel = 0; 
		return MIN_MEMORY_STACK_BLOCK;
	}
	
	float fN = log(userNeedSize + 8.0f)*fInvlog2 - 4;

	if( (fN-ulong(fN))==0)
	{
		nLevel = (ulong)fN;
	}
	else
	{
		nLevel = (ulong)fN + 1;
	}
	
	//return 16*Pow2(nLevel);
	return 1<<(nLevel+4);
}

// ������ʵ��ڴ��ߴ�
unsigned long CMemoryPool::CalValidBlockSizeTab(unsigned long userNeedSize, ulong &nLevel)
{
	// �۰����
	if(userNeedSize<=aBlockSize[10])
	{
		for(int i=0; i<=10; i++)
		{
			if(userNeedSize<=aBlockSize[i])
			{
				nLevel = i;
				return aBlockSize[i];
			}
		}
	}
	else
	{
		for(int i=11; i<=19; i++)
		{
			if(userNeedSize<=aBlockSize[i])
			{
				nLevel = i;
				return aBlockSize[i];
			}
		}
	}

	return 0;
}

// д�ڴ��"ͷ"
inline void CMemoryPool::WritePoolHead(ulong nLevel, MemoryBlockHeader* pHead)
{
	// ����
	m_lock.Lock();
	
	m_internalPool[nLevel] = pHead;
	
	// ����
	m_lock.Unlock();
}

// *****************************************************************
// ���ܣ������ڴ�
// nSize------------�û�������ڴ��ĳߴ�, ��λΪbyte
// *****************************************************************
void* CMemoryPool::NewIn(int nSize) // Ӧ�ó���������ڴ��ߴ�
{
	if(nSize<=0)
		return NULL;

	// ����ڴ��Ϊ�գ�������ʼ��
	if(m_internalPool.empty())
		InitialPool();

	// ������ʵ��ڴ��Ĵ�С��16, 32, 64, 128, 256, 512��......
	ulong nLevel = 0;
	ulong blockSize = CalValidBlockSize(nSize, nLevel);
	
#ifdef _DEBUG
	//std::cout<<"s_NewNum "<<s_NewNum<<" blockSize "<<blockSize<<std::endl;
	s_NewNum++;
#endif
	
	// ������ڶ���������ڴ��ĳߴ磬����ϵͳֱ�ӷ��䣬�����ڴ��������
	if(blockSize<=MAX_MEMORY_STACK_BLOCK_SIZE)
	{
		// �����ڴ�����Ƿ��и�������ʣ���ڴ�
		if(m_internalPool[nLevel]!=NULL)
		{
			// �����ʣ����ڴ棬����ڴ���з���
			// ����
			void* pBlock = (char*)m_internalPool[nLevel] + SMemoryBlockHeadSize;
			//m_sVecPool[nLevel] = m_sVecPool[nLevel]->m_pNext; // "д"
			WritePoolHead( nLevel, m_internalPool[nLevel]->m_pNext);
#ifdef _DEBUG
			RegNew(pBlock);
#endif
			return pBlock;
		}
		else
		{
			// ���û��ʣ����ڴ棬���OS�����µ��ڴ�
			char* pBlock = new char[blockSize]; // ��OS�����ڴ�
			MemoryBlockHeader* pHead = (MemoryBlockHeader*)(pBlock);
			pHead->m_blockLevel = nLevel;
			pHead->m_pNext = NULL;
#ifdef _DEBUG
			RegNew(pBlock + SMemoryBlockHeadSize);
#endif
			return pBlock + SMemoryBlockHeadSize;
		}
	}
	else
	{
		// ������ڶ���������ڴ��ĳߴ磬����ϵͳֱ�ӷ��䣬�����ڴ��������
		char* pBlock = new char[nSize+SMemoryBlockHeadSize]; // ��OS�����ڴ�
		MemoryBlockHeader* pHead = (MemoryBlockHeader*)(pBlock);
		pHead->m_blockLevel = nLevel;
		pHead->m_pNext = NULL;
#ifdef _DEBUG
		RegNew(pBlock + SMemoryBlockHeadSize);
#endif
		return pBlock + SMemoryBlockHeadSize;
	}

	return NULL;
}

// *****************************************************************
// �ͷ��ڴ�
// *****************************************************************
bool CMemoryPool::DeleteIn(void* pDataBlock )
{
	if(pDataBlock==NULL)
		return true;

#ifdef _DEBUG
	RegDelete(pDataBlock);
	//std::cout<<"s_DeleteNum "<<s_DeleteNum<<std::endl;
	s_DeleteNum++;
#endif

	MemoryBlockHeader* pHead = (MemoryBlockHeader*)((char*)(pDataBlock) - SMemoryBlockHeadSize);

	// ������ڴ���С������
	ulong nLevel = pHead->m_blockLevel;

	// ������ڴ��ĳߴ���ڴ�ع�������ߴ�ҪС����ҵ���Ӧ����������֦��ȥ
	if(pHead->m_blockLevel<=MAX_BLOCK_SIZE_LEVEL)
	{
		// "����"
		pHead->m_pNext = m_internalPool[nLevel]; 
		//m_sVecPool[nLevel] = pHead; // "д"
		WritePoolHead( nLevel, pHead);
	}
	// ������ڴ��ĳߴ���ڴ�ع�������ߴ�Ҫ����ֱ���ͷ�
	else
	{
		delete[] (char*)pHead; pHead = NULL;
	}

	return true;
}


// �����ڴ��
bool CMemoryPool::Destroy()
{
	//// ����
	//CMutexLock lock;
	//lock.Lock();

#ifdef _DEBUG
	PrintLeakInfo();
#endif

	for(int i=0; i<m_branchNumber; i++)
	{
		while(m_internalPool[i]!=NULL)
		{
			MemoryBlockHeader* pBlock = m_internalPool[i]->m_pNext;
			delete[] (char*)m_internalPool[i]; m_internalPool[i] = NULL;
			m_internalPool[i] = pBlock;
		}
	}

	//// ����
	//lock.Unlock();

	return true;
}

void* CMemoryPool::Operate( void* pDataBlock, int nSize/*=0*/ )
{
	CMutexLock lock;
	lock.Lock();

	if(nSize>0) // �����ڴ�
	{
		return NewIn(nSize);
	}
	else if(pDataBlock) // �ͷ��ڴ�
	{
		DeleteIn(pDataBlock);
	}

	// ����
	lock.Unlock();
	return NULL;
}