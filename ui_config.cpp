#include "ui_config.h"
#include "..\MzCommon\MzCommon.h"
#include "..\MzCommon\UiSingleOption.h"

using namespace MzCommon;

#define MZ_IDC_TOOLBAR_MAIN 101
#define MZ_IDC_SCROLLWIN 102

#define MZ_IDC_BUTTON_JIEQI_MODE 103
#define MZ_IDC_LIST_CONFIG 104
#define MZ_IDC_BUTTON_FONT_SIZE 105

CalendarConfig AppConfig;

MZ_IMPLEMENT_DYNAMIC(Ui_ConfigWnd)

const wchar_t* JIEQIMODESTR[] = {
	L"��ũ����һΪ���",
	L"�Խ���������Ϊ���",
};

const wchar_t* FONTSIZESTR[] = {
	L"С����",
	L"������",
	L"������",
};

Ui_ConfigWnd::Ui_ConfigWnd(){
}

BOOL Ui_ConfigWnd::OnInitDialog() {
    // Must all the Init of parent class first!
    if (!CMzWndEx::OnInitDialog()) {
        return FALSE;
    }

    ImagingHelper *imgArrow = ImagingHelper::GetImageObject(GetMzResModuleHandle(), MZRES_IDR_PNG_ARROW_RIGHT, true);
    // Then init the controls & other things in the window
    int y = 0;
	m_lblTitle.SetPos(0,y,GetWidth(),MZM_HEIGHT_CAPTION);
	m_lblTitle.SetText(L"����");
    AddUiWin(&m_lblTitle);

	y += MZM_HEIGHT_CAPTION;
    m_BtnJieqi.SetPos(0, y, GetWidth(), MZM_HEIGHT_BUTTONEX);
    m_BtnJieqi.SetText(L"��֧����");
    m_BtnJieqi.SetButtonType(MZC_BUTTON_LINE_BOTTOM);
    m_BtnJieqi.SetID(MZ_IDC_BUTTON_JIEQI_MODE);
    m_BtnJieqi.SetImage2(imgArrow);
    m_BtnJieqi.SetImageWidth2(imgArrow->GetImageWidth());
    m_BtnJieqi.SetShowImage2(true);
    AddUiWin(&m_BtnJieqi);

	y += MZM_HEIGHT_BUTTONEX;
    m_BtnFontSize.SetPos(0, y, GetWidth(), MZM_HEIGHT_BUTTONEX);
    m_BtnFontSize.SetText(L"��ʷ�ϵĽ��������С");
    m_BtnFontSize.SetTextMaxLen(0);
    m_BtnFontSize.SetButtonType(MZC_BUTTON_LINE_BOTTOM);
    m_BtnFontSize.SetID(MZ_IDC_BUTTON_FONT_SIZE);
    m_BtnFontSize.SetImage2(imgArrow);
    m_BtnFontSize.SetImageWidth2(imgArrow->GetImageWidth());
    m_BtnFontSize.SetShowImage2(true);
    AddUiWin(&m_BtnFontSize);

	m_Toolbar.SetPos(0, GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);
	m_Toolbar.SetButton(0, true, true, L"����");
	m_Toolbar.EnableLeftArrow(true);
    m_Toolbar.SetID(MZ_IDC_TOOLBAR_MAIN);
    AddUiWin(&m_Toolbar);

	updateUi();

    return TRUE;
}

void Ui_ConfigWnd::updateUi(){
	m_BtnJieqi.SetText2(JIEQIMODESTR[AppConfig.IniJieqiOrder.Get()]);
	m_BtnJieqi.Invalidate();
	m_BtnJieqi.Update();

    m_BtnFontSize.SetText2(FONTSIZESTR[AppConfig.IniHistodayFontSize.Get()]);
	m_BtnFontSize.Invalidate();
	m_BtnFontSize.Update();

}

void Ui_ConfigWnd::OnMzCommand(WPARAM wParam, LPARAM lParam) {
    UINT_PTR id = LOWORD(wParam);
    switch (id) {
		case MZ_IDC_BUTTON_JIEQI_MODE:
            ShowJieqiOptionDlg();
			break;
		case MZ_IDC_BUTTON_FONT_SIZE:
            ShowFontSizeOptionDlg();
			break;
        case MZ_IDC_TOOLBAR_MAIN:
        {
            int nIndex = lParam;
            if (nIndex == 0) {
                // exit the modal dialog
                EndModal(ID_OK);
                return;
            }
            break;
		}
    }
}

void Ui_ConfigWnd::ShowJieqiOptionDlg(){
    Ui_SingleOptionWnd dlg;
    for(int i = 0; i < sizeof(JIEQIMODESTR)/sizeof(JIEQIMODESTR[0]); i++){
        dlg.AppendOptionItem(const_cast<LPTSTR>(JIEQIMODESTR[i]));
    }
    dlg.SetSelectedIndex(AppConfig.IniJieqiOrder.Get());
    dlg.SetTitleText(L"�趨��֧���·�ʽ");
    RECT rcWork = MzGetWorkArea();
    dlg.Create(rcWork.left + 40, rcWork.top + 120, RECT_WIDTH(rcWork) - 80, RECT_HEIGHT(rcWork) - 240,
        m_hWnd, 0, WS_POPUP);
    // set the animation of the window
    dlg.SetAnimateType_Show(MZ_ANIMTYPE_NONE);
    dlg.SetAnimateType_Hide(MZ_ANIMTYPE_FADE);
    int nRet = dlg.DoModal();
    if(nRet == ID_OK){
        AppConfig.IniJieqiOrder.Set(dlg.GetSelectedIndex());
        updateUi();
    }
}

//��ʷ�ϵĽ������ִ�С
void Ui_ConfigWnd::ShowFontSizeOptionDlg(){
    Ui_SingleOptionWnd dlg;
    for(int i = 0; i < sizeof(FONTSIZESTR)/sizeof(FONTSIZESTR[0]); i++){
        dlg.AppendOptionItem(const_cast<LPTSTR>(FONTSIZESTR[i]));
    }
    dlg.SetSelectedIndex(AppConfig.IniHistodayFontSize.Get());
    dlg.SetTitleText(L"�趨������ִ�С");
    RECT rcWork = MzGetWorkArea();
    dlg.Create(rcWork.left + 40, rcWork.top + 120, RECT_WIDTH(rcWork) - 80, RECT_HEIGHT(rcWork) - 240,
        m_hWnd, 0, WS_POPUP);
    // set the animation of the window
    dlg.SetAnimateType_Show(MZ_ANIMTYPE_NONE);
    dlg.SetAnimateType_Hide(MZ_ANIMTYPE_FADE);
    int nRet = dlg.DoModal();
    if(nRet == ID_OK){
        AppConfig.IniHistodayFontSize.Set(dlg.GetSelectedIndex());
        updateUi();
    }
}