#pragma once

#include <string>
#include <thread>
#include <functional>
#include <mutex>

#include "BindMember.h"
#include "Mutex.h"

class AnimatedSprite;
class Shockwave;

class ShockwaveRenderer
{
public:
							ShockwaveRenderer(	const std::string&						aBmpFileName,
												HDC										aTargetDc,
												const Functor<std::mutex&, int>&		aGetPaintMutexCallback,
												const Functor<void, const RECT*>&		aIvalidationCallback);
							~ShockwaveRenderer();

private:

	Functor<void, const RECT*>		myInvalidationCallback;
	std::string						myBmpFileName;
	HDC								myTargetDc;
	Mutex							myPaintMutex;
	Functor<std::mutex&, int>		myGetPaintMutexCallback;

	static void						renderThread(ShockwaveRenderer* aRenderer);
	void							renderShockwaves();
	
	unsigned int volatile			myIsStopRender;
	std::thread						myRenderThread;
};
