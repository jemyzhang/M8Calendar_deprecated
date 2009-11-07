#include "m8calendar.h"
#include "..\MzCommon\MzCommon.h"
//#include "ui_reminder.h"
//#include "ui_password.h"
//#include <MotorVibrate.h>
#include "ui_config.h"
#include "ui_today.h"
#include "ui_calendar.h"
#include "ui_today_da.h"

using namespace MzCommon;
// The global variable of the application.
M8CalendarApp theApp;
//CashReminder calendar_reminder;

extern CalendarConfig AppConfig;

BOOL M8CalendarApp::Init() {
    // Init the COM relative library.
    CoInitializeEx(0, COINIT_MULTITHREADED);

	// �ж��Ƿ������ѵ���
	LPWSTR str = GetCommandLine();
	if(lstrlen(str)){
        if(wcscmp(str,L"-da") == 0){
			//����Ƿ��Ѿ���
			m_pShowWnd = new Ui_TodayDAWnd;

			HANDLE m_hCHDle = CreateMutex(NULL,true,L"M8CalendarDlg");
			if(GetLastError() == ERROR_ALREADY_EXISTS)
			{
				HWND pWnd=FindWindow(m_pShowWnd->GetMzClassName(),NULL);
				if(pWnd)
				{
					PostMessage(pWnd,MZFC_WM_MESSAGE_QUIT,NULL,NULL);
				}
				PostQuitMessage(0);
				return true; 
			}
            int dlgX,dlgY;
	        RECT rcWork = MzGetWorkArea();
            //����Ƿ����
            DEVMODE settings;
            memset(&settings, 0, sizeof(DEVMODE));
            settings.dmSize = sizeof(DEVMODE);

            settings.dmFields = DM_DISPLAYORIENTATION;
            ChangeDisplaySettingsEx(NULL, &settings, NULL, CDS_TEST, NULL);
            if(settings.dmDisplayOrientation == 0 || settings.dmDisplayOrientation == 2){
                //����
                dlgX = rcWork.left + 140;
                dlgY = 0;
            }else{
                dlgX = rcWork.left + 20;
                dlgY = rcWork.top + 50;
            }
            //����

            m_pShowWnd->Create(dlgX, dlgY, 440, 460, 0, 0, 0,WS_EX_TOPMOST);
            m_pShowWnd->SetAnimateType_Show(MZ_ANIMTYPE_NONE);
            m_pShowWnd->SetAnimateType_Hide(MZ_ANIMTYPE_FADE);
            m_pShowWnd->Show();
		    return true;
        }
	}

    //������������
	//�������Ƿ��Ѿ�����
    if(AppConfig.IniStartupPage.Get() != 0){
        m_pShowWnd = new Ui_TodayWnd;
    }else{
        m_pShowWnd = new Ui_CalendarWnd;
    }
	HANDLE m_hCHDle = CreateMutex(NULL,true,L"M8Calendar");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND pWnd=FindWindow(m_pShowWnd->GetMzClassName(),NULL);
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

    m_pShowWnd->Create(rcWork.left, rcWork.top, RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork), 0, 0, 0);
    m_pShowWnd->SetAnimateType_Show(MZ_ANIMTYPE_ZOOM_IN);
    m_pShowWnd->SetAnimateType_Hide(MZ_ANIMTYPE_NONE);
    m_pShowWnd->Show();

    // return TRUE means init success.
    return TRUE;
}