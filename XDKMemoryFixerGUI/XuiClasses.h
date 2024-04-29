#ifndef __XuiClasses_H__
#define __XuiClasses_H__
#pragma once

class CMyApp : public CXuiModule
{
protected:
	// Override so that CMyApp can register classes.
	virtual HRESULT RegisterXuiClasses();

	// Override so that CMyApp can unregister classes. 
	virtual HRESULT UnregisterXuiClasses();
};


//==============================================================================================================================================================================
//
//			CLASS TO HANDLE MainScene
//
//==============================================================================================================================================================================
#pragma region MainScene
class CMainScene : public CXuiSceneImpl
{
	//private:
public:
	XUI_IMPLEMENT_CLASS(CMainScene, L"MainScene", XUI_CLASS_SCENE)
protected:
	CXuiTextElement textNotXDK, textBottomInfo, textVersion;
	CXuiControl btnBACK_Exit, btnX_Save, btnA_Toggle;
	CXuiList List_Features;

	// Handler for the XM_INIT message.
	HRESULT OnInit(XUIMessageInit* pInitData, BOOL& bHandled);
	HRESULT OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled);
	HRESULT OnNotifyItemCheckChanged(HXUIOBJ hObjSource, XUINotifyItemCheckChanged* pNotifyItemCheckChangedData, BOOL& bHandled);
	VOID PopulateCheckList();

	// Message map.
	XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT(OnInit)
		XUI_ON_XM_NOTIFY_PRESS(OnNotifyPress)
		XUI_ON_XM_NOTIFY_ITEM_CHECKCHANGED(OnNotifyItemCheckChanged)
		XUI_END_MSG_MAP()

};
#pragma endregion

#endif