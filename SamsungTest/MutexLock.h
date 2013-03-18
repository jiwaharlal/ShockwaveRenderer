#pragma once

#include <windows.h>

class MutexLock
{
public:
			MutexLock(HANDLE aMutex);
			~MutexLock();
private:
	HANDLE myMutex;
};
