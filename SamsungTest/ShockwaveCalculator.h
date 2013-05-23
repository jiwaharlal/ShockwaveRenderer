#pragma once

#include <vector>
/*#if defined _HAS_ITERATOR_DEBUGGING
	#undef _HAS_ITERATOR_DEBUGGING
	#define _HAS_ITERATOR_DEBUGGING 0
#endif*/
#include <list>

#include "BasicDefs.h"

//namespace sw {

class Bitmap;

class ShockwaveCalculator
{
public:
				ShockwaveCalculator(SHARED_PTR(Bitmap)	aSrcBitmap,
									SHARED_PTR(Bitmap)	aDestBitmap );
				~ShockwaveCalculator();

	void		calculateShockwave(	float			aAmplitude, 
									float			aOutsideRadix,
									unsigned int	aXCenter,
									unsigned int	aYCenter );
private:
	SHARED_PTR(Bitmap)	mySrcBitmap;
	SHARED_PTR(Bitmap)	myDestBitmap;

	bool				myIsMultithreaded;

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