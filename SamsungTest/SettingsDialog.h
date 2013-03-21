#pragma once

#include <windows.h>
#include <map>

class SettingsDialog
{
public:
							SettingsDialog(HWND aParent);
							~SettingsDialog(void);

	void					show();
	void					hide();
	void					loadSettings();
	void					saveSettings();

	static HWND				getCurrent();

private:
	HWND									myHandle;
	static HWND								myDlgCurrent;
	static std::map<HWND, SettingsDialog*>	myDialogs;

	std::string					getControlText( int aControlId );
	static INT_PTR  CALLBACK	clsDialogProc(HWND aHandle, UINT aMsg, WPARAM aWParam, LPARAM aLParam);
	INT_PTR 					dialogProc(UINT aMsg, WPARAM aWParam, LPARAM aLParam);
};
