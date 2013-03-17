// SamsungTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <WinGDI.h>
#include <iostream>
#include "ModelWindow.h"
#include <conio.h>
#include <stdio.h>
#include <sstream>
#include <math.h>
#include "ModelClock.h"
#include "BasicDefs.h"
#include "MutexLock.h"

using namespace std;


void renderWaves(HWND aWindowHandle) {
    RECT Rect; // just a simple rect to hold the size of our window
    HDC WindowDC = GetDC(aWindowHandle); // grab a device content to it
    GetClientRect(aWindowHandle, &Rect); // grab window size

    BITMAPINFO bi24BitInfo; // We set this up to grab what we want
    bi24BitInfo.bmiHeader.biBitCount = 24; // rgb 8 bytes for each component(3)
    bi24BitInfo.bmiHeader.biCompression = BI_RGB;// rgb = 3 components
    bi24BitInfo.bmiHeader.biPlanes = 1;
    bi24BitInfo.bmiHeader.biSize = sizeof(bi24BitInfo.bmiHeader); // size of this struct
    bi24BitInfo.bmiHeader.biWidth = Rect.right - Rect.left; // width of window
    bi24BitInfo.bmiHeader.biHeight = Rect.bottom - Rect.top; // height of window

    BYTE *bBytes = new BYTE[bi24BitInfo.bmiHeader.biWidth * bi24BitInfo.bmiHeader.biHeight * 3]; // create enough room. all pixels * each color component

    HDC iDC = CreateCompatibleDC(0); // create dc to store the image
    HBITMAP iBitmap = CreateDIBSection(iDC, &bi24BitInfo, DIB_RGB_COLORS, 0, 0, 0); // create a dib section for the dc
    SelectObject(iDC, iBitmap); // assign the dib section to the dc

    SetWindowPos(aWindowHandle, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE); // bring to front to see it
    //SendMessage(Window, WM_PAINT, 0, 0); // send a paint message so it will paint before we try to paint to it

    BitBlt(iDC, 0, 0, bi24BitInfo.bmiHeader.biWidth, bi24BitInfo.bmiHeader.biHeight, WindowDC, 0, 0, SRCCOPY); // copy hdc to our hdc
    GetDIBits(iDC, iBitmap, 0, bi24BitInfo.bmiHeader.biHeight, bBytes, &bi24BitInfo, DIB_RGB_COLORS); // grab the pixels in the dc

	int height = bi24BitInfo.bmiHeader.biHeight;
	int width = bi24BitInfo.bmiHeader.biWidth;
    for (int i = 0; i < 10000000; i++)
	{
		/*for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				int offset = (row * width + col) * 3;

				RGB* Pixel = (RGB*)&bBytes[offset];

				BYTE Temp = Pixel->Red;
				Pixel->Red = static_cast<BYTE>((255 * (height - row) / height) * (sin(float(col - i) / 15.0) + 1.0) / 2.0); //Pixel->Green;
				//Pixel->Blue = 0; //Pixel->Red;
				BYTE green = static_cast<BYTE>((255 * row / height) * (sin(float(col + i) / 15.0) + 1.0) / 2.0);
				Pixel->Green = green;
			}
		}*/
		SetDIBitsToDevice(iDC, 0, 0, bi24BitInfo.bmiHeader.biWidth, bi24BitInfo.bmiHeader.biHeight, 0, 0, 0, bi24BitInfo.bmiHeader.biHeight, bBytes, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
		BitBlt(WindowDC, 0, 0, bi24BitInfo.bmiHeader.biWidth, bi24BitInfo.bmiHeader.biHeight, iDC, 0, 0, SRCCOPY); // copy hdc to their hdc
	}

	DeleteDC(iDC); // delete dc
    ReleaseDC(aWindowHandle, WindowDC); // release our dc handle
    DeleteObject(iBitmap); // delete object
}

float f(float x, float amplitude) {
    return E * x * pow((float)E, - x / amplitude);
}

DWORD WINAPI renderThread(LPVOID aParam) {
	ModelWindow* modelWindow = *((ModelWindow**)aParam);
	HWND windowHandle = modelWindow->getHandle();
    HDC windowDc = GetDC(windowHandle); // grab a device content to it

	HBITMAP myBmp = (HBITMAP)LoadImageA(0, /*"d:\\tmp\\graph.bmp"*/ "..\\img\\graph.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (myBmp == NULL) {
		DWORD errorCode = GetLastError();
		printf("Cannot load image - error 0x%x", errorCode);

		return 0;
	}

	//HDC 			hdc;
	BITMAP 			bitmap;
	HDC 			hdcMem;
    HGDIOBJ 		oldBitmap;

	//if (myBmp == NULL) {
	//	break;
	//}

	//hdc = BeginPaint(hWnd, &ps);

	hdcMem = CreateCompatibleDC(windowDc);
	HDC editDc = CreateCompatibleDC(windowDc);
	
    oldBitmap = SelectObject(hdcMem, myBmp);

    GetObject(myBmp, sizeof(bitmap), &bitmap);

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
    
	GetDIBits(editDc, editBitmap, 0, bi24BitInfo.bmiHeader.biHeight, /*bitmapBytes*/bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // grab the pixels in the dc
	RGB *originalPixels = new RGB[bitmap.bmWidth * bitmap.bmHeight];
	memcpy(originalPixels, bitmapPixels, bitmap.bmWidth * bitmap.bmHeight * 3);

	modelWindow->setBitmapToDraw(editDc, bitmap.bmHeight, bitmap.bmWidth);
	
	SetDIBitsToDevice(editDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, 0, 0, 0, bitmap.bmHeight, /*bitmapBytes*/bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
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

			int yStart = y0 - outsideRadix;
			int yFinish = y0 + outsideRadix;
			int xStart = x0 - outsideRadix;
			int xFinish = x0 + outsideRadix;
			for (int dy = -outsideRadix; dy < outsideRadix; ++dy) {
				int row = y0 + dy;

				for (int dx = -outsideRadix; dx < outsideRadix; dx++) {
					float pointR = sqrt((float)((dx * dx) + (dy * dy)));
					if ( pointR > outsideRadixFloat ) {
						continue;
					}
					int col = x0 + dx;
					int offset = (row * bitmap.bmWidth + col);
					if ( pointR < insideRadix ) {
						bitmapPixels[offset] = originalPixels[offset];
						continue;
					}
					if (dx == 0 && dy == 0) {
						continue;
					}
					float distance;
					unsigned int key = (static_cast<int>((static_cast<float>(outsideRadix) - pointR) * 20.0f) << 16) + static_cast<int>(amplitude * 20.f);
					DistanceMap::iterator it = distanceHash.find(key);
					if (it == distanceHash.end())
					{					

						
						float x = outsideRadixFloat - pointR;
						float deltaX = epsilon;
						float y = f(x, amplitude);
						float deltaY = f(x + deltaX, amplitude) - y;

						float sign = 1.0;
						if (deltaY < 0.0) {
							deltaY = -deltaY;
							sign = -1.0;
						}

						float alpha = atan(deltaX / deltaY);
						float beta = asin(cos(alpha) / 1.63f);
						float theta = alpha + beta;
				        
						distance = y / tan(theta) * sign;

						distanceHash[key] = distance;
					}
					else
					{
						distance = it->second;
					}

					int srcX = col - (int)((float)(col - x0) / pointR * distance);
					int srcY = row - (int)((float)(row - y0) / pointR * distance);

					int srcOffset = (srcY * bitmap.bmWidth + srcX)/* * 3*/;
					bitmapPixels[offset] = originalPixels[srcOffset];
				}
			}
			double frameCalculationTime = clock.getTime() - frameStartTime;
			//modelWindow->lockPaint();
			//SetDIBitsToDevice(editDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, 0, 0, 0, bitmap.bmHeight, /*bitmapBytes*/bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
			
			{
				MutexLock lock(modelWindow->getPaintMutex());
				SetDIBitsToDevice(editDc, xStart, bitmap.bmHeight - yFinish, xFinish - xStart, yFinish - yStart, xStart, yStart, 0, bitmap.bmHeight, /*bitmapBytes*/bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
			}
			//modelWindow->unlockPaint();
			RECT r;
			r.left = xStart;
			r.top = bitmap.bmHeight - yFinish;
			r.right = xFinish;
			r.bottom = bitmap.bmHeight - yStart;
			
			InvalidateRect(windowHandle, &r, false);
			
			frames++;
			
			//Sleep(50);
			double frameTime = clock.getTime() - frameStartTime;
			//printf("Frame %d, calculated in %f, rendered in %f seconds\n\r", frames, frameCalculationTime, frameTime);
			//BitBlt(windowDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, editDc, 0, 0, SRCCOPY);
		}

		double timeDif = clock.getTime() - startTime;
		printf("%d frames, took %f seconds\n\r", frames, timeDif);
	

		//Sleep(3000);
	}
	
	SetDIBitsToDevice(editDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, 0, 0, 0, bitmap.bmHeight, /*bitmapBytes*/bitmapPixels, &bi24BitInfo, DIB_RGB_COLORS); // set the new dibs to the dc
	//BitBlt(WindowDC, 0, 0, bi24BitInfo.bmiHeader.biWidth, bi24BitInfo.bmiHeader.biHeight, iDC, 0, 0, SRCCOPY); // copy hdc to their hdc
    BitBlt(windowDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, editDc, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);

	//renderWaves(windowHandle);

	return 0;
}

int main(int argc, char* argv[])
{
	MSG        Msg;

	/*if ( argc < 3 || argc > 5 ) {
		cout << "Usage: UbisoftTest.exe <speed> <angle> [gravity] [starting height]\n";
		
		return 0;
	}

	speed = atof(argv[1]);
	angle = atof(argv[2]);

	if ( argc > 3 ) {
		gravity = atof(argv[3]);
		if ( argc > 4 ) {
			startHeight = atof(argv[4]);
		} 
	}*/

	ModelWindow* wnd = new ModelWindow();
	
	HWND windowHandle = wnd->getHandle();
	CreateThread(NULL, 0, renderThread, &wnd, 0, NULL);
	// receive and process messages
	while( GetMessage(&Msg, NULL, 0, 0) )
	{
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
	}

	delete wnd;
	return 0;
}

