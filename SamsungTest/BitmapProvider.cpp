#include "BitmapProvider.h"

#include "Bitmap.h"

//namespace sw {

BitmapProvider::BitmapProvider(void)
{
}

BitmapProvider::~BitmapProvider(void)
{
}

BitmapProvider&		
BitmapProvider::instance()
{
	static BitmapProvider instance;

	return instance;
}

SHARED_PTR(Bitmap)
BitmapProvider::getBitmap(
	const std::string& aBmpFileName)
{
	BitmapStorage::iterator it = myBitmaps.find(aBmpFileName);
	if (it != myBitmaps.end()) {
		return it->second;
	}
	
	SHARED_PTR(Bitmap) newBitmap(new Bitmap(std::string("..//img//") + aBmpFileName));
	myBitmaps[aBmpFileName] = newBitmap;
	return newBitmap;
}

//}