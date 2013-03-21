#pragma once

#include "BasicDefs.h"
#include "ModelClock.h"

class ShockwaveCalculator;
class Bitmap;

class Shockwave
{
public:
					Shockwave(	SHARED_PTR(Bitmap)	aSrcBitmap,
								SHARED_PTR(Bitmap)	aDestBitmap,
								int					aSpreadRadius,
								int					aCenterX,
								int					aCenterY );
					~Shockwave();

	void			startAnimation();
	bool			isFinished();
	void			render(	RECT&					aOutUpdateRect );
	float			getDuration();
private:
	float				mySpreadRadius;
	int					myCenterX;
	int					myCenterY;
	SystemTimerClock	myClock;
	double				myStartTime;
	bool				myAnimationFinished;
	int					myWaveSpeed;
	float				myRadiusAmplitudeRatio;
	SHARED_PTR(Bitmap)	mySrcBitmap;
	SHARED_PTR(Bitmap)	myDestBitmap;

	SHARED_PTR(ShockwaveCalculator)	myCalculator;
};
