#pragma once

// include the MZFC library header file
#include <mzfc_inc.h>

#define VER_STRING L"1.80"
#define BUILD_STRING L"20091106"
#define APPNAME L"’∆…œ≈©¿˙"

// Popup window derived from CMzWndEx

class UI_AboutWnd : public CMzWndEx {
    MZ_DECLARE_DYNAMIC(UI_AboutWnd);
public:
protected:
    UiToolbar_Text m_Toolbar;
    UiCaption m_CaptionTitle;
	UiStatic m_TextName;
	UiStatic m_TextAuthor;
	UiStatic m_TextVersion;

    // Initialization of the window (dialog)
    virtual BOOL OnInitDialog();

    // override the MZFC command handler
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
private:
};
