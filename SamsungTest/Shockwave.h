#pragma once

#include "BasicDefs.h"
#include "ModelClock.h"

class ShockwaveCalculator;
class Bitmap;

class Shockwave
{
public:
					Shockwave(	int aSpreadRadius,
								int aCenterX,
								int aCenterY);
					~Shockwave();

	void			startAnimation();
	bool			isFinished();
	void			render(	SHARED_PTR(Bitmap)	aSrcBitmap,
							SHARED_PTR(Bitmap)	aDestBitmap );
private:
	float				mySpreadRadius;
	int					myCenterX;
	int					myCenterY;
	SystemTimerClock	myClock;
	double				myStartTime;

	SHARED_PTR(ShockwaveCalculator)	myCalculator;
};
