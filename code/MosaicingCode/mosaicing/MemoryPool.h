// "MemoryPool.h"
// �ڴ��
// �Ƕ��̰߳�ȫ(2011-01-16)
#pragma once

//#include "afxwin.h"

#include <vector>
#include "Lock.h"

//#pragma warning(push)
#pragma warning(disable:4251) // your declarations that cause 4251
//#pragma warning(pop)


#if defined (_WINDLL) || defined(_CONSOLE) || defined(_WINDOWS)
	#ifndef MV_MODULE_TYPE
		#define MV_MODULE_TYPE _declspec(dllexport)
	#else
		#define MV_MODULE_TYPE _declspec(dllimport)
	#endif
#else
	#define MV_MODULE_TYPE 
#endif

#ifndef _MemoryPool
	#if _DEBUG
		//#pragma comment(lib,"memoryPoold.lib")
	#else
		//#pragma comment(lib,"memoryPool.lib")
	#endif
#endif

typedef unsigned long ulong;

typedef unsigned short ushort;

struct MV_MODULE_TYPE MemoryBlockHeader
{
	ulong m_blockLevel;
	MemoryBlockHeader* m_pNext;
};

// �ڴ�Ǽ�
struct MemoryRegistrator
{
	int id;
	void* pAddress;
};

// ***************************************************
// �ڴ����
// ***************************************************
class MV_MODULE_TYPE CMemoryPool
{
public:

	CMemoryPool();

	~CMemoryPool();

public:
	// ��ʼ���ڴ��
	void InitialPool();

	// �ڴ����
	void* Operate(void* pDataBlock, int nSize=0);

	// �����ڴ�
	void* NewIn(int nSize); // Ӧ�ó���������ڴ��ߴ�

	// �ͷ��ڴ�
	bool DeleteIn(void* pDataBlock );
	
	// �������е��ڴ��
	static bool Destroy();

private:
	inline void WritePoolHead(ulong nLevel, MemoryBlockHeader* pHead);

	ulong CalValidBlockSize(ulong userNeedSize, ulong &nLevel);

	// ������ʵ��ڴ��ߴ�
	unsigned long CalValidBlockSizeTab(unsigned long userNeedSize, ulong &nLevel);

	inline ulong Pow2(ulong n);

	ulong Pow2Inv(ulong val);
#ifdef _DEBUG
	void RegNew(void*);

	void RegDelete(void*);

	static void PrintLeakInfo();
#endif

private:
	 //CMultiReadSingleWriteLock m_Lock;
	CMutexLock m_lock;

	// ��֦��
	static int m_branchNumber;

	// ����
	static std::vector<MemoryBlockHeader*> m_internalPool;
};




