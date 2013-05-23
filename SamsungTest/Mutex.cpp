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
	decreaseCopyCount();
}

const Mutex&
Mutex::operator=(
	const Mutex& other)
{
	decreaseCopyCount();
	myCopyCounter = other.myCopyCounter;
	++(*myCopyCounter);
	myMutex = other.myMutex;

	return *this;
}

void
Mutex::decreaseCopyCount()
{
	(*myCopyCounter)--;
	if (*myCopyCounter == 0) {
		delete myCopyCounter;
		CloseHandle(myMutex);
	}
}


HANDLE 
Mutex::handle() const
{
	return myMutex;
}
