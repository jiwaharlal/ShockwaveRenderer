#pragma once

#include <vector>
/*#if defined _HAS_ITERATOR_DEBUGGING
	#undef _HAS_ITERATOR_DEBUGGING
	#define _HAS_ITERATOR_DEBUGGING 0
#endif*/
#include <list>

#include "BasicDefs.h"
#include "Mutex.h"

class ShockwaveCalculator
{
public:
				ShockwaveCalculator(RGB*			aOriginalPixels,
									RGB*			aResultPixels,
									unsigned int	aImageHeight,
									unsigned int	aImageWidth,
									bool			aIsMultithreaded = false);
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
	RGB*				myOriginalPixels;
	RGB*				myResultPixels;
	unsigned int		myImageHeight;
	unsigned int		myImageWidth;

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
