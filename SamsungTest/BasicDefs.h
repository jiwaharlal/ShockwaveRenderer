#pragma once

#include <windows.h>

#include "rl_ptr.h"

#define E 2.7183f
#define PI 3.14159265358f

struct RGB
{
        BYTE Blue;
        BYTE Green;
        BYTE Red;
};

#define SHARED_PTR(X) koki::rl_ptr<X>

#if defined _DEBUG
	#define ASSERT(aCondition, aMessage) \
		if (!(aCondition)) { \
		printf("Assertion failed with message %s\n\r", aMessage); \
			*static_cast<int*>(NULL) = 1; \
		} 
#else
	#define ASSERT(aCondition, aMessage)
#endif
