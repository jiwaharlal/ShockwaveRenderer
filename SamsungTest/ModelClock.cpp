#include "ModelClock.h"
#include <windows.h>
#include <stdio.h>

ModelClock::ModelClock(void)
{
}


ModelClock::~ModelClock(void)
{
}

SystemTimerClock::SystemTimerClock() {
	unsigned __int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	timerFreq = static_cast<double>(freq);
}


double SystemTimerClock::getTime() const {
	unsigned __int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER *)&curTime);

	return static_cast<double>(curTime/timerFreq);
}