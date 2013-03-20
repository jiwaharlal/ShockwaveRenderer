#pragma once

#include <string>
#include <map>

#include "BasicDefs.h"

//namespace sw {

class Bitmap;

class BitmapProvider
{
public:
	static BitmapProvider&	instance();

	SHARED_PTR(Bitmap)		getBitmap(const std::string& aBmpFileName);
private:
	typedef std::map<std::string, SHARED_PTR(Bitmap) > BitmapStorage;
	BitmapStorage	myBitmaps;

							BitmapProvider(void);
							~BitmapProvider(void);
};

//}