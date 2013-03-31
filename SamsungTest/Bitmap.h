#pragma once

#include <string>
#include <vector>

#include "BasicDefs.h"

//namespace sw {

	class Bitmap
	{
	public:
								Bitmap(const std::string&	aBmpFileName);
								~Bitmap(void);

		const RGB&				getPixel(int aX, int aY) const;
		void					setPixel(int aX, int aY, const RGB& aValue);

		void					copyPixelsToDevice(
										HDC					aTargetDc,
										int					aXDest = 0,
										int					aYDest = 0,
										int					aHeight = 0,
										int					aWidth = 0,
										int					aXSrc = 0,
										int					aYSrc = 0);

		RGB*					getPixels();
		int						getHeight() const;
		int						getWidth() const;
	private:
		BITMAPINFO			myBitmapInfo;
		std::vector<RGB>	myPixels;
		int					myHeight;
		int					myWidth;
		HBITMAP				myBmp;

		//HDC					myPrevTargetDc;
		//HBITMAP				myDibSection;
	};

//}