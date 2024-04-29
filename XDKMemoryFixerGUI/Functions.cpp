#include"stdafx.h"
#include <time.h>

HMODULE hAPP = NULL;
BOOL IsDevkit = FALSE;

// =========================  ENABLE DEBUG PRINTS  ====================================
#ifdef _DEBUG
VOID Byrom_Print(const CHAR* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
}

#endif

PWCHAR LinkWChar(PWCHAR Text, ...) {
	WCHAR Buffer[0x1000], MessageBuffer[0x100];;

	va_list pArgList;
	va_start(pArgList, Text);
	vswprintf(Buffer, Text, pArgList);
	va_end(pArgList);

	swprintf(MessageBuffer, Buffer);

	return MessageBuffer;
}

VOID XNotify(PWCHAR text)
{
	XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, 0, 2, text, NULL);
}


/*
---------------------------------------------------------------------------------------------------------
XAM FEATURE LIST
----------------------------------------------------------------------------------------------------------
Id             Name	Enabled	Devkit	Heap Size	Image Size	Pool Size	Stack Size	Total Size
1    PRELOADED_HUD	Yes		No 		0x00000000	0x00060000	0x00000000	0x00000000	0x00060000
2        MESSENGER	No 		No 		0x00000000	0x00000000	0x00000000	0x00000000	0x00000000
3              XMP	No 		No 		0x00200000	0x00000000	0x00000000	0x00000000	0x00200000
4        COMMUNITY	Yes		No 		0x00040000	0x000C0000	0x00000000	0x00000000	0x00100000
5             XIME	No 		No 		0x00000000	0x00040000	0x00000000	0x00000000	0x00040000
6          XSTUDIO	No 		No 		0x00000000	0x000A0000	0x00000000	0x00010000	0x000B0000
7   WIRELESS_WAVEA	No 		No 		0x00000000	0x00080000	0x00040000	0x00008000	0x000C8000
33      DEVKIT_HEAP	No 		Yes		0x00060000	0x00000000	0x00000000	0x00000000	0x00060000
34       PIX_STREAM	No 		Yes		0x00060000	0x00000000	0x00013000	0x00000000	0x00073000
35        ETX_BOOST	No 		Yes		0x00000000	0x00000000	0x00010000	0x00000000	0x00010000
36          XS_LOGS	No 		Yes		0x00000000	0x00000000	0x00014000	0x00000000	0x00014000
38          TESTXEX	No 		Yes		0x00000000	0x00020000	0x00000000	0x00000000	0x00020000
39  XAMUIAUTOMATION	No 		Yes		0x00000000	0x00030000	0x00000000	0x00008000	0x00038000
*/

XAM_FEATURE FeatList[13] = {
	{ L"PRELOADED_HUD",		1,	FALSE, FALSE },
	{ L"MESSENGER",			2,	FALSE, FALSE },
	{ L"XMP",				3,	FALSE, FALSE },
	{ L"COMMUNITY",			4,	FALSE, FALSE },
	{ L"XIME",				5,	FALSE, FALSE },
	{ L"XSTUDIO",			6,	FALSE, FALSE },
	{ L"WIRELESS_WAVEA",	7,	FALSE, FALSE },
	{ L"DEVKIT_HEAP",		33, FALSE, FALSE },
	{ L"PIX_STREAM",		34, FALSE, FALSE },
	{ L"ETX_BOOST",			35, FALSE, FALSE },
	{ L"XS_LOGS",			36, FALSE, FALSE },
	{ L"TESTXEX",			38, FALSE, FALSE },
	{ L"XAMUIAUTOMATION",	39, FALSE, FALSE },
};

VOID GrabEnableStates()	// Grabs the current enable state for each Xam feature and update the list above accordingly.
{
	for (int i = 0; i < ARRAYSIZE(FeatList); i++)
	{
		FeatList[i].bOld_En_State = XamFeatureEnabled(FeatList[i].ID); // (Old) We won't be changing this in the ui, we use it for comparison before saving the changes.
		ByromDbg("[GrabEnableStates] Xam feature \"%ls\" is %s", FeatList[i].Feature_Name, FeatList[i].bOld_En_State ? "Enabled" : "Disabled");
		FeatList[i].bNew_En_State = FeatList[i].bOld_En_State; // (New) This is the one we will be changing based on the ui checkbox states.
	}
}

extern "C" {
	NTSYSAPI
		EXPORTNUM(515)
		VOID
		NTAPI
		XamFeatureEnableDisable(
			IN		BOOL EnableDisable,
			IN		INT AppID
		);
}

BOOL SaveEnableStates() // Call this once any changes have been made in the ui. Feature will only be updated if the new enable state bool is different to the old enable state bool.
{
	BOOL bSaveWasRequired = FALSE;
	for (int i = 0; i < ARRAYSIZE(FeatList); i++)
	{
		if (FeatList[i].bNew_En_State != FeatList[i].bOld_En_State) // Means the enable state for this feature has been updated in the ui so we need to save those changes.
		{
			bSaveWasRequired = TRUE;
			ByromDbg("[SaveEnableStates] Xam feature \"%ls\" is now %s", FeatList[i].Feature_Name, FeatList[i].bNew_En_State ? "Enabled" : "Disabled");
			XamFeatureEnableDisable(FeatList[i].bNew_En_State, FeatList[i].ID);
			FeatList[i].bOld_En_State = FeatList[i].bNew_En_State; // Now the changes are saved we should make them match.
		}
	}	

	return bSaveWasRequired;
}