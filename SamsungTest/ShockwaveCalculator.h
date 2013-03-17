#pragma once

#include <vector>

#include "BasicDefs.h"
#include "Mutex.h"

class ShockwaveCalculator
{
public:
				ShockwaveCalculator(RGB*			aOriginalPixels,
									RGB*			aResultPixels,
									unsigned int	aImageHeight,
									unsigned int	aImageWidth,
									bool			aIsMultythreaded = false);
				~ShockwaveCalculator();

	void		calculateShockwave(	float			aAmplitude, 
									float			aOutsideRadix,
									unsigned int	aXCenter,
									unsigned int	aYCenter );
private:
	struct CalculationTask 
	{
		CalculationTask(ShockwaveCalculator* calc, int aYMin, int aYMax, Mutex* aMutex );

		ShockwaveCalculator*		calculator;
		int							yMin;
		int							yMax;
		Mutex*						mutex;
	};
	RGB*				myOriginalPixels;
	RGB*				myResultPixels;
	unsigned int		myImageHeight;
	unsigned int		myImageWidth;
	//HANDLE				myTasksMutex;
	typedef std::vector<Mutex> MutexStorage;
	MutexStorage	myChunkMutexes;
	bool			myIsMultythreaded;

	float			myAmplitude;
	float			myOutsideRadix;
	int				myXCenter;
	int				myYCenter;
	float			myInsideRadix;

	void					calculateForYRange( int				aYMin,
												int				aYMax );
	static DWORD WINAPI 	calculateChunckThread( LPVOID		aParam );
};
