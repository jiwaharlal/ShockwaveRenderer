#pragma once

#include <vector>
/*#if defined _HAS_ITERATOR_DEBUGGING
	#undef _HAS_ITERATOR_DEBUGGING
	#define _HAS_ITERATOR_DEBUGGING 0
#endif*/
#include <list>

#include "BasicDefs.h"
#include "Mutex.h"

//namespace sw {

class Bitmap;

class ShockwaveCalculator
{
public:
				ShockwaveCalculator(SHARED_PTR(Bitmap)	aSrcBitmap,
									SHARED_PTR(Bitmap)	aDestBitmap,
									bool				aIsMultithreaded = false);
				~ShockwaveCalculator();

	void		calculateShockwave(	float			aAmplitude, 
									float			aOutsideRadix,
									unsigned int	aXCenter,
									unsigned int	aYCenter );
private:
	struct CalculationTask 
	{
		CalculationTask( int aYMin, int aYMax, const Mutex& aMutex );

		int							yMin;
		int							yMax;
		Mutex						mutex;
		bool						isDone;
	};

	SHARED_PTR(Bitmap)	mySrcBitmap;
	SHARED_PTR(Bitmap)	myDestBitmap;

	/*RGB*				myOriginalPixels;
	RGB*				myResultPixels;
	unsigned int		myImageHeight;
	unsigned int		myImageWidth;
	*/

	bool				myIsMultithreaded;

	typedef std::vector<HANDLE> ThreadStorage;
	ThreadStorage		myThreads;

	typedef std::list<CalculationTask> TaskStorage;
	TaskStorage			myTasks;
	Mutex				myTaskListMutex;

	float				myAmplitude;
	float				myOutsideRadix;
	int					myXCenter;
	int					myYCenter;
	float				myInsideRadix;

	void					calculateForYRange( int				aYMin,
												int				aYMax );
	static DWORD WINAPI		workerThreadStart( LPVOID			aParam );
	void					workerThreadFunc();
};

//}