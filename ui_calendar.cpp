#include "ui_calendar.h"
#include "mz_commonfunc.h"
using namespace MZ_CommonFunc;

#include "ui_about.h"
#include "ui_dateEdit.h"
#include "ui_history.h"
#include "resource.h"
#include "ui_config.h"

MZ_IMPLEMENT_DYNAMIC(Ui_CalendarWnd)

#define MZ_IDC_TOOLBAR_CALENDAR 101
#define MZ_IDC_Edit_YEAR 102
#define MZ_IDC_Edit_MONTH 103
#define MZ_IDC_Edit_DAY 104
#define MZ_IDC_CALENDAR_GRID 105
//#define MZ_IDC_CALENDAR_NEXT 106
//#define MZ_IDC_CALENDAR_PRE 107
#define MZ_IDC_YIJI_TIP 108

#define IDC_PPM_ABOUT 101
#define IDC_PPM_TODATE 102
#define IDC_PPM_DATES 103
#define IDC_PPM_HISTORY 104
#define IDC_PPM_RETURN 105

void UiImage::PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate){
	UiWin::PaintWin(hdcDst,prcWin,prcUpdate);
//	if(_reqUpdate){
//		_reqUpdate = false;
		if(pimg){
			pimg->Draw(hdcDst,prcWin,true,true);
		}
//	}
}

UiGrid::UiGrid()
	:UiWin()
{
	_grids = 0;
	_selbg = RGB(64,192,192);
	_seltxt = RGB(255,255,255);
	_rows = 1;
	_cols = 1;
	setGridSize(_rows,_cols);
	_gwidth = 68;
	_gheight = 68;
	_reqUpdate = false;
	_isAutosize = false;
#if GRID_USE_UILIST
	_colList = 0;
#endif
}

UiGrid::~UiGrid(){
	setGridSize(0,0);	//release 
	if(pMemDC) ReleaseDC(GetParentWnd(),pMemDC);
	if(pBitmap) DeleteObject(pBitmap);
}

void UiGrid::setGridAutoSize(bool a){
	_isAutosize = a;
}

void UiGrid::setSelectedIndex(int row,int col){
	if(checkRange(row,col)){
		int oldr, oldc;
		if(getSelectedIndex(oldr,oldc)){
			_grids[oldr][oldc].isSelected = false;
		}
		_grids[row][col].isSelected = true;
	}
}

bool UiGrid::calcSelectedIndex(int x, int y, int &row, int &col){
	bool ret = true;
	int c = x / _gwidth;
	int r = y / _gheight;
	if(c > _cols || r > _rows){
		row = -1;
		col = -1;
		ret = false;
	}else{
		row = r;
		col = c;
	}
	return ret;
}

bool UiGrid::getSelectedIndex(int &row, int &col){
	bool ret = true;
		for(int i = 0; i < _rows; i++){
			for(int j = 0; j < _cols; j++){
				if(_grids[i][j].isSelected){
					row = i;
					col = j;
					return ret;
				}
			}
		}
	row = -1;
	col = -1;
	return false;
}

void UiGrid::setGridSize(int width, int height){
	_gwidth = width;
	_gheight = height;
}

void UiGrid::setGrids(int nRow, int nCol){
	if(_grids){
		for(int i = 0; i < _rows; i++){
			for(int j = 0; j < _cols; j++){
				_grids[i][j].text.SetBufferSize(0);
			}
			delete _grids[i];
		}
		delete _grids;
		_grids = 0;
	}
#if GRID_USE_UILIST
	if(_colList){
		delete[] _colList;
		_colList = 0;
	}
#endif
	_rows = nRow;
	_cols = nCol;
	if(_rows >= 0 && _cols >= 0){
		_grids = new GridAttr_ptr[_rows];
		for(int i = 0; i < _rows; i++){
			_grids[i] = new GridAttr_t[_cols];
			for(int j = 0; j < _cols; j++){
				_grids[i][j].isSelected = false;
				_grids[i][j].text = 0;
				_grids[i][j].text1 = 0;
				_grids[i][j].textColor = RGB(0,0,0);
				_grids[i][j].text1Color = RGB(128,128,128);
				_grids[i][j].textSize = 30;
				_grids[i][j].text1Size = 17;
			}
		}
	#if GRID_USE_UILIST
		_colList = new UiButton[_cols*_rows];
	#endif
	}


}

int UiGrid::getColCount(){
	return _cols;
}

int UiGrid::getRowCount(){
	return _rows;
}

void UiGrid::setText(int row,int col, LPCTSTR text){
	if(checkRange(row,col)){
		_grids[row][col].text = text;
	}
}

void UiGrid::setText1(int row,int col, LPCTSTR text){
	if(checkRange(row,col)){
		_grids[row][col].text1 = text;
	}
}

void UiGrid::setTextColor(int row,int col, COLORREF c){
	if(checkRange(row,col)){
		_grids[row][col].textColor = c;
	}
}

void UiGrid::setText1Color(int row,int col, COLORREF c){
	if(checkRange(row,col)){
		_grids[row][col].text1Color = c;
	}
}

void UiGrid::setTextSize(int row,int col, int nSize){
	if(checkRange(row,col)){
		_grids[row][col].textSize = nSize;
	}
}

LPCTSTR UiGrid::getText(int row,int col){
	if(checkRange(row,col)){
		return _grids[row][col].text;
	}
	return 0;
}

COLORREF UiGrid::getTextColor(int row,int col){
	if(checkRange(row,col)){
		return _grids[row][col].textColor;
	}
	return RGB(0,0,0);
}

int UiGrid::getTextSize(int row,int col){
	if(checkRange(row,col)){
		return _grids[row][col].textSize;
	}
	return 0;
}

bool UiGrid::checkRange(int row, int col){
	if(row >= _rows || col >= _cols ||
		row < 0 || col < 0){
			return false;
	}
	return true;
}

void UiGrid::SetPos(int x, int y, int w, int h, UINT flags){
	UiWin::SetPos(x,y,w,h,flags);
	m_nMaxX = w;
	m_nMaxY = h;
#if GRID_USE_UILIST
#else
	pMemDC = CreateCompatibleDC(GetDC(GetParentWnd()));
	pBitmap = CreateCompatibleBitmap(GetDC(GetParentWnd()),m_nMaxX,m_nMaxY);
#endif
}

void UiGrid::PaintWin(HDC hdcDst, RECT* prcWin, RECT* prcUpdate){
	UiWin::PaintWin(hdcDst,prcWin,prcUpdate);
	if(_reqUpdate){
		_reqUpdate = false;
		SelectObject(pMemDC, pBitmap);
		HBRUSH myBrush = CreateSolidBrush(RGB(255-16,255-16,255-16));
		RECT rect;
		rect.top = 0;
		rect.bottom = m_nMaxY;
		rect.left = 0;
		rect.right = m_nMaxX;

		FillRect(pMemDC,&rect,myBrush);//画之前先擦除.
		//SetTextColor(RGB(0,0,0));

		//无数据
		if(_rows <= 0 || _cols <= 0){
			BitBlt(hdcDst,0,0,m_nMaxX,m_nMaxY,pMemDC,0,0,SRCCOPY);
			return;
		}
		int _gridw = _gwidth;
		int _gridh = _gheight;
		if(_isAutosize){
			int _width = prcWin->right - prcWin->left;
			int _height = prcWin->bottom - prcWin->top;
			_gridw = _width/_cols;
			_gridh = _height/_rows;
		}
		int _x = 0;//prcWin->left;
		int _y = 0;//prcWin->top;

		HPEN pen = CreatePen(PS_SOLID, 0,RGB(128,128,128));
		HPEN poldpen = (HPEN)SelectObject(pMemDC,pen);
		SetBkMode(pMemDC,TRANSPARENT);
		for(int i = 0; i < _rows; i++){
			int cx = _x;
			int cy = _y;
			for(int j = 0; j < _cols; j++){
				//格子
				CRect rect(cx + (_gridw-1)*j,cy + (_gridh-1)*i,cx + _gridw*j + _gridw,cy + _gridh*i + _gridh);
				Rectangle(pMemDC,rect.left,rect.top,rect.right,rect.bottom);
				//格子背景
				CRect frect(rect.left+1,rect.top + 1,rect.right - 2,rect.bottom - 2);
				CRect textrect(rect.left+1,rect.top + 1,rect.right - 2,rect.bottom - 20);
				HBRUSH bqbrush;
				if(_grids[i][j].isSelected){	//selected
					::SetTextColor(pMemDC,_seltxt);
					MzDrawSelectedBg_NoLine(pMemDC,&frect);
				}else{
					bqbrush = CreateSolidBrush(RGB(255-16,255-16,255-16));
					::SetTextColor(pMemDC,_grids[i][j].textColor);
					FillRect(pMemDC,&frect,bqbrush);
				}
				HFONT font = FontHelper::GetFont(_grids[i][j].textSize);
				SelectObject(pMemDC,font);
				MzDrawText( pMemDC,_grids[i][j].text.C_Str(), &textrect, DT_CENTER|DT_VCENTER );
				DeleteObject(font);

				font = FontHelper::GetFont(_grids[i][j].text1Size);
				SelectObject(pMemDC,font);
				::SetTextColor(pMemDC,_grids[i][j].text1Color);
				CRect text1rect(rect.left+1,rect.top + 35,rect.right - 2,rect.bottom - 2);
				MzDrawText( pMemDC,_grids[i][j].text1.C_Str(), &text1rect, DT_CENTER|DT_VCENTER );
				DeleteObject(font);
			}
		}
		SelectObject(pMemDC,poldpen);
	}
	BitBlt(hdcDst,prcWin->left,prcWin->top,m_nMaxX,m_nMaxY,pMemDC,0,0,SRCCOPY);
}
//////

Ui_CalendarWnd::Ui_CalendarWnd(void)
{
	_showMonthByJieqi = Ui_ConfigWnd::getJieqiMode();
	_isMouseMoving = false;
}

Ui_CalendarWnd::~Ui_CalendarWnd(void)
{
}
CMzString Ui_CalendarWnd::getDate(){
	CMzString sDate;
	wsprintf(sDate.C_Str(), L"%4d-%02d-%02d", _year, _month, _day);
	return sDate;
}

BOOL Ui_CalendarWnd::OnInitDialog() {
    // Must all the Init of parent class first!
    if (!CMzWndEx::OnInitDialog()) {
        return FALSE;
    }

    // Then init the controls & other things in the window
    int y = 0;
	m_CaptionHeader.SetPos(0,y,GetWidth(),128 + MZM_HEIGHT_CAPTION/2);
	m_CaptionHeader.EnableNotifyMessage(true);
	AddUiWin(&m_CaptionHeader);

	m_ZodiacImage.SetPos(5,0,128,128);
	m_CaptionHeader.AddChild(&m_ZodiacImage);

	m_YearMonth.SetPos(128 + 40,10,(GetWidth() - 128)*3/4,MZM_HEIGHT_CAPTION);
	m_YearMonth.SetTextSize(32);
	m_YearMonth.EnableNotifyMessage(true);
	m_CaptionHeader.AddChild(&m_YearMonth);

	m_GanZhiYMD.SetPos(128 + 40,10 + MZM_HEIGHT_CAPTION,(GetWidth() - 128)*3/4,MZM_HEIGHT_CAPTION);
	m_GanZhiYMD.SetTextColor(RGB(128,128,128));
	m_GanZhiYMD.SetTextSize(23);
	m_GanZhiYMD.EnableNotifyMessage(true);
	m_CaptionHeader.AddChild(&m_GanZhiYMD);

	m_LunarMD.SetPos(128 + 40,10 + MZM_HEIGHT_CAPTION + 30,(GetWidth() - 128)*3/4,MZM_HEIGHT_CAPTION);
	m_LunarMD.SetTextColor(RGB(128,128,128));
	m_LunarMD.SetTextSize(23);
	m_LunarMD.EnableNotifyMessage(true);
	m_CaptionHeader.AddChild(&m_LunarMD);

	m_WeekBar.SetPos(0,128,GetWidth(),MZM_HEIGHT_CAPTION/2);
	m_WeekBar.SetText(L"星期日   星期一   星期二   星期三    星期四   星期五   星期六");
	m_WeekBar.SetTextSize(17);
	m_WeekBar.SetTextColor(RGB(128,128,128));
	m_WeekBar.EnableNotifyMessage(true);
	m_CaptionHeader.AddChild(&m_WeekBar);

	y+= 128 + MZM_HEIGHT_CAPTION / 2;
    m_Calendar.SetPos(2, y, GetWidth()-2, 69*6);
	m_Calendar.SetID(MZ_IDC_CALENDAR_GRID);
	m_Calendar.EnableNotifyMessage(true);
    AddUiWin(&m_Calendar);

    m_Toolbar.SetPos(0, GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);
    m_Toolbar.SetButton(0, true, true, L"今日");
    m_Toolbar.SetButton(1, true, true, L"更多");
    m_Toolbar.SetButton(2, true, true, L"设置");
    m_Toolbar.SetID(MZ_IDC_TOOLBAR_CALENDAR);
    AddUiWin(&m_Toolbar);

	m_Tipyiji.SetID(MZ_IDC_YIJI_TIP);
	m_Tipyiji.EnableNotifyMessage(true);
	m_Tipyiji.SetVisible(false);
	AddUiWin(&m_Tipyiji);

	DateTime::getDate(&_year,&_month,&_day);
	updateGrid();
	updateInfo();
    return TRUE;
}

void Ui_CalendarWnd::updateGrid(){
	int week = (DateTime::getWeekDay(_year,_month,1)+1)%7;	//获取1号的星期
	int days = DateTime::getDays(_year,_month);
	int rows = (week+days)/7 + ((week+days)%7 ? 1 : 0);
	m_Calendar.setGrids(rows,7);
	int t_year,t_month,t_day;
	DateTime::getDate(&t_year,&t_month,&t_day);
	wchar_t datestr[8];
	LunarSolarDateTime _lstm(_year,_month);

	LS24TERM_ptr p24term = _lstm.SolarTerm();
			//update lunar info
			//update 24 jieqi

	for(int i = 0; i < days; i++){
		int r = (week + i) / 7;
		int c = (week + i) % 7;
		wsprintf(datestr,L"%d",i+1);
		m_Calendar.setText(r,c,datestr);
		_lstm.setDay(i + 1);
		m_Calendar.setText1(r,c,_lstm.LunarDay().C_Str());
		if(t_year == _year && t_month == _month && t_day == i+1){
			m_Calendar.setTextColor(r,c,RGB(192,64,64));
		}
		if(_day == i+1){
			m_Calendar.setSelectedIndex(r,c);
			sel_row = r;
			sel_col = c;
		}
		for(int j = 0; j < 2; j++){
			if(i == (p24term[j].day - 1)){
				m_Calendar.setText1(r,c,p24term[j].name);
				m_Calendar.setText1Color(r,c,RGB(200,64,128));
				break;
			}
		}
		wchar_t* sh = _lstm.SolarHoliday();
		if(sh){
			m_Calendar.setText1(r,c,sh);
			m_Calendar.setText1Color(r,c,RGB(64,64,255));
		}
		wchar_t* lh = _lstm.LunarHoliday();
		if(lh){
			m_Calendar.setText1(r,c,lh);
			m_Calendar.setText1Color(r,c,RGB(255,0,0));
		}
	}
	m_Calendar.Invalidate();
	m_Calendar.Update();
}

void Ui_CalendarWnd::OnMzCommand(WPARAM wParam, LPARAM lParam) {
    UINT_PTR id = LOWORD(wParam);
    switch (id) {
        case MZ_IDC_TOOLBAR_CALENDAR:
        {
            int nIndex = lParam;
			if(nIndex == 0){	//今日
				DateTime::getDate(&_year,&_month,&_day);
				updateGrid();
				updateInfo();
				showTip();
				return;
			}
			if(nIndex == 1){	//更多
				CPopupMenu ppm;
				struct PopupMenuItemProp pmip;      

				pmip.itemCr = MZC_BUTTON_PELLUCID;
				pmip.itemRetID = IDC_PPM_RETURN;
				pmip.str = L"取消";
				ppm.AddItem(pmip);

				pmip.itemCr = MZC_BUTTON_PELLUCID;
				pmip.itemRetID = IDC_PPM_ABOUT;
				pmip.str = L"关于";
				ppm.AddItem(pmip);

				pmip.itemCr = MZC_BUTTON_PELLUCID;
				pmip.itemRetID = IDC_PPM_HISTORY;
				pmip.str = L"历史上的今天";
				ppm.AddItem(pmip);

				pmip.itemCr = MZC_BUTTON_PELLUCID;
				pmip.itemRetID = IDC_PPM_DATES;
				pmip.str = L"相隔日期计算";
				ppm.AddItem(pmip);

				pmip.itemCr = MZC_BUTTON_PELLUCID;
				pmip.itemRetID = IDC_PPM_TODATE;
				pmip.str = L"转到日期";
				ppm.AddItem(pmip);

				RECT rc = MzGetWorkArea();      
				rc.top = rc.bottom - ppm.GetHeight();
				ppm.Create(rc.left,rc.top,RECT_WIDTH(rc),RECT_HEIGHT(rc),m_hWnd,0,WS_POPUP);      
				int nID = ppm.DoModal();
				switch(nID){
				case IDC_PPM_ABOUT:
				{
					UI_AboutWnd dlg;
					RECT rcWork = MzGetWorkArea();
					dlg.Create(rcWork.left, rcWork.top, RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork),
							m_hWnd, 0, WS_POPUP);
					// set the animation of the window
					dlg.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_RIGHT_TO_LEFT_2);
					dlg.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_LEFT_TO_RIGHT_1);
					dlg.DoModal();
					break;
				}
				case IDC_PPM_TODATE:
				{
					MzOpenSip(IM_SIP_MODE_ADDRESSEE_123);//IM_SIP_MODE_DIGIT);
					Ui_ToDateWnd dlg;
					dlg.setSolarDate(_year,_month,_day);
					LunarSolarDateTime _lt = LunarSolarDateTime(_year,_month,_day);
					_lt.SolarToLunar();
					LSDate lunardate = _lt.getLunarDate();
					dlg.setLunarDate(lunardate.year,lunardate.month,lunardate.day);
					RECT rcWork = MzGetVisibleDesktopRect();//MzGetWorkArea();
					dlg.Create(rcWork.left, rcWork.bottom - 250, RECT_WIDTH(rcWork),250,// RECT_HEIGHT(rcWork),
							m_hWnd, 0, WS_POPUP);
					// set the animation of the window
					dlg.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_RIGHT_TO_LEFT_2);
					dlg.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_LEFT_TO_RIGHT_1);
					int ret = dlg.DoModal();
					if(ret == ID_OK){
						dlg.getDate((DWORD &)_year,(DWORD &)_month,(DWORD &)_day);
						updateGrid();
						updateInfo();
					}
					MzCloseSip();
					break;
				}
				case IDC_PPM_DATES:
				{
					MzOpenSip(IM_SIP_MODE_ADDRESSEE_123);//IM_SIP_MODE_DIGIT);
					Ui_LenDateWnd dlg;
					dlg.setFromDate(_year,_month,_day);
					dlg.setToDate(_year,_month,_day);
					RECT rcWork = MzGetVisibleDesktopRect();//MzGetWorkArea();
					dlg.Create(rcWork.left, rcWork.bottom - 250, RECT_WIDTH(rcWork),250,// RECT_HEIGHT(rcWork),
							m_hWnd, 0, WS_POPUP);
					// set the animation of the window
					dlg.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_RIGHT_TO_LEFT_2);
					dlg.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_LEFT_TO_RIGHT_1);
					int ret = dlg.DoModal();
					if(ret == ID_OK){
						LSDate f,t;
						dlg.getDateFrom(f.year,f.month,f.day);
						dlg.getDateTo(t.year,t.month,t.day);
						int of = LunarSolarDateTime(f.year,f.month,f.day).SolarDaysFromBaseDate();
						int ot = LunarSolarDateTime(t.year,t.month,t.day).SolarDaysFromBaseDate();
						wchar_t msgstr[128];
						wsprintf(msgstr,L"从%d年%d月%d日\n至%d年%d月%d日\n相隔：%d 天",f.year,f.month,f.day,
							t.year,t.month,t.day,ot - of);
						MzCloseSip();
						MzMessageBoxEx(m_hWnd,msgstr,L"计算相隔时间");
					}else{
						MzCloseSip();
					}
					break;
				}
				case IDC_PPM_HISTORY:
				{
					UI_HistoryWnd dlg;
					RECT rcWork = MzGetWorkArea();
					dlg.Create(rcWork.left, rcWork.top, RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork),
							m_hWnd, 0, WS_POPUP);
					// set the animation of the window
					dlg.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_RIGHT_TO_LEFT_2);
					dlg.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_LEFT_TO_RIGHT_1);
					dlg.setupdate(_month,_day);
					dlg.DoModal();
					break;
				}
				case IDC_PPM_RETURN:
				default:
					return;
					break;
				}
				return;
			}
			if(nIndex == 2){	//设定
				Ui_ConfigWnd dlg;
				RECT rcWork = MzGetWorkArea();
				dlg.Create(rcWork.left, rcWork.top, RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork),
						m_hWnd, 0, WS_POPUP);
				// set the animation of the window
				dlg.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_RIGHT_TO_LEFT_2);
				dlg.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_LEFT_TO_RIGHT_1);
				int ret = dlg.DoModal();
				if(ret == ID_OK){
					_showMonthByJieqi = dlg.getJieqiMode();
					updateGrid();
					updateInfo(true);
					showTip();
				}
				return;
			}
		}
	}
}
void Ui_CalendarWnd::showTip(bool bshow){
	//
	if(bshow){
		m_Tipyiji.SetVisible(true);
	}

	if(m_Tipyiji.IsVisible()){
		RECT r = m_Tipyiji.GetRect();

		LunarSolarDateTime _lstm(_year,_month,_day);
		_lstm.SolarToLunar();
		CMzString yi,ji;
		bool ret = _lstm.HuangliYiJi(yi,ji);
		m_Tipyiji.setText(yi.C_Str(),ji.C_Str(),ret);
		int row,col;
		m_Calendar.getSelectedIndex(row,col);
		RECT rcWork;
		rcWork.left = 69 * col + 2;
		if(col > 2){
			rcWork.left = 480 - 320 - 10;
		}
		rcWork.top = m_Calendar.GetTopPos() + 69 * (row + 1) + 28;
		if(row > 4){
			rcWork.left += 69;
			rcWork.top -= 69;
		}

		rcWork.right = rcWork.left + 320;
		rcWork.bottom = rcWork.top + 120;
		m_Tipyiji.SetPos(rcWork.left, rcWork.bottom, RECT_WIDTH(rcWork), RECT_HEIGHT(rcWork));

		RECT r2 = m_Tipyiji.GetRect();

		if(r.left != r2.left || r.top != r2.top){
			Invalidate(&r);
		}
		m_Tipyiji.Invalidate();
		m_Tipyiji.Update();
	}
}

LRESULT Ui_CalendarWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
		case MZ_WM_MOUSE_NOTIFY:
		{
			int nID = LOWORD(wParam);
			int nNotify = HIWORD(wParam);
            short x = LOWORD(lParam);
            short y = HIWORD(lParam);
			if(nID == MZ_IDC_YIJI_TIP && nNotify == MZ_MN_LBUTTONUP){
				if (!m_Tipyiji.IsMouseDownAtScrolling() && !m_Tipyiji.IsMouseMoved()) {
					if(m_Tipyiji.IsVisible()){
						m_Tipyiji.SetVisible(false);
						m_Tipyiji.Invalidate();
						m_Tipyiji.Update();
					}
				}
				return 0;
			}
            if (nNotify == MZ_MN_MOUSEMOVE) {
				if(!_isMouseMoving){
					_isMouseMoving = true;
					_MouseX = x;
					_MouseY = y;
					return 0;
				}
			}

            if (nNotify == MZ_MN_LBUTTONUP) {
				if(_isMouseMoving){
					_isMouseMoving = false;
					if(x > _MouseX + 200 && y < _MouseY + 100 && y > _MouseY - 100){	//右移 下一月
						DateTime::getNextDate(_year,_month);
						_day = 1;
					}
					if(x < _MouseX - 200 && y < _MouseY + 100 && y > _MouseY - 100){	//左移 上一月
						DateTime::getPreDate(_year,_month);
						_day = 1;
					}
					if(y > _MouseY + 200 && x < _MouseX + 100 && x > _MouseX - 100){	//下移 下一年
						_year++;
						_day = 1;
					}
					if(y < _MouseY - 200 && x < _MouseX + 100 && x > _MouseX - 100){	//上移 上一年
						_year--;
						_day = 1;
					}
					updateGrid();
					updateInfo();
					showTip();
					return 0;
				}

                if (!m_Calendar.IsMouseDownAtScrolling() && !m_Calendar.IsMouseMoved()) {
					int r = 0;
					int c = 0;
					bool force = false;
					if(m_Calendar.calcSelectedIndex(x,y,r,c)){
						if(sel_row == r && sel_col == c){
							force = true;
						}
						//check if is invalid selection
						CMzString s = m_Calendar.getText(r,c);
						_day = _wtoi(s.C_Str());
						if(s.Length() == 0){
							return 0;
						}
						updateGrid();
						//m_Calendar.setSelectedIndex(r,c);
						//m_Calendar.Invalidate();
						//m_Calendar.Update();
						updateInfo();
						showTip(force);
					}
                }
                return 0;
            }
#if 0
            if (nID == MZ_IDC_CALENDAR_GRID && nNotify == MZ_MN_LBUTTONDBLCLK) {
                if (!m_Calendar.IsMouseDownAtScrolling() && !m_Calendar.IsMouseMoved()) {
					int r = 0;
					int c = 0;
					if( m_Calendar.calcSelectedIndex(x,y,r,c)){
						//check if is invalid selection
						CMzString s = m_Calendar.getText(r,c);
						if(s.Length() == 0){
							return 0;
						}
						_day = _wtoi(s.C_Str());
						showTip(true);
					}
                }
                return 0;
            }
#endif
			break;
		}
    }
    return CMzWndEx::MzDefWndProc(message, wParam, lParam);
}

void Ui_CalendarWnd::updateInfo(bool forceupdate){
	static int y = 0;
	static int m = 0;
	static int d = 0;
	if(y != _year || 
		m != _month ||
		d != _day || forceupdate){
		LunarSolarDateTime _lstm(_year,_month,_day);
		_lstm.SolarToLunar();
		_lstm.setLunarMonthGanZhiMode(_showMonthByJieqi);
		unsigned char zodiac;
		CMzString zodiacName = _lstm.Zodiac(&zodiac);
		ImagingHelper *pimg = ImagingHelper::GetImageObject(MzGetInstanceHandle(), IDB_PNG1 + zodiac, true);
		//update zodiac image
		m_ZodiacImage.setupImage(pimg);
		m_ZodiacImage.Invalidate();
		m_ZodiacImage.Update();

		//update datetime
		wchar_t datestr[16];
		wsprintf(datestr,L"%d年%d月%d日",_year,_month,_day);
		m_YearMonth.SetText(datestr);
		m_YearMonth.Invalidate();
		m_YearMonth.Update();

		//update ganzhi datetime
		wsprintf(datestr,L"%s年 %s月 %s日",
			_lstm.GanZhiYear().C_Str(),
			_lstm.GanZhiMonth().C_Str(),
			_lstm.GanZhiDay().C_Str());
		m_GanZhiYMD.SetText(datestr);
		m_GanZhiYMD.Invalidate();
		m_GanZhiYMD.Update();
		//update lunar datetime
		wsprintf(datestr,L"农历%s%s日 %s年",
			_lstm.LunarMonth().C_Str(),
			_lstm.OriginalLunarDay().C_Str(),
			zodiacName.C_Str());
		m_LunarMD.SetText(datestr);
		m_LunarMD.Invalidate();
		m_LunarMD.Update();
		y = _year;
		m = _month;
		d = _day;
	}
}