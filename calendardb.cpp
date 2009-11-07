#include "calendardb.h"
//#include "pinyin_sort.h"
#include "..\MzCommon\MzCommon.h"
using namespace MzCommon;
#pragma comment(lib,"libsqlitece_s.lib")

clsCalendarDB::clsCalendarDB() {
}

clsCalendarDB::~clsCalendarDB() {
    disconnectDatabase();
	clean();
}

bool clsCalendarDB::connect(wchar_t* dbpath){
	bool rc = false;
	if(File::FileExists(dbpath)){	//file exists
		rc = true;
	}
	connectDatabase(dbpath);
	//createDefaultDatabase(dbpath);
	return rc;
}
void clsCalendarDB::recover(){
	createDefaultDatabase();
}

bool clsCalendarDB::decrypt(wchar_t* pwd, int len){
	char* temp = new char[len*2+1];
	int bytecnt = 0;
	wchar_t *p = pwd;
	char* b_pwd = temp;
	for(int i = 0; i < len; i++){
		wchar_t w = *p++;
		if(w&0xff){
			*b_pwd++ = w&0xff;
			bytecnt++;
		}
		if((w>>8)&0xff){
			*b_pwd++ = (w>>8)&0xff;
			bytecnt++;
		}
	}
	*b_pwd = '\0';
	bool ret = decrytpDatabase(temp,bytecnt);
	delete temp;
	return ret;
}

bool clsCalendarDB::encrypt(wchar_t* pwd, int len){
	char* temp = new char[len*2+1];
	int bytecnt = 0;
	wchar_t *p = pwd;
	char* b_pwd = temp;
	for(int i = 0; i < len; i++){
		wchar_t w = *p++;
		if(w&0xff){
			*b_pwd++ = w&0xff;
			bytecnt++;
		}
		if((w>>8)&0xff){
			*b_pwd++ = (w>>8)&0xff;
			bytecnt++;
		}
	}
	*b_pwd = '\0';
	bool ret =  setDatabasePassword(temp,bytecnt);
	delete temp;
	return ret;
}

void clsCalendarDB::clean(){
	clearHistorySearchResults();
}

bool clsCalendarDB::HistoryExists(CALENDAR_HISTORY_ptr his) {
	bool nRet = true;
    int rc = 0;
    wsprintf(sqlcmdw, L"SELECT COUNT(*) FROM "TABLE_HISTORY
		L" WHERE TITLE='%s'",
		his->title);
	rc = sqlite3_prepare16(db,sqlcmdw,-1,&pStmt,&pzTail);
    if (!rc) {
		rc = sqlite3_step(pStmt);
		if(rc != SQLITE_DONE){
			nRet = sqlite3_column_int(pStmt, 0) > 0;
		}
    }
    sqlite3_finalize(pStmt);
    return nRet;
}

int clsCalendarDB::appendHistory(CALENDAR_HISTORY_ptr his) {
    int rc = 0;
//	if(HistoryExists(his)) return rc;	//ÖØ¸´¼ÇÂ¼

    wsprintf(sqlcmdw, L"INSERT INTO "TABLE_HISTORY
        L" "HISTORY_TBL_INSERT,
		his->year,his->month_day,
		his->title);
	rc = sqlite3_prepare16(db,sqlcmdw,-1,&pStmt,&pzTail);
    if (rc == SQLITE_OK) {
        rc = sqlite3_bind_text16(pStmt,1,his->content,lstrlen(his->content) * sizeof(his->content[0]),SQLITE_TRANSIENT);
		rc = sqlite3_step(pStmt);
    }else{
        int test = lstrlen(his->content);
        int test2 = lstrlen(sqlcmdw);
        int a= 0;
    }
	rc = sqlite3_finalize(pStmt);
    return rc;
}

void clsCalendarDB::connectDatabase(const wchar_t * dbfile) {
    const wchar_t* f = dbfile;
    if (f == NULL) {
        f = DEFAULT_DB;
    }
    int rc = sqlite3_open16(f, &db);

    return;
}

void clsCalendarDB::disconnectDatabase() {
    sqlite3_close(db);
    return;
}

void clsCalendarDB::createDefaultDatabase() {
    int rc = 0;
    //what ever create db
    //UNIQUE solved duplication problem
    //create account table
    wsprintf(sqlcmdw, L"CREATE TABLE %s (%s);", TABLE_HISTORY, HISTORY_TBL_ITEM);

    if (sqlite3_prepare16(db,sqlcmdw,-1,&pStmt,&pzTail) == SQLITE_OK) {
		sqlite3_step(pStmt);
    }
	sqlite3_finalize(pStmt);
    return;
}

CALENDAR_HISTORY_ptr clsCalendarDB::historyByIndex(int index) {
	if(index >= list_search_history.size() || index < 0) return 0;
    list<CALENDAR_HISTORY_ptr>::iterator i = list_search_history.begin();
    for (int j = 0; j < index; j++) i++;
    return *i;
}
bool clsCalendarDB::decrytpDatabase(const char* pwd,int len){
	return (sqlite3_key(db,pwd,len) == SQLITE_OK);
}

bool clsCalendarDB::setDatabasePassword(const char* pwd,int len){
	return (sqlite3_rekey(db,pwd,len) == SQLITE_OK);
}

void clsCalendarDB::clearHistorySearchResults(){
	if(!list_search_history.empty()){
		list<CALENDAR_HISTORY_ptr>::iterator i = list_search_history.begin();
		for(;i != list_search_history.end();i++){
			CALENDAR_HISTORY_ptr his = *i;
			delete *i;
		}
		list_search_history.clear();
	}
}

bool clsCalendarDB::searchHistory(const wchar_t* sqlcmdw){
    bool rc = true;

	//do some clear work
	clearHistorySearchResults();

    if (sqlite3_prepare16(db, sqlcmdw, -1, &pStmt, &pzTail) == SQLITE_OK) {
        while (sqlite3_step(pStmt) == SQLITE_ROW) {
            CALENDAR_HISTORY_ptr h = new CALENDAR_HISTORY_t;
            h->year = sqlite3_column_int(pStmt, 0);
            h->month_day = sqlite3_column_int(pStmt, 1);

			C::newstrcpy(&h->title,(LPWSTR) sqlite3_column_text16(pStmt, 2));
			C::newstrcpy(&h->content,(LPWSTR) sqlite3_column_text16(pStmt, 3),30);

			list_search_history.push_back(h);
        }
	}else{
		rc = false;
	}
    sqlite3_finalize(pStmt);
	if(list_search_history.empty()) rc = false;
	return rc;
}

bool clsCalendarDB::getHistoryListByDate(int month_day){
    wsprintf(sqlcmdw,L"SELECT * FROM '"TABLE_HISTORY
        L"' WHERE MONTH_DAY=%d ORDER BY YEAR DESC",
        month_day);
	return searchHistory(sqlcmdw);
}

bool clsCalendarDB::getHistoryDetailByDate(CALENDAR_HISTORY_ptr his){
    bool rc = true;
	wsprintf(sqlcmdw,L"select CONTENT from '"TABLE_HISTORY
        L"' where TITLE='%s'",
		his->title);
    if (sqlite3_prepare16(db, sqlcmdw, -1, &pStmt, &pzTail) == SQLITE_OK) {
        if (sqlite3_step(pStmt) == SQLITE_ROW) {
			C::newstrcpy(&his->content,(LPWSTR) sqlite3_column_text16(pStmt, 0));
        }
	}else{
		rc = false;
	}
    sqlite3_finalize(pStmt);
	return rc;
}