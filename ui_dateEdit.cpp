#include "ui_dateEdit.h"
#include "..\MzCommon\MzCommon.h"

#include "lunarsolardatetime.h"

using namespace MzCommon;

MZ_IMPLEMENT_DYNAMIC(Ui_ToDateWnd)

#define MZ_IDC_EDIT_SOLAR 101
#define MZ_IDC_EDIT_LUNAR 102
#define MZ_IDC_BTN_TO 103
#define MZ_IDC_BTN_CANCEL 104


void UiDateEdit::init(){
	int baseid = GetID();
	int y = 0;
	m_EdtCalendarYear.SetPos(0, y, GetWidth()/3, MZM_HEIGHT_SINGLELINE_EDIT);
	m_EdtCalendarYear.SetMaxChars(4);
//	m_EdtCalendarYear.SetSipMode(IM_SIP_MODE_DIGIT);
	m_EdtCalendarYear.SetTip2(L"��",true);
	m_EdtCalendarYear.SetRightInvalid(40);
	m_EdtCalendarYear.SetID(baseid);
	m_EdtCalendarYear.EnableNotifyMessage(true);
	AddChild(&m_EdtCalendarYear);

	m_EdtCalendarMonth.SetPos(GetWidth()/3, y, GetWidth()/3, MZM_HEIGHT_SINGLELINE_EDIT);
	m_EdtCalendarMonth.SetMaxChars(2);
//	m_EdtCalendarMonth.SetSipMode(IM_SIP_MODE_DIGIT);
	m_EdtCalendarMonth.SetTip2(L"��",true);
	m_EdtCalendarMonth.SetRightInvalid(40);
	m_EdtCalendarMonth.SetID(baseid);
	m_EdtCalendarMonth.EnableNotifyMessage(true);
	AddChild(&m_EdtCalendarMonth);

	m_EdtCalendarDay.SetPos(GetWidth()*2/3, y, GetWidth()/3, MZM_HEIGHT_SINGLELINE_EDIT);
	m_EdtCalendarDay.SetMaxChars(2);
//	m_EdtCalendarDay.SetSipMode(IM_SIP_MODE_DIGIT);
	m_EdtCalendarDay.SetTip2(L"��",true);
	m_EdtCalendarDay.SetRightInvalid(40);
	m_EdtCalendarDay.SetID(baseid);
	m_EdtCalendarDay.EnableNotifyMessage(true);
	AddChild(&m_EdtCalendarDay);
}

bool UiDateEdit::checkDateText(){
	int year,month,day;
	year = _wtoi(m_EdtCalendarYear.GetText().C_Str());
	month = _wtoi(m_EdtCalendarMonth.GetText().C_Str());
	day = _wtoi(m_EdtCalendarDay.GetText().C_Str());
	int rc = DateTime::checkDate(year,month,day);
	if(rc&1) {
		m_EdtCalendarYear.SetTextColor(RGB(255,0,0));
	}else{
		m_EdtCalendarYear.SetTextColor(RGB(0,0,0));
	}
	if(rc&2) {
		m_EdtCalendarMonth.SetTextColor(RGB(255,0,0));
	}else{
		m_EdtCalendarMonth.SetTextColor(RGB(0,0,0));
	}
	if(rc&4) {
		m_EdtCalendarDay.SetTextColor(RGB(255,0,0));
	}else{
		m_EdtCalendarDay.SetTextColor(RGB(0,0,0));
	}

	m_EdtCalendarYear.Invalidate();
	m_EdtCalendarYear.Update();
	m_EdtCalendarMonth.Invalidate();
	m_EdtCalendarMonth.Update();
	m_EdtCalendarDay.Invalidate();
	m_EdtCalendarDay.Update();
	return (rc != 0);
}

Ui_ToDateWnd::Ui_ToDateWnd(void)
{
}

Ui_ToDateWnd::~Ui_ToDateWnd(void)
{
}

void Ui_ToDateWnd::getDate(DWORD &year, DWORD &month, DWORD &day){
	m_EdtSolarDate.getDate(year,month,day);
}

BOOL Ui_ToDateWnd::OnInitDialog() {
    // Must all the Init of parent class first!
    if (!CMzWndEx::OnInitDialog()) {
        return FALSE;
    }

    // Then init the controls & other things in the window
	m_bg.SetPos(0,0,GetWidth(),GetHeight());
	AddUiWin(&m_bg);

    int y = 20;
	m_LblSolarDate.SetPos(0,y,GetWidth()/4,MZM_HEIGHT_SINGLELINE_EDIT);
	m_LblSolarDate.SetText(L"����");
	m_LblSolarDate.SetTextColor(RGB(255,255,255));
	m_bg.AddChild(&m_LblSolarDate);

	m_EdtSolarDate.SetPos(GetWidth()/4,y,GetWidth()*3/4 - 20,MZM_HEIGHT_SINGLELINE_EDIT);
	m_EdtSolarDate.SetID(MZ_IDC_EDIT_SOLAR);
	m_EdtSolarDate.init();
	m_EdtSolarDate.Update();
	m_EdtSolarDate.EnableNotifyMessage(true);
	m_bg.AddChild(&m_EdtSolarDate);

	y += MZM_HEIGHT_SINGLELINE_EDIT;
	m_LblLunarDate.SetPos(0,y,GetWidth()/4,MZM_HEIGHT_SINGLELINE_EDIT);
	m_LblLunarDate.SetText(L"ũ��");
	m_LblLunarDate.SetTextColor(RGB(255,255,255));
    m_bg.AddChild(&m_LblLunarDate);

	m_EdtLunarDate.SetPos(GetWidth()/4,y,GetWidth()*3/4 - 20,MZM_HEIGHT_SINGLELINE_EDIT);
	m_EdtLunarDate.SetID(MZ_IDC_EDIT_LUNAR);
	m_EdtLunarDate.init();
	m_EdtLunarDate.Update();
	m_EdtLunarDate.EnableNotifyMessage(true);
	m_bg.AddChild(&m_EdtLunarDate);

	y += MZM_HEIGHT_SINGLELINE_EDIT;
	m_BtnDateTo.SetPos(0,y,GetWidth()/2,MZM_HEIGHT_BUTTONEX + 20);
	m_BtnDateTo.SetText(L"ת��");
	m_BtnDateTo.SetID(MZ_IDC_BTN_TO);
	m_bg.AddChild(&m_BtnDateTo);

	m_BtnCancel.SetPos(GetWidth()/2,y,GetWidth()/2,MZM_HEIGHT_BUTTONEX + 20);
	m_BtnCancel.SetText(L"ȡ��");
	m_BtnCancel.SetID(MZ_IDC_BTN_CANCEL);
	m_bg.AddChild(&m_BtnCancel);

    return TRUE;
}

void Ui_ToDateWnd::OnMzCommand(WPARAM wParam, LPARAM lParam) {
    UINT_PTR id = LOWORD(wParam);
    switch (id) {
		case MZ_IDC_BTN_TO:
			if(!m_EdtSolarDate.checkDateText() &&
				!m_EdtLunarDate.checkDateText()){
				EndModal(ID_OK);
			}
			break;
		case MZ_IDC_BTN_CANCEL:
			EndModal(ID_CANCEL);
			break;
	}
	return;
}

LRESULT Ui_ToDateWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
		case MZ_MW_TEXT_EDITED:
		{
			int nID = LOWORD(wParam);
			if(nID == MZ_IDC_EDIT_SOLAR){
				DWORD y,m,d;
				m_EdtSolarDate.getDate(y,m,d);
				LunarSolarDateTime s(y,m,d);
				s.SolarToLunar();
				LSDate l = s.getLunarDate();
				m_EdtLunarDate.setDate(l.year,l.month,l.day);
				m_EdtLunarDate.Invalidate();
				m_EdtLunarDate.Update();
				return 0;
			}else if(nID == MZ_IDC_EDIT_LUNAR){
				DWORD y,m,d;
				m_EdtLunarDate.getDate(y,m,d);
				LunarSolarDateTime s(y,m,d);
				s.setLunarDate(y,m,d);
				LSDate l = s.getSolarDate();
				m_EdtSolarDate.setDate(l.year,l.month,l.day);
				m_EdtSolarDate.Invalidate();
				m_EdtSolarDate.Update();
				return 0;
			}
			return 0;
		}
		case MZ_WM_UI_FOCUS:
		{
            int nID = LOWORD(wParam);
            if (nID == MZ_IDC_EDIT_SOLAR || nID == MZ_IDC_EDIT_LUNAR) {
//				MzOpenSip(IM_SIP_MODE_DIGIT);
                return 0;
            }
		}
    }
    return CMzWndEx::MzDefWndProc(message, wParam, lParam);
}

////
MZ_IMPLEMENT_DYNAMIC(Ui_LenDateWnd)

Ui_LenDateWnd::Ui_LenDateWnd(void)
{
}

Ui_LenDateWnd::~Ui_LenDateWnd(void)
{
}

void Ui_LenDateWnd::getDateFrom(DWORD &year, DWORD &month, DWORD &day){
	m_EdtFromDate.getDate(year,month,day);
}
void Ui_LenDateWnd::getDateTo(DWORD &year, DWORD &month, DWORD &day){
	m_EdtToDate.getDate(year,month,day);
}

BOOL Ui_LenDateWnd::OnInitDialog() {
    // Must all the Init of parent class first!
    if (!CMzWndEx::OnInitDialog()) {
        return FALSE;
    }

    // Then init the controls & other things in the window
	m_bg.SetPos(0,0,GetWidth(),GetHeight());
	AddUiWin(&m_bg);

    int y = 20;
	m_LblFromDate.SetPos(0,y,GetWidth()/4,MZM_HEIGHT_SINGLELINE_EDIT);
	m_LblFromDate.SetText(L"�ӣ�");
	m_LblFromDate.SetTextColor(RGB(255,255,255));
	m_bg.AddChild(&m_LblFromDate);

	m_EdtFromDate.SetPos(GetWidth()/4,y,GetWidth()*3/4 - 20,MZM_HEIGHT_SINGLELINE_EDIT);
	m_EdtFromDate.init();
	m_EdtFromDate.Update();
	m_bg.AddChild(&m_EdtFromDate);

	y += MZM_HEIGHT_SINGLELINE_EDIT;
	m_LblToDate.SetPos(0,y,GetWidth()/4,MZM_HEIGHT_SINGLELINE_EDIT);
	m_LblToDate.SetText(L"����");
	m_LblToDate.SetTextColor(RGB(255,255,255));
    m_bg.AddChild(&m_LblToDate);

	m_EdtToDate.SetPos(GetWidth()/4,y,GetWidth()*3/4 - 20,MZM_HEIGHT_SINGLELINE_EDIT);
	m_EdtToDate.init();
	m_EdtToDate.Update();
	m_bg.AddChild(&m_EdtToDate);

	y += MZM_HEIGHT_SINGLELINE_EDIT;
	m_BtnDateLen.SetPos(0,y,GetWidth()/2,MZM_HEIGHT_BUTTONEX + 20);
	m_BtnDateLen.SetText(L"����");
	m_BtnDateLen.SetID(MZ_IDC_BTN_TO);
	m_bg.AddChild(&m_BtnDateLen);

	m_BtnCancel.SetPos(GetWidth()/2,y,GetWidth()/2,MZM_HEIGHT_BUTTONEX + 20);
	m_BtnCancel.SetText(L"ȡ��");
	m_BtnCancel.SetID(MZ_IDC_BTN_CANCEL);
	m_bg.AddChild(&m_BtnCancel);

    return TRUE;
}

void Ui_LenDateWnd::OnMzCommand(WPARAM wParam, LPARAM lParam) {
    UINT_PTR id = LOWORD(wParam);
    switch (id) {
		case MZ_IDC_BTN_TO:
			if(!m_EdtFromDate.checkDateText() &&
				!m_EdtToDate.checkDateText()){
				EndModal(ID_OK);
			}
			break;
		case MZ_IDC_BTN_CANCEL:
			EndModal(ID_CANCEL);
			break;
	}
	return;
}

LRESULT Ui_LenDateWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
		case MZ_WM_UI_FOCUS:
		{
            int nID = LOWORD(wParam);
		}
    }
    return CMzWndEx::MzDefWndProc(message, wParam, lParam);
}
