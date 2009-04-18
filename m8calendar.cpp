#include "m8calendar.h"
#include "mz_commonfunc.h"
//#include "ui_reminder.h"
//#include "ui_password.h"
#include <MotorVibrate.h>

using namespace MZ_CommonFunc;
// The global variable of the application.
M8CashApp theApp;
wchar_t db_path[256];
clsCASHDB calendar_db;
//CashReminder calendar_reminder;

bool M8CashApp::checkpwd(){
	wchar_t currpath[128];
	bool ret = true;
	if(File::GetCurrentPath(currpath)){
		wsprintf(db_path,L"%s\\calendar.db",currpath);
	}else{
		wsprintf(db_path,DEFAULT_DB);
	}
	if(calendar_db.connect(db_path)){
		//popup password dialog
		//try if there is a password
		calendar_db.decrypt(NULL,0);
		//检查记录版本
		calendar_db.versionUpdate(m_MainWnd.m_hWnd);
		calendar_db.recover();
		if(!calendar_db.load()){
			ret = false;
		}
	}else{
		//检查记录版本
		calendar_db.versionUpdate(m_MainWnd.m_hWnd);
		calendar_db.recover();
		calendar_db.load();
	}
	return ret;
}

BOOL M8CashApp::Init() {
    // Init the COM relative library.
    CoInitializeEx(0, COINIT_MULTITHREADED);
#if 0
	// 载入提醒
	cash_reminder.loadReminderList();

	// 判断是否是提醒调用
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
			wsprintf(prestr,L"M8Cash财务提醒:\n%s",tmp);
		}else{
			wsprintf(prestr,L"M8Cash财务提醒");
		}
		MzSetVibrateOn(MZ_VIBRATE_ON_TIME,MZ_VIBRATE_OFF_TIME);
		//SetBackLightState(true);	//开背光
		//if(IsLockPhoneStatus()){	//解锁
			//MzLeaveLockPhone();
		//}
		while(MzMessageBoxEx(0, prestr, L"Exit", MB_OK) != 1);
		MzSetVibrateOff();
		PostQuitMessage(0);
		return true;
	}
#endif
	//正常启动程序
	//检测程序是否已经运行
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
	//SetForegroundWindow(m_MainWnd.m_hWnd);
	//检测密码
	//if(checkpwd()){
//		m_MainWnd.updateText();
	//}else{
	//	PostQuitMessage(0);
	//}

    // return TRUE means init success.
    return TRUE;
}