#pragma once

#include <string>

#include "BasicDefs.h"
#include "ModelClock.h"

//namespace sw {

class Bitmap;

class AnimatedSprite
{
public:
			AnimatedSprite(const std::string&	aBmpFileName);
	
			~AnimatedSprite();

	void	startAnimation();

	bool	isFinished();

	void	drawTo(SHARED_PTR(Bitmap)	aTargetBitmap,
					int					aX,
					int					aY);
private:
	SHARED_PTR(Bitmap)		myBitmap;
	//RGB*					myBitmapPixels;
	SystemTimerClock		myClock;
	/*int						myLeftBottomX;
	int						myLeftBottomY;
	int						myHeight;
	int						myWidth;
	int						myBitmapHeight;
	int						myBitmapWidth;
	int						myXBase;
	int						myYBase;
*/
	double					myStartTime;
	int						myFrameHeight;
	int						myFrameWidth;
	double					myFrameDuration;
	int						myBaseX;
	int						myBaseY;

	POINT	getFrameXY(int				aFrameIndex);
	int		getCurrentFrameIndex();
	int		getFrameCount();
};

//}