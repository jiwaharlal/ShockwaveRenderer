#pragma once

#include "BasicDefs.h"

class ShockwaveCalculator;

class Shockwave
{
public:
					Shockwave(	int aSpreadRadius,
								int aCenterX,
								int aCenterY);
					~Shockwave();
private:
	float		mySpreadRadius;
	int			myCenterX;
	int			myCenterY;

	SHARED_PTR(ShockwaveCalculator)	myCalculator;
};
