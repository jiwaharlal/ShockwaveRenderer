#include "MutexLock.h"

MutexLock::MutexLock(HANDLE aMutex)
	: myMutex(aMutex)
{
	while ( WaitForSingleObject(aMutex, INFINITE) != WAIT_OBJECT_0 )
	{
	}
}

MutexLock::~MutexLock(void)
{
	ReleaseMutex(myMutex);
}
