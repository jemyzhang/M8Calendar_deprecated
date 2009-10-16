#pragma once

// include the MZFC library header file
#include <mzfc_inc.h>
#include <list>
#include "lunarsolardatetime.h"
#include "..\MzCommon\UiImage.h"

class UiTitleBar  : public UiStatic{
public:
    void OnPaint(HDC hdcDst, RECT* prcWin, RECT* prcUpdate){
		HBRUSH myBrush = CreateSolidBrush(RGB(153,204,255));
		FillRect(hdcDst,prcWin,myBrush);
        UiStatic::OnPaint(hdcDst,prcWin,prcUpdate);
    }
};

class Ui_TodayWnd : public CMzWndEx 
{
	MZ_DECLARE_DYNAMIC(Ui_TodayWnd);
public:
	Ui_TodayWnd(void);
	~Ui_TodayWnd(void);
public:
    UiToolbar_Text m_Toolbar;
	UiTitleBar m_YearMonth;
	UiStatic m_BigDay;
	UiStatic m_GanZhiYear;	//��֧��
	UiStatic m_LunarMonthDay;	//ũ������
	UiStatic m_GanZhiMonth;	//��֧����
	UiStatic m_WeekDayCN;	//���ڣ��У�
	UiStatic m_WeekDayEN;	//���ڣ�Ӣ��
    UiStatic m_Celi1;    //������
    UiStatic m_Celi2;    //������
    UiStatic m_Jieqi;    //����
    UiStatic m_Yi;    //��
    UiStatic m_Ji;    //��
    UiImage m_Header;
    UiImage m_Spliter;
    UiImage m_Spliter_h;
private:
    void updateUi();
protected:
    // Initialization of the window (dialog)
    virtual BOOL OnInitDialog();

    // override the MZFC command handler
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
private:
	int _year, _month, _day;
};
