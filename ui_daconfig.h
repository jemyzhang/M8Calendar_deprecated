#ifndef _UI_CALENDARDACONFIG_H
#define _UI_CALENDARDACONFIG_H

// include the MZFC library header file
#include <mzfc_inc.h>
#include "../MzCommon/MzConfig.h"
// Main window derived from CMzWndEx

class CalendarDAConfig : public AppConfigIni{
public:
	CalendarDAConfig()
		:AppConfigIni(L"daconfig.ini"){
            InitIniKey();
	}
protected:
	void InitIniKey(){
        IniClockSize.InitKey(L"DaUi",L"ClockSize",65);
		IniClockXPos.InitKey(L"DaUi",L"ClockXPos",345);
		IniClockYPos.InitKey(L"DaUi",L"ClockYPox",95);
		IniSecColor.InitKey(L"DaUi",L"SecColor",0);
        IniMinColor.InitKey(L"DaUi",L"MinColor",0);
        IniHourColor.InitKey(L"DaUi",L"HourColor",0);
		IniRoundColor.InitKey(L"DaUi",L"RoundColor",RGB(255,215,130));
		IniAutoTimeout.InitKey(L"DaTimeout",L"AutoTimeOut",10);
	}
public:
	MzConfig IniSecColor;
    MzConfig IniMinColor;
    MzConfig IniHourColor;
	MzConfig IniRoundColor;
	MzConfig IniClockSize;
	MzConfig IniClockXPos;
	MzConfig IniClockYPos;
	MzConfig IniAutoTimeout;
};

#endif /*_UI_CALENDARDACONFIG_H*/