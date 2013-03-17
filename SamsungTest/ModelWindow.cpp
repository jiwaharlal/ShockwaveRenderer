#include "ModelWindow.h"
#include "MutexLock.h"

#include "BindMember.h"

ModelWindow::WndStorage ModelWindow::windows;

ModelWindow::ModelWindow() 
	: timer(NULL)
	, hdc(NULL)
	, myBmp(NULL)
	, myDrawHdc(NULL)
{	
	WNDCLASSEX WndClsEx;
	LPCTSTR ClsName = L"UbisoftTest";
	LPCTSTR WndName = L"Throw the ball";

	// Create the application window
	WndClsEx.cbSize        = sizeof(WNDCLASSEX);
	WndClsEx.style         = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc   = ModelWindow::clsWndProc;
	WndClsEx.cbClsExtra    = 0;
	WndClsEx.cbWndExtra    = 0;
	WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = ClsName;
	WndClsEx.hInstance     = NULL;
	WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	// Register the application
	RegisterClassEx(&WndClsEx);

	// Create the window object
	hWnd = CreateWindow(ClsName,
			  WndName,
			  WS_OVERLAPPEDWINDOW,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  NULL,
			  NULL,
			  NULL, //hInstance,
			  NULL);
	
	// Find out if the window was created
	if( !hWnd ) {
		throw WindowException();
	}

	windows[hWnd] = this;

	// Display the window to the user
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	myPaintMutex = CreateMutex(NULL, FALSE, L"paintMutex");

	Functor<std::string> f = BindMember(this, &ModelWindow::loadBitmap);

	f("some bitmap");
}

ModelWindow::~ModelWindow(void)
{
	WndStorage::iterator it = windows.find(hWnd);
	if ( it != windows.end() ) {
		windows.erase(it);
	}
}

HWND ModelWindow::getHandle()
{
	return hWnd;
}

void ModelWindow::lockPaint()
{
	DWORD waitResult;
	do {
		waitResult = WaitForSingleObject(myPaintMutex, INFINITE);
	} while (waitResult != WAIT_OBJECT_0);
	
	//myPaintMutex = CreateMutex(NULL, TRUE, L"paintMutex");
}

void ModelWindow::unlockPaint()
{
	ReleaseMutex(myPaintMutex);
}

HANDLE ModelWindow::getPaintMutex()
{
	return myPaintMutex;
}

void ModelWindow::setBitmapToDraw(HDC aDc, int aHeight, int aWidth) {
	myDrawHdc = aDc;
	myDrawHeight = aHeight;
	myDrawWidth = aWidth;
}

void ModelWindow::loadBitmap(std::string aBmpFileName) {
	myBmp = (HBITMAP)LoadImageA(0, "d:\\tmp\\bmp1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (myBmp == NULL) {
		DWORD errorCode = GetLastError();
		printf("Cannot load image - error 0x%x", errorCode);
	}
	RECT r;
	GetClientRect(hWnd, &r);
	InvalidateRect(hWnd, &r, true);
}

// select target window between class instances and call it's wndProc
LRESULT CALLBACK  ModelWindow::clsWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if ( windows.find(hWnd) != windows.end() ) {
		return windows[hWnd]->wndProc(Msg, wParam, lParam);
	}

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT ModelWindow::wndProc(UINT Msg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT		ps;
	POINT wndPoint;
	RECT rect;

    switch(Msg)
    {
	case WM_CREATE:
		loadBitmap("");
		break;
	case WM_LBUTTONDOWN:
		loadBitmap("");
		break;
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
	case WM_TIMER:
		// Define region to repaint
		/*p = ball->getPrevPosition();
		wndPoint = modelToClient(p);
		rect.bottom = wndPoint.y + eraseRadix;
		rect.left = wndPoint.x - eraseRadix;
		rect.right = wndPoint.x + eraseRadix;
		rect.top = wndPoint.y - eraseRadix;
		
		// send message to repaint region
		InvalidateRect(hWnd, &rect, true);*/
		break;
	case WM_PAINT:

		PAINTSTRUCT 	ps;
    	HDC 			hdc;

		if (myDrawHdc == NULL) {
			break;
		}

		hdc = BeginPaint(hWnd, &ps);
		//lockPaint();
		{
			MutexLock lock(myPaintMutex);

    		BitBlt(hdc, 0, 0, myDrawWidth, myDrawHeight, myDrawHdc, 0, 0, SRCCOPY);
		}

		EndPaint(hWnd, &ps);
		//unlockPaint();
    	break;
    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

	return 0;
}

void ModelWindow::setTimer() {
	timer = SetTimer(hWnd, timer, 10, NULL);
}

void ModelWindow::killTimer() {
	KillTimer(hWnd, timer);
}
