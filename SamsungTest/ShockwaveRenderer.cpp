#include "ShockwaveRenderer.h"

#include "Settings.h"
#include "ModelClock.h"
#include "BasicDefs.h"
#include "ShockwaveCalculator.h"
#include "MutexLock.h"
#include "Sprite.h"
#include "Bitmap.h"
#include "Blast.h"

ShockwaveRenderer::ShockwaveRenderer(
	const std::string&			aBmpFileName,
	HDC							aTargetDc,
	const Mutex&				aPaintMutex,
	const Functor<const RECT*>	aInvalidationCallback)
	: myPaintMutex(aPaintMutex)
	, myTargetDc(aTargetDc)
	, myBmpFileName(aBmpFileName)
	, myInvalidationCallback(aInvalidationCallback)
{
	//myTargetDc = CreateCompatibleDC(GetDC(NULL));
	CreateThread(NULL, 0, renderThread, this, 0, NULL);
}

ShockwaveRenderer::~ShockwaveRenderer(void)
{
}

DWORD WINAPI
ShockwaveRenderer::renderThread(LPVOID aParam)
{
	static_cast<ShockwaveRenderer*>(aParam)->renderShockwaves();
	return 0;
}

void
ShockwaveRenderer::renderShockwaves()
{
	const Settings& settings = Settings::instance();

	//int waveSpeed = settings.getWaveSpeed();
	//float radiusAmplitudeRatio = settings.getRadiusAmplitudeRatio();

	SystemTimerClock clock;
	
	//AnimatedSprite sprite("SkybusterExplosion.bmp");

	SHARED_PTR(Bitmap) srcBitmap(new Bitmap(myBmpFileName));

	{
		MutexLock lock(myPaintMutex.handle());
		srcBitmap->copyPixelsToDevice(myTargetDc);
	}

	myInvalidationCallback(NULL);

	SHARED_PTR(Bitmap) destBitmap(new Bitmap(myBmpFileName));
	
	typedef std::list<SHARED_PTR(Blast) > BlastStorage;
	BlastStorage blasts;
	
	double nextBlastTime = clock.getTime();

	long long prevSeconds = static_cast<long long>(nextBlastTime);
	int frameCounter = 0;
	while (true) {
		double curTime = clock.getTime();
		
		frameCounter++;
		long long seconds = static_cast<long long>(curTime);
		if (seconds != prevSeconds) {
			printf("FPS: %d\n\r", frameCounter);
			frameCounter = 0;
			prevSeconds = seconds;
		}

		if (curTime >= nextBlastTime)
		{
			int minRadius = settings.getMinRadius();
			int maxRadius = settings.getMaxRadius();

			int spreadRadius = minRadius + rand() % (maxRadius - minRadius);
			int minX = spreadRadius;
			int minY = spreadRadius;
			int maxX = srcBitmap->getWidth() - spreadRadius;
			int maxY = srcBitmap->getHeight() - spreadRadius;

			int x0 = minX + rand() % (maxX - minX);
			int y0 = minY + rand() % (maxY - minY);
			
			blasts.push_back(SharedNew(new Blast(srcBitmap, destBitmap, spreadRadius, x0, y0)));

			if (settings.isBlastsParallel()) 
			{
				float duration = blasts.back()->getDuration();
				nextBlastTime = curTime + (static_cast<double>(rand() % static_cast<int>(duration * 1000)) / 1000.0);
			}
			else
			{
				nextBlastTime = curTime + blasts.back()->getDuration();
			}
		}

		BlastStorage::iterator it = blasts.begin();
		RECT redrawRect;
		redrawRect.left = srcBitmap->getWidth();
		redrawRect.bottom = srcBitmap->getHeight();
		redrawRect.right = 0;
		redrawRect.top = 0;

		RECT r;
		while (it != blasts.end())
		{
			if ((*it)->isFinished())
			{
				it = blasts.erase(it);
				continue;
			}
			(*it)->renderShockwave(r);
			++it;

			redrawRect.bottom = min(redrawRect.bottom, r.bottom);
			redrawRect.left = min(redrawRect.left, r.left);
			redrawRect.right = max(redrawRect.right, r.right);
			redrawRect.top = max(redrawRect.top, r.top);
		}
		for (it = blasts.begin(); it != blasts.end(); ++it)
		{
			(*it)->renderSplash(r);

			redrawRect.bottom = min(redrawRect.bottom, r.bottom);
			redrawRect.left = min(redrawRect.left, r.left);
			redrawRect.right = max(redrawRect.right, r.right);
			redrawRect.top = max(redrawRect.top, r.top);
		}

		int bmpHeight = destBitmap->getHeight();
		{
			MutexLock lock(myPaintMutex.handle());
			destBitmap->copyPixelsToDevice(myTargetDc);
			/*destBitmap->copyPixelsToDevice(
								myTargetDc, 
								redrawRect.left, 
								bmpHeight - redrawRect.top, 
								redrawRect.right - redrawRect.left,
								redrawRect.top - redrawRect.bottom,
								redrawRect.left,
								redrawRect.bottom);*/
			//destBitmap->copyPixelsToDevice(myTargetDc, xStart, bmpHeight - yFinish, xFinish - xStart, yFinish - yStart, xStart, yStart);
		}

		r.left = redrawRect.left;
		r.top = bmpHeight - redrawRect.top;
		r.right = redrawRect.right;
		r.bottom = bmpHeight - redrawRect.bottom;
		myInvalidationCallback(&r);

		//myInvalidationCallback(NULL);
	}

}
