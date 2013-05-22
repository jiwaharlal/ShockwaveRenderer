#include "ShockwaveCalculator.h"
#include <algorithm>
#include <math.h>
#include <map>
#include <vector>
#include <list>
#include <memory>

#include "MutexLock.h"
#include "ModelClock.h"
#include "Settings.h"
#include "Bitmap.h"
#include "ThreadPool.h"

#ifndef max
#define max(x, y) std::max(x, y);
#endif

ShockwaveCalculator::CalculationTask::CalculationTask( int aYMin, int aYMax, const Mutex& aMutex )
	: yMin(aYMin)
	, yMax(aYMax)
	, mutex(aMutex)
	, isDone(false)
{}

ShockwaveCalculator::ShockwaveCalculator(
	SHARED_PTR(Bitmap)	aSrcBitmap,
	SHARED_PTR(Bitmap)	aDestBitmap )
	: mySrcBitmap(aSrcBitmap)
	, myDestBitmap(aDestBitmap)
{
	myIsMultithreaded = Settings::instance().isMultithreaded();
	//if (! myIsMultithreaded ) {
		return;
	//}
	int threadCount = Settings::instance().getThreadCount();
	for (int i = 0; i < threadCount; i++) {
		myThreads.push_back(CreateThread(NULL, 0, workerThreadStart, this, 0, NULL));
	}
}

ShockwaveCalculator::~ShockwaveCalculator(void)
{
	if (! myIsMultithreaded ) {
		return;
	}
	for (ThreadStorage::iterator it = myThreads.begin(); it != myThreads.end(); ++it) {
		TerminateThread(*it, 0);
	}
	MutexLock lock(myTaskListMutex.handle());
}

float waveFunc(float x, float amplitude) 
{
    return E * x * pow((float)E, - x / amplitude);
}

void 
ShockwaveCalculator::calculateShockwave(
	float				aAmplitude,
	float				aOutsideRadix,
	unsigned int		aXCenter,
	unsigned int		aYCenter )
{
	myAmplitude = aAmplitude;
	myOutsideRadix = aOutsideRadix;
	myXCenter = aXCenter;
	myYCenter = aYCenter;
	myInsideRadix = max(0.0f, aOutsideRadix - aAmplitude);

	SystemTimerClock clock;

	unsigned int outsideRadixInt = static_cast<int>(aOutsideRadix);
	int yMin = aYCenter - outsideRadixInt;
	int yMax = aYCenter + outsideRadixInt;
	
	if (! myIsMultithreaded) {
		calculateForYRange(yMin, yMax);
	} else {
		std::list<std::shared_ptr<Job> > jobs;

		for (int yLow = yMin, yHigh = yLow + 20; yHigh < yMax; yLow = yHigh, yHigh += 20) {
			jobs.push_back(ThreadPool::instance().addJob(std::bind(&ShockwaveCalculator::calculateForYRange, this, yLow, yHigh)));
			//myTasks.push_back(CalculationTask(yLow, yHigh, Mutex()));
		}

		for (auto& j: jobs)
		{
			j->waitForCompletion();
		}

		/*{
			MutexLock listLock(myTaskListMutex.handle());
			myTasks.clear();
			for (int yLow = yMin, yHigh = yLow + 20; yHigh < yMax; yLow = yHigh, yHigh += 20) {
				myTasks.push_back(CalculationTask(yLow, yHigh, Mutex()));
			}
		}
		
		while(true) {
			{
				MutexLock listLock(myTaskListMutex.handle());

				TaskStorage::iterator it;
				for (it = myTasks.begin(); it != myTasks.end(); ++it) {
					if ( WaitForSingleObject(it->mutex.handle(), 0) != WAIT_OBJECT_0 ) {
						continue;
					}
					ReleaseMutex(it->mutex.handle());
					if ( it->isDone ) {
						it = myTasks.erase(it);
						
						if (it == myTasks.end()) {
							break;
						}
					}
				}
				if ( myTasks.empty() ) {
					break;
				}
			}
		}*/
	}
}

DWORD WINAPI
ShockwaveCalculator::workerThreadStart( LPVOID			aParam )
{
	static_cast<ShockwaveCalculator*>(aParam)->workerThreadFunc();
	return 0;
}

void
ShockwaveCalculator::workerThreadFunc()
{
	while (true)
	{
		CalculationTask* task = NULL;

		{
			MutexLock listLock(myTaskListMutex.handle());
			
			for (TaskStorage::iterator it = myTasks.begin(); it != myTasks.end(); ++it)
			{
				if (WaitForSingleObject(it->mutex.handle(), 0) != WAIT_OBJECT_0) {
					continue;
				}
				if (! it->isDone ) {
					task = &(*it);
					break;
				}
				ReleaseMutex(it->mutex.handle());
			}
		}

		if (task != NULL) {
			calculateForYRange(task->yMin, task->yMax);
			task->isDone = true;
			ReleaseMutex(task->mutex.handle());
		}
	}
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
			//int offset = (row * myImageWidth + col);
			if ( pointR < insideRadix ) {
				//myResultPixels[offset] = myOriginalPixels[offset];
				myDestBitmap->setPixel(col, row, mySrcBitmap->getPixel(col, row));
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

			//int srcOffset = (srcY * myImageWidth + srcX);
			//myResultPixels[offset] = myOriginalPixels[srcOffset];
			myDestBitmap->setPixel(col, row, mySrcBitmap->getPixel(srcX, srcY));
		}
	}

	return;
}
