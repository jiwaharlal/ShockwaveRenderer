#include "Bitmap.h"

//namespace sw {

Bitmap::Bitmap(
	const std::string&		aBmpFileName)
{
	myBmp = (HBITMAP)LoadImageA(0, aBmpFileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (myBmp == NULL) {
		DWORD errorCode = GetLastError();
		printf("Cannot load image \"%s\" - error 0x%x", aBmpFileName.c_str(), errorCode);
		return;
	}
	
	BITMAP 			bitmap;
	
	HDC memDc = CreateCompatibleDC(GetDC(NULL)); //(aTargetDc);
	
    SelectObject(memDc, myBmp);
    GetObject(myBmp, sizeof(bitmap), &bitmap);

    myBitmapInfo.bmiHeader.biBitCount = 24; // rgb 8 bytes for each component(3)
    myBitmapInfo.bmiHeader.biCompression = BI_RGB;// rgb = 3 components
    myBitmapInfo.bmiHeader.biPlanes = 1;
    myBitmapInfo.bmiHeader.biSize = sizeof(myBitmapInfo.bmiHeader); // size of this struct
    myBitmapInfo.bmiHeader.biWidth = bitmap.bmWidth; // width of window
    myBitmapInfo.bmiHeader.biHeight = bitmap.bmHeight; // height of window

	myHeight = bitmap.bmHeight;
	myWidth = bitmap.bmWidth;
    
	myPixels.resize(bitmap.bmWidth * bitmap.bmHeight);
    
	GetDIBits(memDc, myBmp, 0, bitmap.bmHeight, &myPixels[0], &myBitmapInfo, DIB_RGB_COLORS); // grab the pixels in the dc

	//DeleteObject(myBmp);
	DeleteDC(memDc);
}

Bitmap::~Bitmap()
{
	DeleteObject(myBmp);
}

const RGB&
Bitmap::getPixel(int aX, int aY) const
{
	ASSERT(aX < myWidth && aY < myHeight, "Out of bitmap boudaries");

	int offset = aY * myWidth + aX;
	return myPixels[offset];
}

void					
Bitmap::setPixel(int aX, int aY, const RGB& aValue)
{
	ASSERT(aX < myWidth && aY < myHeight, "Out of bitmap boudaries");
	myPixels[aY * myWidth + aX] = aValue;
}

RGB*
Bitmap::getPixels()
{
	return &(myPixels[0]);
}

int
Bitmap::getHeight() const
{
	return myHeight;
}

int
Bitmap::getWidth() const
{
	return myWidth;
}

void					
Bitmap::copyPixelsToDevice(
	HDC					aTargetDc,
	int					aXDest,		// = 0
	int					aYDest,		// = 0
	int					aHeight,	// = 0
	int					aWidth,		// = 0
	int					aXSrc,		// = 0
	int					aYSrc)		// = 0	
{
	if (aHeight == 0) {
		aHeight = myHeight;
	}
	if (aWidth == 0) {
		aWidth = myWidth;
	}

    SelectObject(aTargetDc, myBmp); // assign the dib section to the dc

	SetDIBitsToDevice(	aTargetDc, 
						aXDest, 
						aYDest, 
						aWidth, 
						aHeight, 
						aXSrc, 
						aYSrc, 
						0, 
						myHeight, 
						&myPixels[0], 
						&myBitmapInfo, 
						DIB_RGB_COLORS);
}

//}