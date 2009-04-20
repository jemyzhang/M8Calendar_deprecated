#include "ui_history.h"
#include "mz_commonfunc.h"
using namespace MZ_CommonFunc;
#include <Mzfc/MzProgressDialog.h> 
#include <fstream>

#include "m8calendar.h"

MZ_IMPLEMENT_DYNAMIC(UI_HistoryWnd)

#define MZ_IDC_TOOLBAR_ABOUT 101
#define MZ_IDC_BUTTON_DETAIL 102
#define MZ_IDC_LIST_HISTORY 103

UI_HistoryWnd::UI_HistoryWnd() {
	int m,d;
	DateTime::getDate(0,&m,&d);
	_month = m;
	_day = d;
	_isConnected = true;
	_detailViewMode = false;
}

bool UI_HistoryWnd::ImportData(TCHAR* filename){

	if(!File::FileExists(filename)){
		return false;
	}

	MzProgressDialog m_Progressdlg;
	m_Progressdlg.SetShowTitle(true);
	m_Progressdlg.SetShowInfo(true);
	m_Progressdlg.SetRange(0,100);
	m_Progressdlg.SetTitle(L"更新历史上的今天数据中，请稍后...");
	m_Progressdlg.SetInfo(L"文件载入中");
	m_Progressdlg.BeginProgress(m_hWnd);

	TEXTENCODE_t enc = File::getTextCode(filename);

	CMzString m_Text;
	if(enc == ttcAnsi){
        ifstream file;
        file.open(filename,  ios::in | ios::binary);
        if (file.is_open())
        {
                file.seekg(0, ios::end);
                int nLen = file.tellg();
                char *ss = new char[nLen+1];
                file.seekg(0, ios::beg);
                file.read(ss, nLen);
                ss[nLen] = '\0';
				wchar_t *wss;
				File::chr2wch(ss,&wss);
                m_Text = wss;
				delete[] ss;
				delete[] wss;
        }
        file.close();
	}else if(enc == ttcUnicode ||
		enc == ttcUnicodeBigEndian){
		wifstream ofile;
        ofile.open(filename, ios::in | ios ::binary);
        if (ofile.is_open())
        {
			ofile.seekg(0, ios::end);
            int nLen = ofile.tellg();
            ofile.seekg(2, ios::beg);
            wchar_t *tmpstr = new wchar_t[nLen + 1];
            ofile.read(tmpstr, nLen);                        
            tmpstr[nLen] = '\0';
            m_Text = tmpstr;
			delete[] tmpstr;
		}
        ofile.close();
	}

	m_Progressdlg.SetInfo(L"文件分析中");
	m_Progressdlg.SetCurValue(20);
	m_Progressdlg.UpdateProgress();
	//导入数据
	wchar_t *pstr = m_Text.C_Str();
	bool newsig = false;
	bool newItem = false;
	bool newContent = false;
	CMzString titlestr,contentstr;
	wchar_t wch = 0;
	int scnt = 0;
	int npos = 0;
	int ncnt = 0;
	int nitems = 0;
	calendar_db.beginTrans();
	do{
		wch = pstr[scnt++];
		if(wch == '#'){
			//写入数据库
			if(newContent){
				newContent = false;
				contentstr = m_Text.SubStr(npos,ncnt);
				CALENDAR_HISTORY_t item;
				swscanf(titlestr.SubStr(0,8).C_Str(),L"%04d%02d%02d",&item.year,&item.month,&item.day);
				item.title = titlestr.SubStr(9,titlestr.Length() - 9).C_Str();
				item.content = contentstr.C_Str();
				calendar_db.appendHistory(&item);
				nitems++;
				npos += ncnt;
				ncnt = 0;
				wchar_t info[32];
				wsprintf(info,L"读入记录中...%d",nitems);
				m_Progressdlg.SetInfo(info);
				m_Progressdlg.UpdateProgress();
			}
			if(newsig){
				//
				newsig = false;
				newItem = true;
				npos++;
				continue;
			}else{
				newsig = true;
				npos++;
				continue;
			}
		}
		if(wch == '\n' || wch == '\r'){
			if(newItem){
				newItem = false;
				newContent = true;
				if(ncnt != 0){
					titlestr = m_Text.SubStr(npos,ncnt);
					npos += (ncnt+1);
					ncnt = 0;
				}else{
					npos++;	//忽略换行符
				}
				continue;
			}
		}
		ncnt++;
	}while(wch != '\0');
	m_Progressdlg.SetInfo(L"写入数据中...");
	m_Progressdlg.SetCurValue(40);
	m_Progressdlg.UpdateProgress();
	calendar_db.commitTrans();
//	m_Progressdlg.SetInfo(L"数据优化中...");
//	m_Progressdlg.SetCurValue(60);
//	m_Progressdlg.UpdateProgress();
	calendar_db.reorgDatebase();
	m_Progressdlg.SetInfo(L"创建索引...");
	m_Progressdlg.SetCurValue(80);
	m_Progressdlg.UpdateProgress();
	calendar_db.indexDatabase();
	m_Progressdlg.SetInfo(L"更新完成");
	m_Progressdlg.SetCurValue(100);
	m_Progressdlg.EndProgress();
	File::DelFile(filename);
	return true;
}

BOOL UI_HistoryWnd::OnInitDialog() {
    // Must all the Init of parent class first!
    if (!CMzWndEx::OnInitDialog()) {
        return FALSE;
    }

    // Then init the controls & other things in the window
    int y = 0;
    m_CaptionTitle.SetPos(0, 0, GetWidth(), MZM_HEIGHT_CAPTION);
	m_CaptionTitle.SetText(L"历史上的今天");
	m_CaptionTitle.SetDrawTextFormat(m_CaptionTitle.GetDrawTextFormat() | DT_END_ELLIPSIS);
    AddUiWin(&m_CaptionTitle);

	y += MZM_HEIGHT_CAPTION;
    m_ListHistory.SetPos(0, y, GetWidth(), GetHeight() - y - MZM_HEIGHT_TEXT_TOOLBAR);
	m_ListHistory.SetID(MZ_IDC_LIST_HISTORY);
	m_ListHistory.EnableNotifyMessage(true);
    AddUiWin(&m_ListHistory);

	m_Toolbar.SetPos(0, GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);
    m_Toolbar.SetButton(0, true, true, L"返回");
    m_Toolbar.EnableLeftArrow(true);
    m_Toolbar.SetID(MZ_IDC_TOOLBAR_ABOUT);
    AddUiWin(&m_Toolbar);

	m_BtnDetail.SetText(L"查看");
	m_BtnDetail.SetPos(0,0,80,50);
	m_BtnDetail.SetTextColor(RGB(64,64,128));
	m_BtnDetail.SetButtonType(MZC_BUTTON_DOWNLOAD);
	m_BtnDetail.SetID(MZ_IDC_BUTTON_DETAIL);
	m_ListHistory.setupButton(&m_BtnDetail);

	//Detail view
	y = MZM_HEIGHT_CAPTION;
//	m_Scrollwin.SetPos(0,y,GetWidth(),GetHeight() - y - MZM_HEIGHT_TEXT_TOOLBAR);
//	AddUiWin(&m_Scrollwin);
//	m_Scrollwin.SetVisible(false);

	m_EdtDetail.SetPos(0,y,GetWidth(),GetHeight() - y - MZM_HEIGHT_TEXT_TOOLBAR);
	m_EdtDetail.SetReadOnly(true);
	m_EdtDetail.SetInsideScroll(true);
	m_EdtDetail.SetTextSize(20);
	AddUiWin(&m_EdtDetail);
	m_EdtDetail.SetVisible(false);

	return TRUE;
}

void UI_HistoryWnd::OnMzCommand(WPARAM wParam, LPARAM lParam) {
    UINT_PTR id = LOWORD(wParam);
    switch (id) {
		case MZ_IDC_BUTTON_DETAIL:
		{
			setupDetailView(m_ListHistory.GetSelectedIndex());
			break;
		}
        case MZ_IDC_TOOLBAR_ABOUT:
        {
            int nIndex = lParam;
            if (nIndex == 0) {
				if(_detailViewMode){
					_detailViewMode = false;
					m_CaptionTitle.SetText(L"历史上的今天");
					m_CaptionTitle.Invalidate();
					m_CaptionTitle.Update();

					m_EdtDetail.SetVisible(false);
					m_EdtDetail.Invalidate();
					m_EdtDetail.Update();

					m_ListHistory.SetVisible(true);
					m_ListHistory.Invalidate();
					m_ListHistory.Update();

					//m_Scrollwin.SetVisible(false);
					//m_Scrollwin.Invalidate();
					//m_Scrollwin.Update();
				}else{
					// exit the modal dialog
					EndModal(ID_OK);
				}
                return;
            }
        }
    }
}
void UI_HistoryWnd::setupDetailView(int idx){
	_detailViewMode = true;
	m_ListHistory.SetVisible(false);
	m_ListHistory.Invalidate();
	m_ListHistory.Update();

	//
	CALENDAR_HISTORY_ptr ph = calendar_db.historyByIndex(idx);
	CALENDAR_HISTORY_t hs;
	hs.year = ph->year;
	hs.month = ph->month; hs.day = ph->day;
	C::newstrcpy(&hs.title,ph->title);
	hs.content = 0;
	calendar_db.getHistoryDetailByDate(&hs);


	m_CaptionTitle.SetText(hs.title);
	m_CaptionTitle.Invalidate();
	m_CaptionTitle.Update();

	m_EdtDetail.SetText(hs.content);
	m_EdtDetail.SetVisible(true);
	m_EdtDetail.Invalidate();
	m_EdtDetail.Update();

	delete[] hs.title; delete[] hs.content;
	//m_Scrollwin.SetVisible(true);
	//m_Scrollwin.Invalidate();
	//m_Scrollwin.Update();
}

void UI_HistoryWnd::setupdate(DWORD month,DWORD day){
	if(month == -1 || day == -1){
		int m,d;
		DateTime::getDate(0,&m,&d);
		month = m;
		day = d;
	}

	wchar_t currpath[128];
	
	if(File::GetCurrentPath(currpath)){
		wsprintf(db_path,L"%s\\calendar.db",currpath);
	}else{
		wsprintf(db_path,DEFAULT_DB);
	}
	if(calendar_db.connect(db_path)){
		//popup password dialog
		//try if there is a password
		calendar_db.decrypt(L"PASSWORD",8);
		//检查记录版本
		calendar_db.recover();
		if(!calendar_db.checkDatabaseVersion()){
			_isConnected = false;
			return;
		}
		//calendar_db.encrypt(0,0);
	}else{
		//检查记录版本
		calendar_db.recover();
		calendar_db.encrypt(L"PASSWORD",8);
	}
	//导入记录
	wchar_t db_txt[256];
	wsprintf(db_txt,L"%s\\历史上的今天.txt",currpath);
	ImportData(db_txt);

	calendar_db.getHistoryListByDate(month,day);

	m_ListHistory.RemoveAll();

    ListItem li;
    CMzString str;

	list<CALENDAR_HISTORY_ptr>::iterator i = calendar_db.list_search_history.begin();
    for (; i != calendar_db.list_search_history.end(); i++) {
        m_ListHistory.AddItem(li);
    }
	m_ListHistory.setupdb(&calendar_db);
	m_ListHistory.Invalidate();
	m_ListHistory.Update();
	_month = month;
	_day = day;
}

void UiHistoryList::DrawItem(HDC hdcDst, int nIndex, RECT* prcItem, RECT *prcWin, RECT *prcUpdate){
	if(_db == 0) return;

	CALENDAR_HISTORY_ptr ph= _db->historyByIndex(nIndex);
	if(ph == 0) return;

    // draw the high-light background for the selected item
    if (nIndex == GetSelectedIndex()) {
        MzDrawSelectedBg(hdcDst, prcItem);
		if(_btn){
			_btn->SetPos(prcItem->right - _btn->GetWidth() - 10,
				prcItem->top + (prcItem->bottom - prcItem->top - _btn->GetHeight())/2 - prcWin->top,
				0,0,SP_NOSIZE);
			AddChild(_btn);
		}
    }
	RECT rcText = *prcItem;
	RECT Rect01;
	RECT Rect02;

	//标题
	COLORREF cr = RGB(0,0,0);       
	::SetTextColor( hdcDst , cr );
    HFONT hf;
	hf = FontHelper::GetFont( 25 );
	SelectObject(hdcDst,hf);
	Rect01.top = rcText.top; Rect01.bottom = Rect01.top + (rcText.bottom - rcText.top)/2;
	Rect01.left = rcText.left; Rect01.right = rcText.right;
	wchar_t datestr[16];
	wsprintf(datestr,L"(%d年%d月%d日)",ph->year,ph->month,ph->day);
	CMzString s = ph->title;
	s = s + datestr;
	MzDrawText( hdcDst , s.C_Str(), &Rect01 , DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
	DeleteObject( hf );

	//内容
	cr = RGB(0,0,0);
	hf = FontHelper::GetFont( 18 );
	SelectObject(hdcDst,hf);
	Rect02.top = Rect01.bottom; Rect02.bottom = rcText.bottom;
	Rect02.left = rcText.left; Rect02.right = rcText.right;
	s = ph->content;
	MzDrawText( hdcDst , s.SubStr(0,30).C_Str(), &Rect02 , DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
	DeleteObject( hf );
}

LRESULT UI_HistoryWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case MZ_WM_MOUSE_NOTIFY:
        {
            int nID = LOWORD(wParam);
            int nNotify = HIWORD(wParam);
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (nID == MZ_IDC_LIST_HISTORY && nNotify == MZ_MN_LBUTTONDOWN) {
                if (!m_ListHistory.IsMouseDownAtScrolling() && !m_ListHistory.IsMouseMoved()) {
                    int nIndex = m_ListHistory.CalcIndexOfPos(x, y);
                    m_ListHistory.SetSelectedIndex(nIndex);
					m_BtnDetail.SetVisible(true);
                    m_ListHistory.Invalidate();
                    m_ListHistory.Update();
                }
                return 0;
            }
            if (nID == MZ_IDC_LIST_HISTORY && nNotify == MZ_MN_MOUSEMOVE) {
                m_ListHistory.SetSelectedIndex(-1);
				m_BtnDetail.SetVisible(false);
                m_ListHistory.Invalidate();
                m_ListHistory.Update();
                return 0;
            }
            if (nID == MZ_IDC_LIST_HISTORY && nNotify == MZ_MN_LBUTTONDBLCLK) {
                if (!m_ListHistory.IsMouseDownAtScrolling() && !m_ListHistory.IsMouseMoved()) {
                    int nIndex = m_ListHistory.CalcIndexOfPos(x, y);
					if(nIndex == -1) return 0;
					//显示详情
					setupDetailView(nIndex);
				}
	            return 0;
			}
		}
    }
    return CMzWndEx::MzDefWndProc(message, wParam, lParam);
}

