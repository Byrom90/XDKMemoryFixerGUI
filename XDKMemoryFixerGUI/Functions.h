#ifndef __Functions_H__
#define __Functions_H__
#pragma once

#define BuildVers 1.01
extern HMODULE hAPP;
extern BOOL IsDevkit;

// =========================  ENABLE DEBUG PRINTS  ====================================
#define _DEBUG

#ifdef _DEBUG
VOID Byrom_Print(const CHAR* fmt, ...);
#define ByromDbg(x, ...) { do { Byrom_Print("[XDKMemoryFixerGUI] "  x "\n", __VA_ARGS__); } while(0); }
#else
#define ByromDbg
#endif

PWCHAR LinkWChar(PWCHAR Text, ...);

VOID XNotify(PWCHAR text);

typedef struct XAM_FEATURE_STRUCT {
	LPCWSTR Feature_Name;
	INT ID;
	BOOL bOld_En_State;
	BOOL bNew_En_State;
} XAM_FEATURE, * PXAM_FEATURE;

extern XAM_FEATURE FeatList[13];
VOID GrabEnableStates();
BOOL SaveEnableStates();

#endif // !__Functions_H