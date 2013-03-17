#include "ShockwaveCalculator.h"
#include <algorithm>
#include <math.h>
#include <map>
#include <vector>

#include "MutexLock.h"

#ifndef max
#define max(x, y) std::max(x, y);
#endif

ShockwaveCalculator::CalculationTask::CalculationTask( ShockwaveCalculator* aCalc, int aYMin, int aYMax, Mutex* aMutex )
	: calculator(aCalc)
	, yMin(aYMin)
	, yMax(aYMax)
	, mutex(aMutex)
{}

ShockwaveCalculator::ShockwaveCalculator(
	RGB*			aOriginalPixels,
	RGB*			aResultPixels,
	unsigned int	aImageHeight,
	unsigned int	aImageWidth,
	bool			aIsMultythreaded /*= false*/ )
	: myOriginalPixels(aOriginalPixels)
	, myResultPixels(aResultPixels)
	, myImageHeight(aImageHeight)
	, myImageWidth(aImageWidth)
	, myIsMultythreaded(aIsMultythreaded)
{
	//myTasksMutex = CreateMutex(NULL, FALSE, NULL);
}

ShockwaveCalculator::~ShockwaveCalculator(void)
{
}

float waveFunc(float x, float amplitude) {
    return E * x * pow((float)E, - x / amplitude);
}

void 
ShockwaveCalculator::calculateShockwave(
	float			aAmplitude, 
	float			aOutsideRadix,
	unsigned int	aXCenter,
	unsigned int	aYCenter )
{
	myAmplitude = aAmplitude;
	myOutsideRadix = aOutsideRadix;
	myXCenter = aXCenter;
	myYCenter = aYCenter;
	myInsideRadix = max(0.0f, aOutsideRadix - aAmplitude);

	unsigned int outsideRadixInt = static_cast<int>(aOutsideRadix);
	int yMin = aYCenter - outsideRadixInt;
	int yMax = aYCenter + outsideRadixInt;
	
	if (! myIsMultythreaded) {
		calculateForYRange(yMin, yMax);
	} else {
		std::vector<CalculationTask> tasks;
		tasks.reserve((yMax - yMin) / 10 + 1);
		myChunkMutexes.reserve((yMax - yMin) / 10 + 1);
		for (int yLow = yMin, yHigh = yLow + 10; yLow <= yMax; yLow = yHigh + 1, yHigh = max(yHigh + 10, yMax)) {
			myChunkMutexes.push_back(Mutex());
			tasks.push_back(CalculationTask(this, yLow, yHigh, &myChunkMutexes.back()));
			CreateThread(NULL, 0, calculateChunckThread, &tasks.back(), 0, NULL);
		}
		for (MutexStorage::iterator it = myChunkMutexes.begin(); it != myChunkMutexes.end(); ++it) {
			MutexLock lock(it->handle());
		}
	}
}

DWORD WINAPI
ShockwaveCalculator::calculateChunckThread(
	LPVOID aParam )
{
	CalculationTask* task = static_cast<CalculationTask*>(aParam);
	MutexLock lock(task->mutex->handle());
	task->calculator->calculateForYRange(task->yMin, task->yMax);

	return 0;
}

void
ShockwaveCalculator::calculateForYRange( 
	int	aYMin,
	int	aYMax )
{
	int outsideRadix = static_cast<int>(myOutsideRadix);
	float insideRadix = max(0.0f, myOutsideRadix - myAmplitude);

	for (int row = aYMin; row < aYMax; ++row) {
		//int row = myYCenter + dy;
		int dy = row - myYCenter;

		for (int dx = -outsideRadix; dx < outsideRadix; dx++) {
			float pointR = sqrt((float)((dx * dx) + (dy * dy)));
			if ( pointR > myOutsideRadix ) {
				continue;
			}
			int col = myXCenter + dx;
			int offset = (row * myImageWidth + col);
			if ( pointR < insideRadix ) {
				myResultPixels[offset] = myOriginalPixels[offset];
				continue;
			}
			if (dx == 0 && dy == 0) {
				continue;
			}
				
			float x = myOutsideRadix - pointR;
			float deltaX = 0.2f;
			float y = waveFunc(x, myAmplitude);
			float deltaY = waveFunc(x + deltaX, myAmplitude) - y;

			float sign = 1.0;
			if (deltaY < 0.0) {
				deltaY = -deltaY;
				sign = -1.0;
			}

			float alpha = atan(deltaX / deltaY);
			float beta = asin(cos(alpha) / 1.63f);
			float theta = alpha + beta;
	        
			float distance = y / tan(theta) * sign;

			int srcX = col - (int)((float)(col - myXCenter) / pointR * distance);
			int srcY = row - (int)((float)(row - myYCenter) / pointR * distance);

			int srcOffset = (srcY * myImageWidth + srcX)/* * 3*/;
			myResultPixels[offset] = myOriginalPixels[srcOffset];
		}
	}
}
