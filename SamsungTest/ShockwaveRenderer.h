#pragma once

#include <string>

#include "BindMember.h"
#include "Mutex.h"

class ShockwaveRenderer
{
public:
							ShockwaveRenderer(	const std::string&			aBmpFileName,
												HDC							aTargetContext,
												const Mutex&				aPaintMutex,
												const Functor<const RECT&>	aIvalidationCallback);
							~ShockwaveRenderer();
private:
	Functor<const RECT&>	myInvalidationCallback;
	std::string				myBmpFileName;
	HDC						myTargetContext;
	Mutex					myPaintMutex;

	static DWORD WINAPI		renderThread(LPVOID aParam);
	void					renderShockwaves();
};
