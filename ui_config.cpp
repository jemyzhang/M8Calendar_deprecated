#include "ui_config.h"
#include "mz_commonfunc.h"
using namespace MZ_CommonFunc;

#include "ReadWriteIni.h"
#define MZ_IDC_TOOLBAR_MAIN 101
#define MZ_IDC_SCROLLWIN 102

#define MZ_IDC_BUTTON_JIEQI_MODE 103
#define MZ_IDC_LIST_CONFIG 104

MZ_IMPLEMENT_DYNAMIC(Ui_ConfigWnd)

const wchar_t* JIEQIMODESTR[] = {
	L"以农历初一为起点",
	L"以节气交界日为起点",
};

int Ui_ConfigWnd::_jieqiShowMode = 0;

Ui_ConfigWnd::Ui_ConfigWnd(){
	_viewMode = 0;
	//从数据库载入设置
	//TODO
	_jieqiShowMode = getJieqiMode();
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
	m_lblTitle.SetText(L"设置");
    AddUiWin(&m_lblTitle);

	y += MZM_HEIGHT_CAPTION;
    m_BtnJieqi.SetPos(0, y, GetWidth(), MZM_HEIGHT_BUTTONEX);
    m_BtnJieqi.SetText(L"干支纪月");
    m_BtnJieqi.SetButtonType(MZC_BUTTON_LINE_BOTTOM);
    m_BtnJieqi.SetID(MZ_IDC_BUTTON_JIEQI_MODE);
    m_BtnJieqi.SetImage2(imgArrow);
    m_BtnJieqi.SetImageWidth2(imgArrow->GetImageWidth());
    m_BtnJieqi.SetShowImage2(true);
    AddUiWin(&m_BtnJieqi);

	y+=MZM_HEIGHT_BUTTONEX;
    m_ScrollWin.SetPos(0, y, GetWidth(), GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR);
    m_ScrollWin.SetID(MZ_IDC_SCROLLWIN);
    m_ScrollWin.EnableScrollBarV(true);
    AddUiWin(&m_ScrollWin);

	m_DetailList.SetPos(0, 0, GetWidth(), GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR);
    m_DetailList.SetID(MZ_IDC_LIST_CONFIG);
	m_DetailList.SetItemHeight(50);
	m_DetailList.SetTextSize(m_DetailList.GetTextSize() - 4);
	m_DetailList.SetDrawTextFormat(DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	m_DetailList.EnableScrollBarV(true);
	m_DetailList.EnableNotifyMessage(true);
	m_ScrollWin.AddChild(&m_DetailList);

	m_Toolbar.SetPos(0, GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);
	m_Toolbar.SetButton(0, true, true, L"取消");
	m_Toolbar.SetButton(2, true, true, L"确定");
	m_Toolbar.EnableLeftArrow(true);
    m_Toolbar.SetID(MZ_IDC_TOOLBAR_MAIN);
    AddUiWin(&m_Toolbar);

	updateUi();

    return TRUE;
}

void Ui_ConfigWnd::updateUi(){
	m_BtnJieqi.SetText2(JIEQIMODESTR[_jieqiShowMode]);
	m_BtnJieqi.Invalidate();
	m_BtnJieqi.Update();

	if(_viewMode == 0){
		m_ScrollWin.SetVisible(false);
	}
	if(_viewMode == 1){
	    ListItem li;
		CMzString str;
		m_DetailList.RemoveAll();
		for(int i = 0; i < 2; i++){
			str = JIEQIMODESTR[i];
			li.Text = str;
			m_DetailList.AddItem(li);
		}
		m_ScrollWin.SetPos(0, MZM_HEIGHT_BUTTONEX + MZM_HEIGHT_CAPTION, GetWidth(), GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR);
        m_DetailList.SetSelectedIndex(_jieqiShowMode);
		m_ScrollWin.SetVisible(true);
	}
	Invalidate();
	UpdateWindow();
}

LRESULT Ui_ConfigWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case MZ_WM_MOUSE_NOTIFY:
        {
            int nID = LOWORD(wParam);
            int nNotify = HIWORD(wParam);
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (nID == MZ_IDC_LIST_CONFIG && nNotify == MZ_MN_LBUTTONDOWN) {
                if (!m_DetailList.IsMouseDownAtScrolling() && !m_DetailList.IsMouseMoved()) {
                    int nIndex = m_DetailList.CalcIndexOfPos(x, y);
					if(nIndex != -1){
						if(_viewMode == 1){
							if(nIndex < 2){
								_jieqiShowMode = nIndex;
							}
						}
						_viewMode = 0;
						updateUi();
					}
                }
                return 0;
            }
            if (nID == MZ_IDC_LIST_CONFIG && nNotify == MZ_MN_MOUSEMOVE) {
                m_DetailList.SetSelectedIndex(-1);
                m_DetailList.Invalidate();
                m_DetailList.Update();
                return 0;
            }
        }
    }
    return CMzWndEx::MzDefWndProc(message, wParam, lParam);
}

void Ui_ConfigWnd::OnMzCommand(WPARAM wParam, LPARAM lParam) {
    UINT_PTR id = LOWORD(wParam);
    switch (id) {
		case MZ_IDC_BUTTON_JIEQI_MODE:
			_viewMode = (_viewMode == 1) ? 0 : 1;
			updateUi();
			break;
        case MZ_IDC_TOOLBAR_MAIN:
        {
            int nIndex = lParam;
            if (nIndex == 0) {
                // exit the modal dialog
                EndModal(ID_CANCEL);
                return;
            }
            if (nIndex == 2) {
                // exit the modal dialog
				setJieqiMode(_jieqiShowMode);
                EndModal(ID_OK);
                return;
            }
            break;
		}
    }
}

int Ui_ConfigWnd::getJieqiMode(){
	wchar_t currpath[128];
	wchar_t ini_path[255];
	int mode = 0;
	if(File::GetCurrentPath(currpath)){
		wsprintf(ini_path,L"%s\\m8calendar.ini",currpath);
	}
	if(!IniReadInt(L"Config",L"JieqiOrder",(DWORD*)&mode,ini_path)){
		setJieqiMode(mode);
	}
	return mode;
}

void Ui_ConfigWnd::setJieqiMode(int m){
	wchar_t currpath[128];
	wchar_t ini_path[255];
	int mode = 0;
	if(File::GetCurrentPath(currpath)){
		wsprintf(ini_path,L"%s\\m8calendar.ini",currpath);
	}
	if(!File::FileExists(ini_path)){
		IniCreateFile(ini_path);
	}
	IniWriteInt(L"Config",L"JieqiOrder",m,ini_path);
}