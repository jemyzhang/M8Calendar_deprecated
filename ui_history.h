#pragma once

// include the MZFC library header file
#include <mzfc_inc.h>
#include "calendardb.h"

class UiHistoryList : public UiList{
public:
	UiHistoryList() { _db = 0; _btn = 0;}
public:
	void DrawItem(HDC hdcDst, int nIndex, RECT* prcItem, RECT *prcWin, RECT *prcUpdate);
	void setupdb(clsCalendarDB *db) { _db = db; }
	void setupButton(UiButton *btn) { _btn = btn; }
private:
	clsCalendarDB *_db;
	UiButton* _btn;
};

// Popup window derived from CMzWndEx

class UI_HistoryWnd : public CMzWndEx {
    MZ_DECLARE_DYNAMIC(UI_HistoryWnd);
public:
	UI_HistoryWnd();
	void setupdate(DWORD month = -1,DWORD day = -1);
    void OnTimer(UINT nIDEvent);
protected:
    void GetHistoryList();
	void setupDetailView(int);
protected:
    UiToolbar_Text m_Toolbar;
    UiCaption m_CaptionTitle;
	UiHistoryList m_ListHistory;
	UiButton m_BtnDetail;
//detail view
	//UiScrollWin m_Scrollwin;
	UiMultiLineEdit m_EdtDetail;

    // Initialization of the window (dialog)
    virtual BOOL OnInitDialog();

    // override the MZFC command handler
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	wchar_t db_path[256];
	clsCalendarDB calendar_db;
	bool _isConnected;
	DWORD _month,_day;
	bool _detailViewMode;
private:
    UINT identHistodayLine(LPTSTR linetext);    //分析行文字的类别 0: 无效 1: 标题 2: 内容
    //bEnd: 最后一条，防止遗漏
    bool getEntryData(LPTSTR linetext,bool bEnd = false); //如果写入获取到完整一条则返回1，否则0
	bool ImportData(TCHAR* filename);
};
