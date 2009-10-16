#pragma once
// include the MZFC library header file
#include <mzfc_inc.h>
#include <windows.h>

#include <list>
#include "sqlite3\sqlite3.h"

using namespace std;

#ifdef _DEBUG
#define DEFAULT_DB		L"Program Files\\M8Calendar\\calendar.db"
#else
#define DEFAULT_DB		L"\\Disk\\Programs\\M8Calendar\\calendar.db"
//#define DEFAULT_DB		"Program Files\\M8Calendar\\calendar.db"
#endif

#define TABLE_HISTORY	L"HISTORY_v1"
#define HISTORY_TBL_ITEM L"YEAR numeric not null,MONTH_DAY numeric not null,TITLE text NOT NULL primary key,CONTENT text NOT NULL"
#define HISTORY_TBL_INSERT L"(YEAR,MONTH_DAY,TITLE,CONTENT) values(%d,%d,'%s',?)"

typedef struct CALENDAR_HISTORY {
    DWORD year;
    DWORD month_day;
    wchar_t* title;
    wchar_t* content;
    CALENDAR_HISTORY(){
        year = 0;
        month_day = 0;
        title = NULL;
        content = NULL;
    }
    ~CALENDAR_HISTORY(){
        if(title){
            delete [] title;
            title = NULL;
        }
        if(content){
            delete [] content;
            content = NULL;
        }
    }
} CALENDAR_HISTORY_t, *CALENDAR_HISTORY_ptr;

class clsCalendarDB {
public:
    clsCalendarDB( );
    ~clsCalendarDB( );
public:
	bool connect(wchar_t *path);
	bool disconnect() { disconnectDatabase(); return true; }
	void recover();
	bool decrypt(wchar_t* pwd,int len);
	bool encrypt(wchar_t* pwd,int len);
	void clean();	//clean rabish
public:
    list<CALENDAR_HISTORY_ptr> list_search_history;
    //sqlite operations
private:
    sqlite3* db;
	sqlite3_stmt* pStmt;
	const void* pzTail;
	wchar_t sqlcmdw[512];
protected:
    void connectDatabase(const wchar_t*);
    void disconnectDatabase();
    void createDefaultDatabase();
	bool decrytpDatabase(const char* pwd,int len);	//true: successful
	bool setDatabasePassword(const char* pwd,int len);	//true: successful
protected:
	bool searchHistory(const wchar_t* sqlcmdw);
private:
	void clearHistorySearchResults();
public:
	bool HistoryExists(CALENDAR_HISTORY_ptr his);
	int appendHistory(CALENDAR_HISTORY_ptr his);
	bool getHistoryListByDate(int month_day);
	CALENDAR_HISTORY_ptr historyByIndex(int index);
	bool getHistoryDetailByDate(CALENDAR_HISTORY_ptr his);
public:
	int beginTrans(){
		bool rc = false;
		wsprintf(sqlcmdw,L"begin transaction;");
		if (sqlite3_prepare16(db, sqlcmdw, -1, &pStmt, &pzTail) == SQLITE_OK) {
			sqlite3_step(pStmt);
		}
		sqlite3_finalize(pStmt);
		return rc;
	}
	int commitTrans(){
		bool rc = false;
		wsprintf(sqlcmdw,L"commit transaction;");
		if (sqlite3_prepare16(db, sqlcmdw, -1, &pStmt, &pzTail) == SQLITE_OK) {
			sqlite3_step(pStmt);
		}
		sqlite3_finalize(pStmt);
		return rc;
	}
	bool indexDatabase(){
		bool rc = false;
		wsprintf(sqlcmdw,L"create index idx_history on '"TABLE_HISTORY 
            L"'(month_day,title)");
		if (sqlite3_prepare16(db, sqlcmdw, -1, &pStmt, &pzTail) == SQLITE_OK) {
			sqlite3_step(pStmt);
		}
		sqlite3_finalize(pStmt);
		return rc;
	}
	bool reorgDatebase(){
		bool rc = false;
		wsprintf(sqlcmdw,L"VACUUM");
		if (sqlite3_prepare16(db, sqlcmdw, -1, &pStmt, &pzTail) == SQLITE_OK) {
			sqlite3_step(pStmt);
		}
		sqlite3_finalize(pStmt);
		return rc;
	}
	bool checkDatabaseVersion(){
		bool rc = false;
		wsprintf(sqlcmdw,L"select count(*) from sqlite_master where name=\"%s\"",TABLE_HISTORY);
		if (sqlite3_prepare16(db, sqlcmdw, -1, &pStmt, &pzTail) == SQLITE_OK) {
			while (sqlite3_step(pStmt) == SQLITE_ROW) {
				int count = _wtoi((LPWSTR) sqlite3_column_text16(pStmt, 0));
				if(count == 1){
					rc = true;
				}
			}
		}
		sqlite3_finalize(pStmt);
		return rc;
	}

};

