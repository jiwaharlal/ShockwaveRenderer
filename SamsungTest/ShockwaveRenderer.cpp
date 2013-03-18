#include "ShockwaveRenderer.h"

#include "Settings.h"
#include "ModelClock.h"
#include "BasicDefs.h"
#include "ShockwaveCalculator.h"
#include "MutexLock.h"

ShockwaveRenderer::ShockwaveRenderer(
	const std::string&			aBmpFileName,
	HDC							aTargetDc,
	const Mutex&				aPaintMutex,
	const Functor<const RECT*>	aInvalidationCallback)
	: myPaintMutex(aPaintMutex)
	, myTargetDc(aTargetDc)
	, myBmpFileName(aBmpFileName)
	, myInvalidationCallback(aInvalidationCallback)
	, myIsMultithreading(true)
{
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
	
	HBITMAP bmp = (HBITMAP)LoadImageA(0, myBmpFileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (bmp == NULL) {
		DWORD errorCode = GetLastError();
		printf("Cannot load image - error 0x%x", errorCode);
		return;
	}

	BITMAP 			bitmap;
	HDC 			memDc;
    HGDIOBJ 		oldBitmap;

	memDc = CreateCompatibleDC(myTargetDc);
	
    oldBitmap = SelectObject(memDc, bmp);
    GetObject(bmp, sizeof(bitmap), &bitmap);

    BITMAPINFO bi24BitInfo; // We set this up to grab what we want
    bi24BitInfo.bmiHeader.biBitCount = 24; // rgb 8 bytes for each component(3)
    bi24BitInfo.bmiHeader.biCompression = BI_RGB;// rgb = 3 components
    bi24BitInfo.bmiHeader.biPlanes = 1;
    bi24BitInfo.bmiHeader.biSize = sizeof(bi24BitInfo.bmiHeader); // size of this struct
    bi24BitInfo.bmiHeader.biWidth = bitmap.bmWidth; // width of window
    bi24BitInfo.bmiHeader.biHeight = bitmap.bmHeight; // height of window

    HBITMAP editBitmap = CreateDIBSection(myTargetDc, &bi24BitInfo, DIB_RGB_COLORS, 0, 0, 0); // create a dib section for the dc
    SelectObject(myTargetDc, editBitmap); // assign the dib section to the dc

	BitBlt(myTargetDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, memDc, 0, 0, SRCCOPY);
	
	RGB *bitmapPixels = new RGB[bitmap.bmWidth * bitmap.bmHeight];
    
	GetDIBits(myTargetDc, editBitmap, 0, bi24BitInfo.bmiHeader.biHeight, bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // grab the pixels in the dc
	RGB *originalPixels = new RGB[bitmap.bmWidth * bitmap.bmHeight];
	memcpy(originalPixels, bitmapPixels, bitmap.bmWidth * bitmap.bmHeight * 3);

	{
		MutexLock lock(myPaintMutex.handle());
		SetDIBitsToDevice(myTargetDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, 0, 0, 0, bitmap.bmHeight, bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
	}

	myInvalidationCallback(NULL);
	

	while (true) {
		ShockwaveCalculator calculator(originalPixels, bitmapPixels, bitmap.bmHeight, bitmap.bmWidth, myIsMultithreading);
	
		double blastTime = clock.getTime();
		int frameCounter = 0;

		int radixLimit = minRadix + rand() % (maxRadix - minRadix); // limit for current shock wave spreading
		int minX = radixLimit;
		int minY = radixLimit;
		int maxX = bitmap.bmWidth - radixLimit;
		int maxY = bitmap.bmHeight - radixLimit;

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
			double calculationEnd = clock.getTime();
			maxCalculationTime = max(maxCalculationTime, calculationEnd - calculationBegin);

			{
				MutexLock lock(myPaintMutex.handle());
				SetDIBitsToDevice(myTargetDc, xStart, bitmap.bmHeight - yFinish, xFinish - xStart, yFinish - yStart, xStart, yStart, 0, bitmap.bmHeight, /*bitmapBytes*/bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
			}

			RECT r;
			r.left = xStart;
			r.top = bitmap.bmHeight - yFinish;
			r.right = xFinish;
			r.bottom = bitmap.bmHeight - yStart;
			myInvalidationCallback(&r);

			double frameEndTime = clock.getTime();
			if ((frameEndTime - frameStartTime) < 0.05) {
				//Sleep(10);
			}
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