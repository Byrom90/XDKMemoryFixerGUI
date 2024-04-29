#include "stdafx.h"
#include <xbdm.h>
#include <time.h>


#pragma region APP
// Register custom classes.
HRESULT CMyApp::RegisterXuiClasses()
{
	CMainScene::Register();
	return S_OK;
}

// Unregister custom classes.
HRESULT CMyApp::UnregisterXuiClasses()
{
	CMainScene::Unregister();
	return S_OK;
}
#pragma endregion

//==============================================================================================================================================================================
//
//			CLASS TO HANDLE MainScene
//
//==============================================================================================================================================================================
#pragma region MainScene
extern BOOL bShouldExit;
BOOL bListPopulated = FALSE;

HRESULT CMainScene::OnInit(XUIMessageInit* pInitData,
	BOOL& bHandled)
{
	GetChildById(L"textNotXDK", &textNotXDK);
	GetChildById(L"textBottomInfo", &textBottomInfo);
	GetChildById(L"textVersion", &textVersion);
	GetChildById(L"btnBACK_Exit", &btnBACK_Exit);
	GetChildById(L"btnX_Save", &btnX_Save);
	GetChildById(L"btnA_Toggle", &btnA_Toggle);
	GetChildById(L"List_Features", &List_Features);

	if (!IsDevkit)
	{ 
		// Show the error text
		textNotXDK.SetShow(TRUE);

		// Disable and hide anything that accepts user input. Keep the exit button enabled & shown
		btnX_Save.SetShow(FALSE);
		btnX_Save.SetEnable(FALSE);
		btnA_Toggle.SetShow(FALSE);
		btnA_Toggle.SetEnable(FALSE);
		List_Features.SetShow(FALSE);
		List_Features.SetEnable(FALSE);

		bHandled = TRUE;
		return S_OK;
	}

	textVersion.SetText(LinkWChar(L"Build:%.02f", BuildVers));
	PopulateCheckList();

	bHandled = TRUE;
	return S_OK;
}

HRESULT CMainScene::OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled)
{
	if (hObjPressed == btnBACK_Exit)
	{
		//ByromDbg("[OnNotifyPress] btnBACK_Exit");
		bShouldExit = TRUE;
	}
	else if (hObjPressed == btnX_Save)
	{
		//ByromDbg("[OnNotifyPress] btnX_Save");
		if (SaveEnableStates())
			XNotify(L"Changes Saved!\nCold reboot is required for them to take effect");
		else
			XNotify(L"No changes to save!");
	}
	else if (hObjPressed == List_Features)
	{
		//ByromDbg("[OnNotifyPress] List_Features");
	}
	else
	{
		//ByromDbg("[OnNotifyPress] Something was pressed!");
	}
	bHandled = TRUE;
	return S_OK;
}

HRESULT CMainScene::OnNotifyItemCheckChanged(HXUIOBJ hObjSource, XUINotifyItemCheckChanged* pNotifyItemCheckChangedData, BOOL& bHandled)
{
	// The new checked state of the list item specified by pNotifyItemCheckChangedData->iItem
	// is in pNotifyItemCheckChangedData->bChecked.

	if (bListPopulated)
	{
		if (hObjSource == List_Features)
		{
			//ByromDbg("[OnNotifyItemCheckChanged] Xam feature \"%ls\" is %s", FeatList[pNotifyItemCheckChangedData->iItem].Feature_Name, pNotifyItemCheckChangedData->bChecked ? "Enabled" : "Disabled");
			FeatList[pNotifyItemCheckChangedData->iItem].bNew_En_State = pNotifyItemCheckChangedData->bChecked;
		}
	}
	bHandled = TRUE;
	return S_OK;
}

VOID CMainScene::PopulateCheckList()
{
	GrabEnableStates();

	List_Features.DeleteItems(0, List_Features.GetItemCount()); // Clear the list

	for (int i = 0; i < ARRAYSIZE(FeatList); i++)
	{
		List_Features.InsertItems(i, 1);
		List_Features.SetText(i, FeatList[i].Feature_Name);
		List_Features.SetItemCheck(i, FeatList[i].bNew_En_State);
	}
	bListPopulated = TRUE;
}
#pragma endregion