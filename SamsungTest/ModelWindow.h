#ifndef API_WINDOW_H
#define API_WINDOW_H

#include <map>
#include <vector>
#include <exception>
#include <Windows.h>

class WindowException: public std::exception {};

// Class encapsulates WIN API window creation and management of ModelEnvironment and thrown object
class ModelWindow
{
	typedef std::map<HWND, ModelWindow*> WndStorage;
public:
	ModelWindow();
	virtual ~ModelWindow(void);

	virtual LRESULT wndProc(UINT Msg, WPARAM wParam, LPARAM lParam);

	void loadBitmap(std::string aBmpFileName);

	HWND getHandle();

	void setBitmapToDraw(HDC aDc, int aHeight, int aWidth);
	void lockPaint();
	void unlockPaint();
	HANDLE getPaintMutex();
private:
	static WndStorage windows;

	HWND hWnd;
	UINT_PTR timer;
	HDC hdc;
	HBITMAP myBmp;
	HDC myDrawHdc;
	int myDrawHeight;
	int myDrawWidth;
	HANDLE myPaintMutex;


	static LRESULT CALLBACK clsWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	void setTimer();
	void killTimer();
};

#endif // API_WINDOW_H