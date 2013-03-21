#include "SettingsDialog.h"

#include <Commctrl.h>
#include <string>

#include "resource.h"
#include "Settings.h"
#include "BasicDefs.h"

std::map<HWND, SettingsDialog*>	SettingsDialog::myDialogs;
HWND SettingsDialog::myDlgCurrent;

SettingsDialog::SettingsDialog(HWND aParent)
{
	myHandle = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG1), aParent, clsDialogProc);
	myDialogs[myHandle] = this;
	myDlgCurrent = myHandle;

	/*MSG msg;
	while(GetMessage(&msg,0,0,0)>0)
	{
		if(!IsDialogMessage(myDlgCurrent,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/
}

SettingsDialog::~SettingsDialog(void)
{
	myDialogs.erase(myDialogs.find(myHandle));
}

HWND
SettingsDialog::getCurrent()
{
	return myDlgCurrent;
}

void
SettingsDialog::show()
{
	loadSettings();
	ShowWindow(myHandle, SW_SHOWNORMAL);
	UpdateWindow(myHandle);
}

void
SettingsDialog::hide()
{
	ShowWindow(myHandle, SW_HIDE);
}

void
SettingsDialog::loadSettings()
{
	Settings& settings = Settings::instance();
	HWND control;
	
	control = GetDlgItem(myHandle, ID_MAX_RADIUS_EDIT);
	SetWindowTextA(control, toStr(settings.getMaxRadius()).c_str());
	control = GetDlgItem(myHandle, ID_MIN_RADIUS_EDIT);
	SetWindowTextA(control, toStr(settings.getMinRadius()).c_str());
	control = GetDlgItem(myHandle, ID_WAVE_SPEED_EDIT);
	SetWindowTextA(control, toStr(settings.getWaveSpeed()).c_str());
	UINT checked = settings.isBlastsParallel() ? BST_CHECKED : BST_UNCHECKED;
	CheckDlgButton(myHandle, ID_PARALLEL_BLASTS_CHECKBOX, checked);
	checked = settings.isMultithreaded() ? BST_CHECKED : BST_UNCHECKED;
	CheckDlgButton(myHandle, ID_MULTITHREADED_CHECKBOX, checked);
}

std::string
SettingsDialog::getControlText( int aControlId )
{
	char buffer[128];
	HWND control;
	control = GetDlgItem(myHandle, aControlId);
	GetWindowTextA(control, buffer, 128);
	return std::string(buffer);
}

void
SettingsDialog::saveSettings()
{
	Settings& settings = Settings::instance();
	//HWND control;
	
	settings.setMaxRadius(atoi(getControlText(ID_MAX_RADIUS_EDIT).c_str()));
	settings.setMinRadius(atoi(getControlText(ID_MIN_RADIUS_EDIT).c_str()));
	settings.setWaveSpeed(atoi(getControlText(ID_WAVE_SPEED_EDIT).c_str()));

	UINT checked = IsDlgButtonChecked(myHandle, ID_PARALLEL_BLASTS_CHECKBOX);
	settings.setIsBlastsParallel(checked == BST_CHECKED);

	checked = IsDlgButtonChecked(myHandle, ID_MULTITHREADED_CHECKBOX);
	settings.setIsMultithreaded(checked == BST_CHECKED);
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
	/*MSG msg;
	while(GetMessage(&msg,0,0,0)>0)
	{
		if(!IsDialogMessage(myDlgCurrent,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/
	switch(aMsg)
	{
	case WM_COMMAND:
		switch(LOWORD(aWParam))
		{
			case IDCANCEL:
				printf("cancel pressed\n\r");
				hide();
				return TRUE;
			case IDOK:
				printf("ok pressed\n\r");
				saveSettings();
				hide();
				break;
			default:
				break;
		}
		break;
	/*case WM_ACTIVATE:
		if (0 == aWParam)             // becoming inactive
			myDlgCurrent = NULL;
		else                         // becoming active
			myDlgCurrent = myHandle;*/
	case WM_CLOSE:
		printf("close pressed\n\r");
		hide();
		break;
	default:
		break;
	}
	//return DefDlgProc(myHandle, aMsg, aWParam, aLParam);
	return NULL;
}
