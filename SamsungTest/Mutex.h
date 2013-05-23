#pragma once

#include <windows.h>

class Mutex
{
public:
					Mutex();
					Mutex(const Mutex&);
					~Mutex();
	const Mutex&	operator=(const Mutex&);

	HANDLE			handle() const;
private:
	void			decreaseCopyCount();

	HANDLE	myMutex;
	int*	myCopyCounter;
};
