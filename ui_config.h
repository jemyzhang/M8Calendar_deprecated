#ifndef _UI_BROWSECFG_H
#define _UI_BROWSECFG_H

// include the MZFC library header file
#include <mzfc_inc.h>

// Main window derived from CMzWndEx

class Ui_ConfigWnd : public CMzWndEx {
    MZ_DECLARE_DYNAMIC(Ui_ConfigWnd);
public:
	Ui_ConfigWnd();
	void updateUi();
	static int getJieqiMode();
	static void setJieqiMode(int);
public:
    UiScrollWin m_ScrollWin;
    UiToolbar_Text m_Toolbar;
	UiCaption m_lblTitle;
    UiButtonEx m_BtnJieqi;	//节气排列方式
	UiList	m_DetailList;	//配置列表

protected:
    // Initialization of the window (dialog)
    virtual BOOL OnInitDialog();

    // override the MZFC window messages handler
    LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam);

    // override the MZFC command handler
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
private:
	static int _jieqiShowMode;	//1: 以节气交界日为起点 0: 以农历初一为起点
	int _viewMode;
};


#endif /*_UI_BROWSECFG_H*/