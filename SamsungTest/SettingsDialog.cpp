#include "SettingsDialog.h"

#include <Commctrl.h>

#include "resource.h"

std::map<HWND, SettingsDialog*>	SettingsDialog::myDialogs;

SettingsDialog::SettingsDialog(HWND aParent)
{
	HINSTANCE moduleInstance = (HINSTANCE)GetModuleHandle(NULL);
	InitCommonControls();
	//DialogBox(moduleInstance, MAKEINTRESOURCE(IDD_DIALOG1), aParent, clsDialogProc);
	myHandle = CreateDialog(moduleInstance, MAKEINTRESOURCE(IDD_DIALOG1), aParent, clsDialogProc);
	myDialogs[myHandle] = this;
}

SettingsDialog::~SettingsDialog(void)
{
	myDialogs.erase(myDialogs.find(myHandle));
}

void
SettingsDialog::Show()
{
	ShowWindow(myHandle, SW_SHOWNORMAL);
	UpdateWindow(myHandle);
}

INT_PTR  CALLBACK 
SettingsDialog::clsDialogProc(
	HWND	aHandle, 
	UINT	aMsg, 
	WPARAM	aWParam, 
	LPARAM	aLParam )
{
	return myDialogs[aHandle]->dialogProc(aMsg, aWParam, aLParam);
}

INT_PTR 
SettingsDialog::dialogProc(
	UINT	aMsg, 
	WPARAM	aWParam, 
	LPARAM	aLParam )
{
	switch(aMsg)
	{
	case WM_COMMAND:
		switch(LOWORD(aWParam))
		{
			case IDCANCEL:
				printf("cancel pressed\n\r");
				ShowWindow(myHandle, SW_HIDE);
				return TRUE;
			case IDOK:
				printf("ok pressed\n\r");
				break;
			default:
				break;
		}
		break;
	case WM_CLOSE:
		printf("close pressed\n\r");
		break;
	default:
		break;
	}
	//return DefDlgProc(myHandle, aMsg, aWParam, aLParam);
	return NULL;
}
