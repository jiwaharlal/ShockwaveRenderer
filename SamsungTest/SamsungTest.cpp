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
#include "SettingsDialog.h"

#include <SDL.h>

using namespace std;

void showSdlSurface()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "error, cannot init SDL" << endl;
		return;
	}
	SDL_Surface* scr = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
	if (!scr)
	{
		cout << "error creating SDL screen" << endl;
		return;
	}
	SDL_WM_SetCaption("sdl window", NULL);

	for (int row = 0; row < 100; row++)
	{
		for (int col = 0; col < 100; col++)
		{
			Uint32* pixel = (Uint32*)scr->pixels + (row * scr->pitch / 4) + col;
			*pixel = 65536 * 255; // red
		}
	}

	//pause until you press escape and meanwhile redraw screen
	SDL_Event event;
	int done = 0;
	while(done == 0)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) done = 2;
			else if(SDL_GetKeyState(NULL)[SDLK_ESCAPE]) done = 2;
			else if(event.type == SDL_KEYDOWN) done = 1; //press any other key for next image
		}
		SDL_UpdateRect(scr, 0, 0, 0, 0); //redraw screen
		SDL_Delay(5); //pause 5 ms so it consumes less processing power
	}

	SDL_Quit();
	//return done == 2 ? 1 : 0;
}

int main(int argc, char* argv[])
{
	//showSdlSurface();
	//return 0;

	MSG        msg;

	std::string bmpFileName;
	if ( argc != 2 ) {
		bmpFileName = "..\\img\\kiev_sat01.bmp";
	} else {
		bmpFileName = argv[1];
	}

	ModelWindow wnd(bmpFileName);
	
	while( GetMessage(&msg, NULL, 0, 0) )
	{
		if(!IsDialogMessage(SettingsDialog::getCurrent(),&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

