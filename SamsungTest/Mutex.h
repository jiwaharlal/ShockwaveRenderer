#pragma once

#include <windows.h>

class Mutex
{
public:
				Mutex();
				Mutex(const Mutex&);
				~Mutex();

	HANDLE		handle() const;
private:
	HANDLE	myMutex;
	int*	myCopyCounter;
};
