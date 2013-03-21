#pragma once

#include <windows.h>
#include <string>
#include <sstream>

#include "rl_ptr.h"

#define E 2.7183f
#define PI 3.14159265358f

struct RGB
{
	bool operator == (const RGB& other) const { return Red == other.Red && Green == other.Green && Blue == other.Blue; }
    BYTE Blue;
    BYTE Green;
    BYTE Red;
};

#define SHARED_PTR(X) koki::rl_ptr<X>

template <class T>
SHARED_PTR(T)
SharedNew(T* value)
{
	return SHARED_PTR(T)(value);
}

#if defined _DEBUG
	#define ASSERT(aCondition, aMessage) \
		if (!(aCondition)) { \
		printf("Assertion failed with message %s\n\r", aMessage); \
			*static_cast<int*>(NULL) = 1; \
		} 
#else
	#define ASSERT(aCondition, aMessage)
#endif

template <typename T> 
std::string 
toStr(const T& value) { 
	std::ostringstream os; 
	os << value; 
	return os.str(); 
}