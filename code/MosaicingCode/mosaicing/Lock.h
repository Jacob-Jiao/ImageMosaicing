
#pragma once


#include <windows.h>


#ifdef WIN32
	// ��������������
	#define  MUTEX CRITICAL_SECTION
	// �����ʼ�����Ĺ���
	#define MUTEXINIT(m) InitializeCriticalSection(m)
	// �������
	#define MUTEXLOCK(m) EnterCriticalSection(m)
	// �������
	#define MUTEXUNLOCK(m) LeaveCriticalSection(m)
	// ����ݻ�����������
	#define MUTEXDESTROY(m) DeleteCriticalSection(m)
#endif

class CMutexLock
{
public:
	// ��ʼ����
	CMutexLock(void) 
	{ 
		MUTEXINIT(&m_Lock); 
	}
	// �ݻ���
	~CMutexLock(void) 
	{ 
		MUTEXDESTROY(&m_Lock); 
	}

public:
	// ����
	void Lock() 
	{ 
		MUTEXLOCK(&m_Lock); 
	}
	// ����
	void Unlock() 
	{ 
		MUTEXUNLOCK(&m_Lock); 
	}

private:

	MUTEX m_Lock;
};