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
	}
public:
	MzConfig IniJieqiOrder;//1: �Խ���������Ϊ��� 0: ��ũ����һΪ���
};

class Ui_ConfigWnd : public CMzWndEx {
    MZ_DECLARE_DYNAMIC(Ui_ConfigWnd);
public:
	Ui_ConfigWnd();
	void updateUi();
public:
    UiScrollWin m_ScrollWin;
    UiToolbar_Text m_Toolbar;
	UiCaption m_lblTitle;
    UiButtonEx m_BtnJieqi;	//�������з�ʽ
	UiList	m_DetailList;	//�����б�

protected:
    // Initialization of the window (dialog)
    virtual BOOL OnInitDialog();

    // override the MZFC window messages handler
    LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam);

    // override the MZFC command handler
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
private:
	int _JieqiOrder;
	int _viewMode;
};


#endif /*_UI_BROWSECFG_H*/