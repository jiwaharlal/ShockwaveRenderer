#include "Shockwave.h"

#include "ShockwaveCalculator.h"

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
