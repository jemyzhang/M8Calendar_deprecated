#include "ui_calendar.h"

#define VER_STRING L"1.60"
#define BUILD_STRING L"20091016"
#define APPNAME L"’∆…œ≈©¿˙"

// Application class derived from CMzApp
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
