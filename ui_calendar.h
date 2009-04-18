#pragma once

// include the MZFC library header file
#include <mzfc_inc.h>
#include "mz_commonfunc.h"
#include <list>
#include "lunarsolardatetime.h"

#define GRID_USE_UILIST 0
#if GRID_USE_UILIST
#pragma message("Grid:使用UiList")
#else
#pragma message("Grid:直接绘图")
#endif
class UiImage : public UiWin
{
public:
	UiImage(void){
		_reqUpdate = true;
		pimg = 0;
	}
	~UiImage(void){
	}
	void setupImage(ImagingHelper* img) {
		pimg = img;
	}
	virtual void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate);
	virtual void Update() {
		_reqUpdate = true;
		UiWin::Update();
	}
private:
	bool _reqUpdate;
	ImagingHelper *pimg;
};
class UiGrid : public UiWin
{
public:
	UiGrid(void);
	~UiGrid(void);
	virtual void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate);
	virtual void OnPaint(HDC hdcDst, RECT* prcWin, RECT* prcUpdate){
		UiWin::OnPaint(hdcDst,prcWin,prcUpdate);
	}
public:
	//if ret = false, no selection
	bool calcSelectedIndex(int x, int y,int &row, int &col);
	void setSelectedIndex(int row,int col);
	//if ret = false, no selection
	bool getSelectedIndex(int &row, int &col);
public:
	//if idx == -1, set all
	void setTextSize(int row,int col, int nSize);
	//if idx == -1 set all
	void setTextColor(int row,int col, COLORREF c);
	void setText1Color(int row,int col, COLORREF c);
	void setText(int row,int col, LPCTSTR text);
	void setText1(int row,int col, LPCTSTR text);
	int getTextSize(int row,int col);
	COLORREF getTextColor(int row,int col);
	LPCTSTR getText(int row,int col);
	void setSelectedBgColor(COLORREF c);
	void setSelectedTextColor(COLORREF c);
	void setGridAutoSize(bool);
	void setGridSize(int width, int height);
public:
	void setGrids(int nRow,int nCol);
	int getRowCount(void);
	int getColCount(void);
public:
	virtual void Update(){
		_reqUpdate = true;
		UiWin::Update();
	}
	virtual void SetPos(int x, int y, int w, int h, UINT flags=0);
private:
	typedef struct GridAttr{
		CMzString text;	//正中
		CMzString text1;	//下一行
		CMzString text2;	//下二行
		int textSize;
		int text1Size;
		COLORREF textColor;
		COLORREF text1Color;
		bool isSelected;
	}GridAttr_t,*GridAttr_ptr;
	GridAttr_ptr *_grids;
#if GRID_USE_UILIST
	UiButton *_colList;
#endif
private:
	COLORREF _selbg, _seltxt;
	int _rows,_cols;
	bool _isAutosize;
	int _gwidth, _gheight;
	bool _reqUpdate;
	int m_nMaxX;
	int m_nMaxY;
private:
	HDC pMemDC;             //定义内存DC指针
	HBITMAP pBitmap;        //定义内存位图指针
private:
	bool checkRange(int row, int col);
};

#define CALENDAR_USE_GRID 1
#if CALENDAR_USE_GRID
#pragma message("日历:使用日历方格")
#else
#pragma message("日历:直接输入")
#endif

class Ui_CalendarWnd : public CMzWndEx 
{
	MZ_DECLARE_DYNAMIC(Ui_CalendarWnd);
public:
	Ui_CalendarWnd(void);
	~Ui_CalendarWnd(void);
public:
    UiToolbar_Text m_Toolbar;
	UiCaption m_CaptionHeader;
	UiButton m_BtnNext;
	UiButton m_BtnPre;
	UiStatic m_YearMonth;
	UiStatic m_WeekBar;
	UiGrid m_Calendar;
	UiStatic m_LunarMD;	//农历月日
	UiStatic m_GanZhiYMD;	//干支年月日
	UiImage m_ZodiacImage;
public:
	CMzString getDate();
protected:
    // Initialization of the window (dialog)
    virtual BOOL OnInitDialog();

    // override the MZFC command handler
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
    // override the MZFC window messages handler
    virtual LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	void updateGrid();
	void updateInfo();
private:
	int _year, _month, _day;
};
