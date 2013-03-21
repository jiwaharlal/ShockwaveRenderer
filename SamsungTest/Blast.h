#pragma once

#include "BasicDefs.h"

class Bitmap;
class Shockwave;
class AnimatedSprite;

class Blast
{
public:
						Blast(SHARED_PTR(Bitmap)	aSrcBitmap,
								SHARED_PTR(Bitmap)	aDestBitmap,
								int					aSpreadRadius,
								int					aCenterX,
								int					aCenterY);
						~Blast();

	float				getDuration();
	void				renderShockwave(RECT& aOutUpdateRect);
	void				renderSplash(RECT& aOutUpdateRect);
	bool				isFinished();
private:
	SHARED_PTR(Shockwave)		myShockwave;
	SHARED_PTR(AnimatedSprite)	mySplashSprite;

	SHARED_PTR(Bitmap)			myDestBitmap;
	SHARED_PTR(Bitmap)			mySrcBitmap;
	int							myCenterX;
	int							myCenterY;
};
