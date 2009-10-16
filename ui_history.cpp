#include "ui_history.h"
#include "..\MzCommon\MzCommon.h"
using namespace MzCommon;
#include <Mzfc/MzProgressDialog.h> 
#include <fstream>

#include "m8calendar.h"
#include "ui_config.h"

extern CalendarConfig AppConfig;
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

UINT UI_HistoryWnd::identHistodayLine(LPTSTR linetext){
    UINT nRet = 2;
    if(linetext == NULL) nRet = 0;  //无效
    if(linetext[0] == '\0' || linetext[0] == '\x0d' || linetext[0] == '\x0a') nRet = 0;    //空白、换行符
    if(linetext[0] == ' ' && linetext[1] == '=') nRet = 0; //编辑人注释
    if(linetext[0] == '#' && linetext[1] == '#') nRet = 1;  //标题
    return nRet;   //内容
}

bool UI_HistoryWnd::getEntryData(LPTSTR linetext,bool bEnd){
    bool nRet = false;
    static CALENDAR_HISTORY_t item;
    static bool newEntryTitle = false;
    static bool newEntryContent = false;
    if(bEnd){
        if(newEntryTitle){    //存在未保存的条目
            //保存条目
            if(newEntryContent){
                newEntryContent = false;
                calendar_db.appendHistory(&item);
                delete [] item.content;
                item.content = NULL;
                nRet = true;
            }
        }
        return nRet;
    }
    switch(identHistodayLine(linetext)){
        case 1://标题
            {
                if(newEntryTitle){    //存在未保存的条目
                    //保存条目
                    if(newEntryContent){
                        newEntryContent = false;
                        calendar_db.appendHistory(&item);
                        delete [] item.content;
                        item.content = NULL;
                        nRet = true;
                    }
                }
                newEntryTitle = true;
                int nlen = lstrlen(linetext);
                wchar_t * wtitle = linetext;//略过日期和##和空格
                while(*wtitle != ' ' && wtitle < linetext+nlen) wtitle++;
                *wtitle = '\0'; //分割日期与标题
                wtitle++;
                wtitle[lstrlen(wtitle) - 1] = '\0'; //去除title后的回车符号'\r''\x13'
                C::newstrcpy(&item.title,wtitle);
                wchar_t *wDate = linetext + 2;//略过##
                wchar_t *wMonth;
                wchar_t *wDay;
                wchar_t *wYear;
                nlen = lstrlen(wDate);
                if(nlen < 7){ //日期长度不合法
                    newEntryTitle = false;
                    break;
                }
                wDay = wDate + nlen - 2;
                item.day = _wtoi(wDay);
                *wDay = '\0';
                wMonth = wDate + nlen - 4;
                item.month = _wtoi(wMonth);
                *wMonth = '\0';
                wYear = wDate;
                item.year = _wtoi(wYear);
                break;
            }
        case 2://内容
            newEntryContent = true;
            if(item.content == NULL){
                int wlen = lstrlen(linetext) + 2;
                item.content = new wchar_t[wlen];
                wcscpy_s(item.content,wlen,linetext);
                item.content[wlen - 2] = '\n';
                item.content[wlen - 1] = '\0';
            }else{
                wchar_t *strtmp = item.content;
                int wlen = lstrlen(linetext) + 2 + lstrlen(strtmp);
                item.content = new wchar_t[wlen];
                wcscpy_s(item.content,wlen,strtmp);
                wcscat_s(item.content,wlen,linetext);
                item.content[wlen - 2] = '\n';
                item.content[wlen - 1] = '\0';
                delete [] strtmp;
            }
            break;
        case 0://无效
        default:
            break;
    }
    return nRet;
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
	m_Progressdlg.SetInfo(L"文件分析中");
	m_Progressdlg.BeginProgress(m_hWnd);

	TEXTENCODE_t enc = File::getTextCode(filename);

	int nitems = 0;
	unsigned int nbytes = 0;
	calendar_db.beginTrans();
    int nPercent = 0;
    int _nPercent = 1;
	if(enc == ttcAnsi){
        ifstream file;
        file.open(filename,  ios::in | ios::binary);
        if (file.is_open())
        {
            file.seekg(0, ios::end);
            unsigned int nLen = file.tellg();
            char *sbuf = new char[nLen+1];
            file.seekg(0, ios::beg);
			while(!file.eof()){
                if(file.getline(sbuf,nLen).good()){
                    nbytes += strlen(sbuf);
                    wchar_t *wss;
                    File::chr2wch(sbuf,&wss);
                    if(getEntryData(wss)){
                        nitems++;
                        nPercent = 100 * nbytes / nLen;
                        if(nPercent != _nPercent){
                            _nPercent = nPercent;
                            wchar_t info[32];
                            wsprintf(info,L"读入%d条记录...%d%%",nitems,nPercent);
                            m_Progressdlg.SetInfo(info);
                            m_Progressdlg.SetCurValue(80 * nbytes / nLen);
                            m_Progressdlg.UpdateProgress();
                            DateTime::waitms(1);
                        }
                    }
                }else{
                    //错误发生
                }
			}
			delete[] sbuf;
        }
        file.close();
	}else if(enc == ttcUnicode ||
		enc == ttcUnicodeBigEndian){
		wifstream ofile;
        ofile.open(filename, ios::in | ios ::binary);
        if (ofile.is_open())
        {
            ofile.seekg(0, ios::end);
            unsigned int nLen = ofile.tellg();
            wchar_t *sbuf = new wchar_t[nLen+1];
            ofile.seekg(2, ios::beg);
            while(!ofile.eof()){
                if(ofile.getline(sbuf,nLen).good()){
                    nbytes += lstrlen(sbuf);
                    if(getEntryData(sbuf)){
                        nitems++;
                        wchar_t info[32];
                        wsprintf(info,L"读入记录中...%d",nitems);
                        m_Progressdlg.SetInfo(info);
                        m_Progressdlg.SetCurValue(80 * nbytes / nLen);
                        m_Progressdlg.UpdateProgress();
                        DateTime::waitms(1);
                    }
                }else{
                    //错误发生
                }
            }
            delete [] sbuf;
		}
        ofile.close();
	}

    getEntryData(0,true);   //最后一条记录

	m_Progressdlg.SetInfo(L"写入数据中...");
	m_Progressdlg.SetCurValue(85);
	m_Progressdlg.UpdateProgress();
	calendar_db.commitTrans();
//	m_Progressdlg.SetInfo(L"数据优化中...");
//	m_Progressdlg.SetCurValue(90);
//	m_Progressdlg.UpdateProgress();
//	calendar_db.reorgDatebase();
//	m_Progressdlg.SetInfo(L"创建索引...");
//	m_Progressdlg.SetCurValue(90);
//	m_Progressdlg.UpdateProgress();
//	calendar_db.indexDatabase();
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
    m_EdtDetail.EnableScrollBarV(true);
    m_EdtDetail.SetTextSize(20 + 5 * AppConfig.IniHistodayFontSize.Get());
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
		//calendar_db.decrypt(L"PASSWORD",8);
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
		//calendar_db.encrypt(L"PASSWORD",8);
	}
	//导入记录
	wchar_t db_txt[256];
	wsprintf(db_txt,L"%s\\历史上的今天.txt",currpath);
	if(!ImportData(db_txt)){
		wsprintf(db_txt,L"%s\\histoday.dat",currpath);
		ImportData(db_txt);
	}

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
    wchar_t *titlestr = new wchar_t[lstrlen(ph->title) + 1 + 16];
    wsprintf(titlestr,L"[%04d年] %s",ph->year,ph->title);
	MzDrawText( hdcDst , titlestr, &Rect01 , DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
	DeleteObject( hf );
    delete [] titlestr;

	//内容
	cr = RGB(0,0,0);
	hf = FontHelper::GetFont( 18 );
	SelectObject(hdcDst,hf);
	Rect02.top = Rect01.bottom; Rect02.bottom = rcText.bottom;
	Rect02.left = rcText.left; Rect02.right = rcText.right;
	MzDrawText( hdcDst , ph->content, &Rect02 , DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
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

