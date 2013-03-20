#include "ShockwaveRenderer.h"

#include "Settings.h"
#include "ModelClock.h"
#include "BasicDefs.h"
#include "ShockwaveCalculator.h"
#include "MutexLock.h"
#include "Sprite.h"
#include "Bitmap.h"

ShockwaveRenderer::ShockwaveRenderer(
	const std::string&			aBmpFileName,
	HDC							aTargetDc,
	const Mutex&				aPaintMutex,
	const Functor<const RECT*>	aInvalidationCallback)
	: myPaintMutex(aPaintMutex)
	, myTargetDc(aTargetDc)
	, myBmpFileName(aBmpFileName)
	, myInvalidationCallback(aInvalidationCallback)
	, myIsMultithreading(false)
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

	int minRadix = settings.getMinRadix();
	int maxRadix = settings.getMaxRadix();
	int waveSpeed = settings.getWaveSpeed();
	float radixAmplitudeRatio = settings.getRadixAmplitudeRatio();

	SystemTimerClock clock;
	
	AnimatedSprite sprite("SkybusterExplosion.bmp");

	SHARED_PTR(Bitmap) srcBitmap(new Bitmap(myBmpFileName));

	{
		MutexLock lock(myPaintMutex.handle());
		srcBitmap->copyPixelsToDevice(myTargetDc);
	}

	myInvalidationCallback(NULL);

	SHARED_PTR(Bitmap) destBitmap(new Bitmap(myBmpFileName));

	while (true) {
		ShockwaveCalculator calculator(srcBitmap, destBitmap, myIsMultithreading); //(originalPixels, bitmapPixels, bitmap.bmHeight, bitmap.bmWidth, myIsMultithreading);
	
		double blastTime = clock.getTime();
		sprite.startAnimation();
		int frameCounter = 0;

		int radixLimit = minRadix + rand() % (maxRadix - minRadix); // limit for current shock wave spreading
		int minX = radixLimit;
		int minY = radixLimit;
		int maxX = srcBitmap->getWidth() - radixLimit;
		int maxY = srcBitmap->getHeight() - radixLimit;

		int x0 = minX + rand() % (maxX - minX);
		int y0 = minY + rand() % (maxY - minY);

		float radixLimitFloat = (float)radixLimit;
		
		//for (int outsideRadix = 1; outsideRadix < radixLimit; outsideRadix += 3) {
		double prevFrameTime = clock.getTime();
		double maxFrameTime = 0.0f;
		double maxCalculationTime = 0.0;
		while(true) {
			double frameTime = clock.getTime();

			maxFrameTime = max(maxFrameTime, frameTime - prevFrameTime);
			
			float outsideRadixFloat = static_cast<float>(frameTime - blastTime) * static_cast<float>(waveSpeed);
			if (outsideRadixFloat > radixLimitFloat) {
				break;
			}
			int outsideRadix = static_cast<int>(outsideRadixFloat);
			if (outsideRadix < 4.0f) {
				continue;
			}
			frameCounter++;
			
			int yFinish = y0 + outsideRadix;
			int yStart = y0 - outsideRadix;
			int xStart = x0 - outsideRadix;
			int xFinish = x0 + outsideRadix;


			double frameStartTime = clock.getTime();

			//float outsideRadixFloat = (float)outsideRadix;
			float amplitude = (radixLimitFloat - outsideRadixFloat) / radixAmplitudeRatio;
			float insideRadix = max(0.0f, outsideRadixFloat - amplitude);

			double calculationBegin = clock.getTime();
			calculator.calculateShockwave(amplitude, outsideRadixFloat, x0, y0);
			if (! sprite.isFinished() ) {
				sprite.render(destBitmap, x0, y0);
			}
			
			double calculationEnd = clock.getTime();
			maxCalculationTime = max(maxCalculationTime, calculationEnd - calculationBegin);

			int bmpHeight = destBitmap->getHeight();
			{
				MutexLock lock(myPaintMutex.handle());
				
				destBitmap->copyPixelsToDevice(myTargetDc, xStart, bmpHeight - yFinish, xFinish - xStart, yFinish - yStart, xStart, yStart);
			}

			RECT r;
			r.left = xStart;
			r.top = bmpHeight - yFinish;
			r.right = xFinish;
			r.bottom = bmpHeight - yStart;
			myInvalidationCallback(&r);

			double frameEndTime = clock.getTime();
			prevFrameTime = frameTime;
		}
		double endTime = clock.getTime();
		double framesPerSecond = static_cast<double>(frameCounter) / (endTime - blastTime);
		printf("FPS: %f, max frame time: %f, max calc time: %f\n\r", framesPerSecond, maxFrameTime, maxCalculationTime);
	}
}

void
ShockwaveRenderer::switchMultithreading()
{
	myIsMultithreading = ! myIsMultithreading;
	if (myIsMultithreading) {
		printf("Multithreading\n\r");
	} else {
		printf("Singlethreading\n\r");
	}
}