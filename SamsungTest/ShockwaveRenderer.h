#pragma once

#include <string>

#include "BindMember.h"
#include "Mutex.h"

class AnimatedSprite;
class Shockwave;

class ShockwaveRenderer
{
public:
							ShockwaveRenderer(	const std::string&			aBmpFileName,
												HDC							aTargetDc, //HWND						aTargetWindowHandle,
												const Mutex&				aPaintMutex,
												const Functor<const RECT*>	aIvalidationCallback);
							~ShockwaveRenderer();

private:

	Functor<const RECT*>	myInvalidationCallback;
	std::string				myBmpFileName;
	HDC						myTargetDc;
	Mutex					myPaintMutex;

	static DWORD WINAPI		renderThread(LPVOID aParam);
	void					renderShockwaves();
};
