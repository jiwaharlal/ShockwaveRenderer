#include "Shockwave.h"

#include "ShockwaveCalculator.h"
#include "Bitmap.h"
#include "Settings.h"

Shockwave::Shockwave(
	SHARED_PTR(Bitmap)	aSrcBitmap,
	SHARED_PTR(Bitmap)	aDestBitmap,
	int					aSpreadRadius,
	int					aCenterX,
	int					aCenterY)
	: mySrcBitmap(aSrcBitmap)
	, myDestBitmap(aDestBitmap)
	, mySpreadRadius(static_cast<float>(aSpreadRadius))
	, myCenterX(aCenterX)
	, myCenterY(aCenterY)
	, myAnimationFinished(true)
{
	myWaveSpeed = Settings::instance().getWaveSpeed();
	myRadiusAmplitudeRatio = Settings::instance().getRadiusAmplitudeRatio();
}

Shockwave::~Shockwave()
{
}

void
Shockwave::startAnimation()
{
	myStartTime = myClock.getTime();
	myCalculator = SHARED_PTR(ShockwaveCalculator)(new ShockwaveCalculator(mySrcBitmap, myDestBitmap));
}

bool
Shockwave::isFinished()
{
	double time = myClock.getTime();
	float outsideRadius = static_cast<float>((time - myStartTime) * static_cast<double>(myWaveSpeed));
	return outsideRadius > mySpreadRadius;
}

float
Shockwave::getDuration()
{
	return static_cast<float>(mySpreadRadius) / static_cast<float>(myWaveSpeed);
}

void
Shockwave::render(	
	RECT&				aOutUpdateRect )
{
	double frameTime = myClock.getTime();
	float outsideRadiusFloat = static_cast<float>((frameTime - myStartTime) * static_cast<double>(myWaveSpeed));
	if (outsideRadiusFloat > mySpreadRadius) {
		return;
	}
	if (outsideRadiusFloat < 4.0f) {
		return;
	}
	int outsideRadius = static_cast<int>(outsideRadiusFloat);
	aOutUpdateRect.left = myCenterX - outsideRadius;
	aOutUpdateRect.bottom = myCenterY - outsideRadius;
	aOutUpdateRect.right = myCenterX + outsideRadius;
	aOutUpdateRect.top = myCenterY + outsideRadius;

	float amplitude = (mySpreadRadius - outsideRadiusFloat) / myRadiusAmplitudeRatio;

	myCalculator->calculateShockwave(amplitude, outsideRadiusFloat, myCenterX, myCenterY);
}
