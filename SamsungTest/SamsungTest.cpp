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

int main(int argc, char* argv[])
{
	MSG        Msg;

	std::string bmpFileName;
	if ( argc != 2 ) {
		bmpFileName = "..\\img\\kiev_sat01.bmp";
	} else {
		bmpFileName = argv[1];
	}

	ModelWindow wnd(bmpFileName);
	
	while( GetMessage(&Msg, NULL, 0, 0) )
	{
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
	}

	return 0;
}

