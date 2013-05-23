#include "Sprite.h"

#include "ModelClock.h"
#include "Bitmap.h"
#include "BitmapProvider.h"
#include "ThreadPool.h"

#include <memory>
#include <functional>
#include <iostream>
#include <list>
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

float
AnimatedSprite::getDuration()
{
	return static_cast<float>(static_cast<double>(getFrameCount()) * myFrameDuration);
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
	int					aY,
	RECT&				aOutRedrawRect)
{
	int frameIndex = getCurrentFrameIndex();
	if (frameIndex >= getFrameCount()) {
		return;
	}
	ASSERT(frameIndex < getFrameCount(), "Frame is out of sprite bounds");
	int framesPerRow = myBitmap->getWidth() / myFrameWidth;
	int frameRow = frameIndex / framesPerRow;
	int frameCol = frameIndex % framesPerRow;
	int leftBottomX = frameCol * myFrameWidth;
	int leftBottomY = myBitmap->getHeight() - (frameRow + 1) * myFrameHeight;

	std::list<std::shared_ptr<Job> > jobs;

	int heightStep = 15;
	for (int hi = heightStep, lo = 0; hi < myFrameHeight; lo = hi, hi += heightStep)
	{
		if (hi > myFrameHeight)
		{
			hi = myFrameHeight;
		}

		jobs.push_back(ThreadPool::instance().addJob(std::bind(
			[&](int low, int high)
			{
				for (int row = low; row < high; row++)
				{
					for (int col = 0; col < myFrameWidth; col++)
					{
						const RGB& pixel = myBitmap->getPixel(leftBottomX + col, leftBottomY + row);
						if (pixel.Red + pixel.Green + pixel.Blue < 150) {
							continue;
						}
						aTargetBitmap->setPixel(aX + col - myBaseX, aY + row - myBaseY, pixel);
					}
				}
			},
			lo,
			hi)));
	}
	
	for (auto& j: jobs)
	{
		j->waitForCompletion();
	}
	/*for (int row = 0; row < myFrameHeight; row++)
	{
		for (int col = 0; col < myFrameWidth; col++)
		{
			const RGB& pixel = myBitmap->getPixel(leftBottomX + col, leftBottomY + row);
			if (pixel.Red + pixel.Green + pixel.Blue < 150) {
				continue;
			}
			aTargetBitmap->setPixel(aX + col - myBaseX, aY + row - myBaseY, pixel);
		}
	}*/

	defineRedrawRect(aX, aY, aOutRedrawRect);
}

void
AnimatedSprite::defineRedrawRect(
	int			aX,
	int			aY,
	RECT&		aOutRedrawRect)
{
	aOutRedrawRect.left = aX - (myFrameWidth - myBaseX);
	aOutRedrawRect.right = aX + (myFrameWidth - myBaseX);
	aOutRedrawRect.bottom = aY - (myFrameHeight - myBaseY);
	aOutRedrawRect.top = aY + (myFrameHeight - myBaseY);
}

void
AnimatedSprite::erasePreviousFrame(
	SHARED_PTR(Bitmap)	aSrcBitmap,
	SHARED_PTR(Bitmap)	aDestBitmap,
	int					aX,
	int					aY,
	RECT&				aOutRedrawRect)
{
	int frameIndex = getCurrentFrameIndex();
	if (frameIndex == 0) {
		return;
	}
	frameIndex -= 1;
	int framesPerRow = myBitmap->getWidth() / myFrameWidth;
	int frameRow = frameIndex / framesPerRow;
	int frameCol = frameIndex % framesPerRow;
	int leftBottomX = frameCol * myFrameWidth;
	int leftBottomY = myBitmap->getHeight() - (frameRow + 1) * myFrameHeight;

	std::list<std::shared_ptr<Job> > jobs;

	int heightStep = 15;
	for (int hi = heightStep, lo = 0; hi < myFrameHeight; lo = hi, hi += heightStep)
	{
		if (hi > myFrameHeight)
		{
			hi = myFrameHeight;
		}

		jobs.push_back(ThreadPool::instance().addJob(std::bind(
			[&](int low, int high)
			{
				for (int row = low; row < high; row++)
				{
					for (int col = 0; col < myFrameWidth; col++)
					{
						const RGB& spritePixel = myBitmap->getPixel(leftBottomX + col, leftBottomY + row);
						if (spritePixel.Red + spritePixel.Green + spritePixel.Blue < 150) 
						{
							continue;
						}
						int x = aX + col - myBaseX;
						int y = aY + row - myBaseY;
						aDestBitmap->setPixel(x, y, aSrcBitmap->getPixel(x, y));
					}
				}
			},
			lo,
			hi)));
	}
	
	for (auto& j: jobs)
	{
		j->waitForCompletion();
	}

	/*
	for (int row = 0; row < myFrameHeight; row++)
	{
		for (int col = 0; col < myFrameWidth; col++)
		{
			const RGB& spritePixel = myBitmap->getPixel(leftBottomX + col, leftBottomY + row);
			if (spritePixel.Red + spritePixel.Green + spritePixel.Blue < 150) 
			{
				continue;
			}
			int x = aX + col - myBaseX;
			int y = aY + row - myBaseY;
			aDestBitmap->setPixel(x, y, aSrcBitmap->getPixel(x, y));
		}
	}*/

	defineRedrawRect(aX, aY, aOutRedrawRect);
}

//}