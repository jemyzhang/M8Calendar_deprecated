#pragma once

// include the MZFC library header file
#include <mzfc_inc.h>
#include "mz_commonfunc.h"
#include <list>
#include "lunarsolardatetime.h"

#define GRID_USE_UILIST 0
#if GRID_USE_UILIST
#pragma message("Grid:ʹ��UiList")
#else
#pragma message("Grid:ֱ�ӻ�ͼ")
#endif

class UiYiJiLabel : public UiWin
{
public:
	UiYiJiLabel(){
		_isTs = false;
	}
	~UiYiJiLabel(){
	}
	virtual void PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate){
		UiWin::PaintWin(hdcDst,prcWin,prcUpdate);
		SetBkMode(hdcDst,TRANSPARENT);

		MzDrawGridDlgBG(hdcDst,prcWin);

		HFONT hf = FontHelper::GetFont( 30 );
		SelectObject( hdcDst , hf );

		RECT rect;
		int height = prcWin->bottom - prcWin->top - 20;
		int width = prcWin->right - prcWin->left;
		rect.left = prcWin->left + 20;
		rect.right = rect.left + 50;
		//��
		rect.top = prcWin->top;
		rect.bottom = rect.top + height / 2;
		::SetTextColor( hdcDst,RGB(64,255,128));
		MzDrawText( hdcDst, L"��", &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
		//��
		rect.top = prcWin->top + height / 2;
		rect.bottom = prcWin->bottom - 10;
		::SetTextColor( hdcDst, RGB(255,64,64));
		MzDrawText( hdcDst, L"��", &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
		DeleteObject( hf );

		hf = FontHelper::GetFont( 20 );
		SelectObject( hdcDst , hf );

		if(_isTs){
			rect.top = prcWin->top;
			rect.bottom = prcWin->bottom - 10;
			rect.left = prcWin->left + 60;
			rect.right = prcWin->right;
			::SetTextColor( hdcDst, RGB(255,64,255));
			MzDrawText( hdcDst, tsText.C_Str(), &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
		}else{
			::SetTextColor( hdcDst, RGB(255,255,255));

			rect.top = prcWin->top;
			rect.bottom = rect.top + height / 2;
			rect.left = prcWin->left + 60;
			rect.right = prcWin->right;
			MzDrawText( hdcDst, yiText.C_Str(), &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );

			rect.top = prcWin->top + height / 2;
			rect.bottom = prcWin->bottom - 10;
			MzDrawText( hdcDst, jiText.C_Str(), &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
		}
		DeleteObject( hf );
	}
	void setText(wchar_t* yi, wchar_t* ji,bool ts = false){
		_isTs = ts;
		if(!ts){
			yiText = yi;
			jiText = ji;
		}else{
			tsText = yi;
		}
	}
private:
	CMzString yiText,jiText,tsText;
	bool _isTs;	//����
};

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
		CMzString text;	//����
		CMzString text1;	//��һ��
		CMzString text2;	//�¶���
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
	HDC pMemDC;             //�����ڴ�DCָ��
	HBITMAP pBitmap;        //�����ڴ�λͼָ��
private:
	bool checkRange(int row, int col);
};

#define CALENDAR_USE_GRID 1
#if CALENDAR_USE_GRID
#pragma message("����:ʹ����������")
#else
#pragma message("����:ֱ������")
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
//	UiButton m_BtnNext;
//	UiButton m_BtnPre;
	UiStatic m_YearMonth;
	UiStatic m_WeekBar;
	UiGrid m_Calendar;
	UiStatic m_LunarMD;	//ũ������
	UiStatic m_GanZhiYMD;	//��֧������
	UiImage m_ZodiacImage;
	UiYiJiLabel m_Tipyiji;
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
	void updateInfo(bool forceupdate = false);
	void showTip(bool bshow = false);
private:
	int _year, _month, _day;
	bool _showMonthByJieqi;
	bool _isMouseMoving;
	short _MouseX;
	short _MouseY;
	int sel_row,sel_col;
};
