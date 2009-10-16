#ifndef _UI_BROWSECFG_H
#define _UI_BROWSECFG_H

// include the MZFC library header file
#include <mzfc_inc.h>
#include "../MzCommon/MzConfig.h"
// Main window derived from CMzWndEx

class CalendarConfig : public AppConfigIni{
public:
	CalendarConfig()
		:AppConfigIni(L"m8calendar.ini"){
            InitIniKey();
	}
protected:
	void InitIniKey(){
		IniJieqiOrder.InitKey(L"Config",L"JieqiOrder",0);
        IniHistodayFontSize.InitKey(L"Config",L"HisFontSize",0);
	}
public:
	MzConfig IniJieqiOrder;//1: 以节气交界日为起点 0: 以农历初一为起点
    MzConfig IniHistodayFontSize;   //0:小 1: 中 2: 大
};

class Ui_ConfigWnd : public CMzWndEx {
    MZ_DECLARE_DYNAMIC(Ui_ConfigWnd);
public:
	Ui_ConfigWnd();
	void updateUi();
public:
    UiToolbar_Text m_Toolbar;
	UiCaption m_lblTitle;
    UiButtonEx m_BtnJieqi;	//节气排列方式
    UiButtonEx m_BtnFontSize;	//浏览字体大小

protected:
    // Initialization of the window (dialog)
    virtual BOOL OnInitDialog();

    // override the MZFC command handler
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
private:
    void ShowJieqiOptionDlg();
    void ShowFontSizeOptionDlg();
};


#endif /*_UI_BROWSECFG_H*/