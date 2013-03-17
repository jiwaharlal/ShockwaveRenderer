#include "Mutex.h"

Mutex::Mutex(void)
{
	myMutex = CreateMutex(NULL, FALSE, NULL);
	myCopyCounter = new int(1);
}

Mutex::Mutex(const Mutex& other)
	: myMutex(other.myMutex)
	, myCopyCounter(other.myCopyCounter)
{
	(*myCopyCounter)++;
}

Mutex::~Mutex(void)
{
	(*myCopyCounter)--;
	if (*myCopyCounter == 0) {
		delete myCopyCounter;
		CloseHandle(myMutex);
	}
}

HANDLE Mutex::handle()
{
	return myMutex;
}
