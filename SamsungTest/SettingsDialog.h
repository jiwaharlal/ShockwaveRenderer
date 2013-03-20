#pragma once

#include <windows.h>
#include <map>

class SettingsDialog
{
public:
							SettingsDialog(HWND aParent);
							~SettingsDialog(void);

	void					Show();
private:
	HWND									myHandle;
	static std::map<HWND, SettingsDialog*>	myDialogs;

	static INT_PTR  CALLBACK clsDialogProc(HWND aHandle, UINT aMsg, WPARAM aWParam, LPARAM aLParam);
	INT_PTR 					dialogProc(UINT aMsg, WPARAM aWParam, LPARAM aLParam);
};
