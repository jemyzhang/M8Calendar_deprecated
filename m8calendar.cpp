#include "m8calendar.h"
#include "..\MzCommon\MzCommon.h"
//#include "ui_reminder.h"
//#include "ui_password.h"
//#include <MotorVibrate.h>

using namespace MzCommon;
// The global variable of the application.
M8CashApp theApp;
//CashReminder calendar_reminder;

BOOL M8CashApp::Init() {
    // Init the COM relative library.
    CoInitializeEx(0, COINIT_MULTITHREADED);
#if 0
	// ��������
	cash_reminder.loadReminderList();

	// �ж��Ƿ������ѵ���
	LPWSTR str = GetCommandLine();
	//str = L"AppRunToHandleNotification 0x360000B1";
	int handle = 0;
	wchar_t prestr[1024];
	if(lstrlen(str)){
		swscanf(str,L"AppRunToHandleNotification 0x%x",&handle);
		ReminderInfo_ptr p = cash_reminder.getReminderByEventId(handle);
		cash_reminder.setNextReminder(p);
		cash_reminder.saveReminderList();
		wchar_t tmp[1024];
		if(p && p->text.C_Str()){
			C::restoreWrap(tmp,p->text.C_Str());
			wsprintf(prestr,L"M8Cash��������:\n%s",tmp);
		}else{
			wsprintf(prestr,L"M8Cash��������");
		}
		MzSetVibrateOn(MZ_VIBRATE_ON_TIME,MZ_VIBRATE_OFF_TIME);
		//SetBackLightState(true);	//������
		//if(IsLockPhoneStatus()){	//����
			//MzLeaveLockPhone();
		//}
		while(MzMessageBoxEx(0, prestr, L"Exit", MB_OK) != 1);
		MzSetVibrateOff();
		PostQuitMessage(0);
		return true;
	}
#endif
	//������������
	//�������Ƿ��Ѿ�����
	HANDLE m_hCHDle = CreateMutex(NULL,true,L"M8Calendar");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND pWnd=FindWindow(m_MainWnd.GetMzClassName(),NULL);
		//HWND pWnd=FindWindow(NULL,L"M8Cash");
		if(pWnd)
		{
			SetForegroundWindow(pWnd);
			PostMessage(pWnd,WM_NULL,NULL,NULL);
		}
		PostQuitMessage(0);
		return true; 
	}
	// Create the main window
	RECT rcWork = MzGetWorkArea();
	m_MainWnd.Create(rcWork.left, rcWork.top, RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork), 0, 0, 0);
	m_MainWnd.Show();

    // return TRUE means init success.
    return TRUE;
}