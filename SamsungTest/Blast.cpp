#include "Blast.h"

#include "BasicDefs.h"
#include "Shockwave.h"
#include "Sprite.h"

Blast::Blast(
	SHARED_PTR(Bitmap)	aSrcBitmap,
	SHARED_PTR(Bitmap)	aDestBitmap,
	int					aSpreadRadius,
	int					aCenterX,
	int					aCenterY)
	: mySrcBitmap(aSrcBitmap)
	, myDestBitmap(aDestBitmap)
	, myCenterX(aCenterX)
	, myCenterY(aCenterY)
{
	myShockwave = SharedNew(new Shockwave(aSrcBitmap, aDestBitmap, aSpreadRadius, aCenterX, aCenterY));
	mySplashSprite = SharedNew(new AnimatedSprite("SkybusterExplosion.bmp"));
	myShockwave->startAnimation();
	mySplashSprite->startAnimation();
}

Blast::~Blast(void)
{
}

float
Blast::getDuration()
{
	return max(myShockwave->getDuration(), mySplashSprite->getDuration());
}

void
Blast::renderShockwave(RECT& aOutUpdateRect)
{
	if (! myShockwave->isFinished()) {
		myShockwave->render(aOutUpdateRect);
	}
}

void
Blast::renderSplash(RECT& aOutUpdateRect)
{
	if (! mySplashSprite->isFinished()) {
		if (myShockwave->isFinished()) {
			mySplashSprite->erasePreviousFrame(mySrcBitmap, myDestBitmap, myCenterX, myCenterY, aOutUpdateRect);
		}
		mySplashSprite->render(myDestBitmap, myCenterX, myCenterY, aOutUpdateRect);
	}
}

bool
Blast::isFinished()
{
	return myShockwave->isFinished() && mySplashSprite->isFinished();
}
