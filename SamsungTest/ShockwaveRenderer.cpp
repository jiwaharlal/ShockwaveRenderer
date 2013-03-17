#include "ShockwaveRenderer.h"

#include "Settings.h"

ShockwaveRenderer::ShockwaveRenderer(
	const std::string&			aBmpFileName,
	HDC							aTargetContext,
	const Mutex&				aPaintMutex,
	const Functor<const RECT&>	aInvalidationCallback )
	: myPaintMutex(aPaintMutex)
	, myTargetContext(aTargetContext)
	, myBmpFileName(aBmpFileName)
	, myInvalidationCallback(aInvalidationCallback)
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

	/*HBITMAP bmp = (HBITMAP)LoadImageA(0, myBmpFileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (myBmp == NULL) {
		DWORD errorCode = GetLastError();
		printf("Cannot load image - error 0x%x", errorCode);

		return 0;
	}

	BITMAP 			bitmap;
	HDC 			hdcMem;
    HGDIOBJ 		oldBitmap;

	hdcMem = CreateCompatibleDC(myTargetContext);
	//HDC editDc = CreateCompatibleDC(windowDc);
	
    oldBitmap = SelectObject(hdcMem, myBmp);
    GetObject(bmp, sizeof(bitmap), &bitmap);

    BITMAPINFO bi24BitInfo; // We set this up to grab what we want
    bi24BitInfo.bmiHeader.biBitCount = 24; // rgb 8 bytes for each component(3)
    bi24BitInfo.bmiHeader.biCompression = BI_RGB;// rgb = 3 components
    bi24BitInfo.bmiHeader.biPlanes = 1;
    bi24BitInfo.bmiHeader.biSize = sizeof(bi24BitInfo.bmiHeader); // size of this struct
    bi24BitInfo.bmiHeader.biWidth = bitmap.bmWidth; // width of window
    bi24BitInfo.bmiHeader.biHeight = bitmap.bmHeight; // height of window

    HBITMAP editBitmap = CreateDIBSection(editDc, &bi24BitInfo, DIB_RGB_COLORS, 0, 0, 0); // create a dib section for the dc
    SelectObject(editDc, editBitmap); // assign the dib section to the dc

	BitBlt(editDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	//BitBlt(windowDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	
	RGB *bitmapPixels = new RGB[bitmap.bmWidth * bitmap.bmHeight];
    
	GetDIBits(editDc, editBitmap, 0, bi24BitInfo.bmiHeader.biHeight, bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // grab the pixels in the dc
	RGB *originalPixels = new RGB[bitmap.bmWidth * bitmap.bmHeight];
	memcpy(originalPixels, bitmapPixels, bitmap.bmWidth * bitmap.bmHeight * 3);

	modelWindow->setBitmapToDraw(editDc, bitmap.bmHeight, bitmap.bmWidth);
	
	SetDIBitsToDevice(editDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, 0, 0, 0, bitmap.bmHeight, bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
	int minRadix = 100;
	int maxRadix = 400;

	SystemTimerClock clock;

	InvalidateRect(windowHandle, NULL, true);

	typedef std::map<unsigned int, float> DistanceMap;
	DistanceMap distanceHash;
	
	while (true) {
		double startTime = clock.getTime();
		int frames = 0;

		int radixLimit = minRadix + rand() % (maxRadix - minRadix); // limit for current shock wave spreading
		int minX = radixLimit;
		int minY = radixLimit;
		int maxX = bitmap.bmWidth - radixLimit;
		int maxY = bitmap.bmHeight - radixLimit;

		int x0 = minX + rand() % (maxX - minX);
		int y0 = minY + rand() % (maxY - minY);

		float radixLimitFloat = (float)radixLimit;
		
		float amplitudeConstant = 5.0;
		
		float epsilon = 0.2f;

		for (int outsideRadix = 1; outsideRadix < radixLimit; outsideRadix += 3) {
			double frameStartTime = clock.getTime();

			float outsideRadixFloat = (float)outsideRadix;
			float amplitude = (radixLimitFloat - outsideRadixFloat) / amplitudeConstant;
			float insideRadix = max(0.0f, outsideRadixFloat - amplitude);
	*/
}