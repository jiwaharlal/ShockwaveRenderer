#include "ModelWindow.h"
#include "MutexLock.h"

#include "BindMember.h"
#include "ShockwaveRenderer.h"
#include "SettingsDialog.h"

ModelWindow::WndStorage ModelWindow::windows;

ModelWindow::ModelWindow(const std::string	aBmpFileName) 
	: myRenderDc(NULL)
	//, mySettingsDlg(new SettingsDialog())
{	
	WNDCLASSEX WndClsEx;
	LPCTSTR ClsName = L"SamsungTest";
	LPCTSTR WndName = L"Render shockwaves";

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

	myRenderDc = CreateCompatibleDC(GetWindowDC(hWnd));

	myRenderer = koki::rl_ptr<ShockwaveRenderer>(new ShockwaveRenderer(
														aBmpFileName, 
														myRenderDc, //hWnd,
														myPaintMutex, 
														bindMember(this, &ModelWindow::invalidateRect)));
	mySettingsDlg = SHARED_PTR(SettingsDialog)(new SettingsDialog(hWnd));
	
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

void ModelWindow::invalidateRect(const RECT* aRect)
{
	InvalidateRect(hWnd, aRect, false);
}

const Mutex& ModelWindow::getPaintMutex()
{
	return myPaintMutex;
}

// select target window between class instances and call it's wndProc
LRESULT CALLBACK  ModelWindow::clsWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if ( windows.find(hWnd) != windows.end() ) {
		return windows[hWnd]->wndProc(Msg, wParam, lParam);
	}

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT ModelWindow::wndProc(UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch(Msg)
    {
	case WM_LBUTTONUP:
		myRenderer->switchMultithreading();
		
		break;
	case WM_RBUTTONUP:
		mySettingsDlg->Show();
		break;
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
	case WM_PAINT:
		PAINTSTRUCT 	ps;
    	HDC 			hdc;

		if (myRenderDc == NULL) {
			break;
		}

		RECT r;
		GetClientRect(hWnd, &r);
		hdc = BeginPaint(hWnd, &ps);
		{
			MutexLock lock(myPaintMutex.handle());
			BitBlt(hdc, 0, 0, r.right, r.bottom, myRenderDc, 0, 0, SRCCOPY);
		}
		EndPaint(hWnd, &ps);
		
		break;
    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

	return 0;
}
