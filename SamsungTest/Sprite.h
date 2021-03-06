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

	void	render(SHARED_PTR(Bitmap)				aTargetBitmap,
					int								aX,
					int								aY,
					RECT&							aOutRedrawRect);
	void	erasePreviousFrame(SHARED_PTR(Bitmap)	aSrcBitmap,
								SHARED_PTR(Bitmap)	aDestBitmap,
								int					aX,
								int					aY,
								RECT&				aOutRedrawRect);
	float	getDuration();
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
	void	defineRedrawRect(int		aX,
							int			aY,
							RECT&		oOutRedrawRect);
};

//}