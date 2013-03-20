#include "Sprite.h"

#include "ModelClock.h"
#include "Bitmap.h"
#include "BitmapProvider.h"

//namespace sw {

AnimatedSprite::AnimatedSprite(
	const std::string&	aBmpFileName)
{
	myBitmap = BitmapProvider::instance().getBitmap(aBmpFileName);
	char buffer[128];

	GetPrivateProfileStringA(aBmpFileName.c_str(), "frameWidth", "0", buffer, 128, "..\\img\\sprites.ini");
	myFrameWidth = atoi(buffer);
	GetPrivateProfileStringA(aBmpFileName.c_str(), "frameHeight", "0", buffer, 128, "..\\img\\sprites.ini");
	myFrameHeight = atoi(buffer);
	GetPrivateProfileStringA(aBmpFileName.c_str(), "frameDuration", "0", buffer, 128, "..\\img\\sprites.ini");
	myFrameDuration = atof(buffer);
	GetPrivateProfileStringA(aBmpFileName.c_str(), "spriteBaseX", "0", buffer, 128, "..\\img\\sprites.ini");
	myBaseX = atoi(buffer);
	GetPrivateProfileStringA(aBmpFileName.c_str(), "spriteBaseY", "0", buffer, 128, "..\\img\\sprites.ini");
	myBaseY = atoi(buffer);
}

AnimatedSprite::~AnimatedSprite(void)
{
}

void
AnimatedSprite::startAnimation()
{
	myStartTime = myClock.getTime();
}

int
AnimatedSprite::getCurrentFrameIndex()
{
	double timePassed = myClock.getTime() - myStartTime;
	return static_cast<int>(timePassed / myFrameDuration);
}

int
AnimatedSprite::getFrameCount()
{
	return (myBitmap->getHeight() / myFrameHeight) * (myBitmap->getWidth() / myFrameWidth);
}

bool
AnimatedSprite::isFinished()
{
	return getCurrentFrameIndex() >= getFrameCount();
}

void
AnimatedSprite::render(
	SHARED_PTR(Bitmap)	aTargetBitmap,
	int					aX,
	int					aY)
{
	int frameIndex = getCurrentFrameIndex();
	int framesPerRow = myBitmap->getWidth() / myFrameWidth;
	int frameRow = frameIndex / framesPerRow;
	int frameCol = frameIndex % framesPerRow;
	int leftBottomX = frameCol * myFrameWidth;
	int leftBottomY = myBitmap->getHeight() - (frameRow + 1) * myFrameHeight;

	for (int row = 0; row < myFrameHeight; row++)
	{
		for (int col = 0; col < myFrameWidth; col++)
		{
			const RGB& pixel = myBitmap->getPixel(leftBottomX + col, leftBottomY + row);
			if (pixel.Red + pixel.Green + pixel.Blue < 150) {
				continue;
			}
			//int targetOffset = (aY + row - myBaseY) * aImageWidth + aX + col - myBaseX;
			aTargetBitmap->setPixel(aX + col - myBaseX, aY + row - myBaseY, pixel);
			//aBitmapPixels[targetOffset] = pixel;
		}
	}
}

//}