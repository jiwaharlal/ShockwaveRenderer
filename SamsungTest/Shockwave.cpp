#include "Shockwave.h"

#include "ShockwaveCalculator.h"
#include "Bitmap.h"
#include "Settings.h"

Shockwave::Shockwave(
	int		aSpreadRadius,
	int		aCenterX,
	int		aCenterY)
	: mySpreadRadius(static_cast<float>(aSpreadRadius))
	, myCenterX(aCenterX)
	, myCenterY(aCenterY)
{
}

Shockwave::~Shockwave()
{
}

void
Shockwave::startAnimation()
{
	myStartTime = myClock.getTime();
}

bool
Shockwave::isFinished()
{
	return false;
}

void
Shockwave::render(	
	SHARED_PTR(Bitmap)	aSrcBitmap,
	SHARED_PTR(Bitmap)	aDestBitmap )
{
}
