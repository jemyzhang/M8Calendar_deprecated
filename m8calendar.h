#include "calendardb.h"
#include "ui_calendar.h"

#define VER_STRING L"0.1"
#define BUILD_STRING L"20090415"
#define APPNAME L"’∆…œ≈©¿˙"

// Application class derived from CMzApp
extern wchar_t db_path[256];
//extern wchar_t ini_path[256];
extern clsCASHDB calendar_db;
//extern wchar_t ini_reminder[256];
//extern CashReminder calendar_reminder;

class M8CashApp : public CMzApp {
public:
    // The main window of the app.
    Ui_CalendarWnd m_MainWnd;

    //check pwd
    bool checkpwd();
    // Initialization of the application
    virtual BOOL Init();
};
