#ifndef API_WINDOW_H
#define API_WINDOW_H

#include <map>
#include <vector>
#include <exception>
#include <Windows.h>

#include "rl_ptr.h"
#include "BasicDefs.h"
#include "Mutex.h"

class SettingsDialog;
class ShockwaveRenderer;

class WindowException: public std::exception {};

class ModelWindow
{
	typedef std::map<HWND, ModelWindow*> WndStorage;
public:
						ModelWindow(const std::string	aBmpFileName);
	virtual				~ModelWindow(void);

	virtual LRESULT		wndProc(UINT					Msg, 
								WPARAM					wParam, 
								LPARAM					lParam);

	HWND getHandle();

	const Mutex&		getPaintMutex();
private:
	static WndStorage	windows;

	HWND							hWnd;
	HDC								myRenderDc;
	Mutex							myPaintMutex;
	koki::rl_ptr<ShockwaveRenderer> myRenderer;
//	koki::rl_ptr<SettingsDialog>	mySettingsDlg;
	SHARED_PTR(SettingsDialog)		mySettingsDlg;

	void						invalidateRect(const RECT*	aRect);

	static LRESULT CALLBACK		clsWndProc(HWND				hWnd, 
											UINT			Msg, 
											WPARAM			wParam, 
											LPARAM			lParam);
};

#endif // API_WINDOW_H