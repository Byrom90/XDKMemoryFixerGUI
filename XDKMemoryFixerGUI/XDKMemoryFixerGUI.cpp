#include "stdafx.h"
#include "FontFile.h"

HXUIOBJ hMainScene;

HRESULT RenderGame(IDirect3DDevice9* pDevice)
{
	// Render game graphics.
	pDevice->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 32, 32, 64),
		1.0,
		0);

	return S_OK;
}

HRESULT InitD3D(IDirect3DDevice9** ppDevice,
	D3DPRESENT_PARAMETERS* pd3dPP)
{
	IDirect3D9* pD3D;

	pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// Set up the presentation parameters.
	ZeroMemory(pd3dPP, sizeof(D3DPRESENT_PARAMETERS));
	pd3dPP->BackBufferWidth = 1280;
	pd3dPP->BackBufferHeight = 720;
	pd3dPP->BackBufferFormat = D3DFMT_A8R8G8B8;
	pd3dPP->BackBufferCount = 1;
	pd3dPP->MultiSampleType = D3DMULTISAMPLE_NONE;
	pd3dPP->SwapEffect = D3DSWAPEFFECT_DISCARD;
	pd3dPP->EnableAutoDepthStencil = TRUE;
	pd3dPP->AutoDepthStencilFormat = D3DFMT_D24S8;
	pd3dPP->PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// Create the device.
	return pD3D->CreateDevice(
		0,
		D3DDEVTYPE_HAL,
		NULL,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		pd3dPP,
		ppDevice);
}


BOOL bShouldExit = FALSE;
// Entry point of the title.
int __cdecl main()
{
	ByromDbg("App init...");

	IsDevkit = *(DWORD*)0x8E038610 & 0x8000 ? FALSE : TRUE;
	ByromDbg("IsDevkit: %s", IsDevkit ? "TRUE" : "FALSE");

	//if (IsDevkit)
	//	GrabEnableStates();

	hAPP = GetModuleHandle(NULL);

	IDirect3DDevice9* pDevice;
	D3DPRESENT_PARAMETERS d3dpp;
	HRESULT hr;

	// Initialize D3D
	hr = InitD3D(&pDevice, &d3dpp);
	if (FAILED(hr)) {
		ByromDbg("InitD3D failed!");
	}

	// Declare an instance of the XUI framework.
	CMyApp app;

	// Initialize the application.
	hr = app.InitShared(pDevice, &d3dpp, XuiD3DXTextureLoader);
	if (FAILED(hr))
	{
		ByromDbg("InitShared failed!");
		return 1;
	}

	// Register a default typeface
	app.RegisterDefaultTypeface(L"Arial Unicode MS", LinkWChar(L"memory://%X,%X", FONT, sizeof(FONT)));


	if (!app.LoadSkin(LinkWChar(L"section://%08X,XDKMF#0_skin.xur", hAPP)))
	{
		if (app.LoadFirstScene(L"", LinkWChar(L"section://%08X,XDKMF#1_scene.xur", hAPP), /*NULL*/hMainScene))
		{
			ByromDbg("Failed to load scene!");
			XNotify(L"Failed to load scene!");
			bShouldExit = TRUE;
		}
	}
	else
	{
		ByromDbg("Failed to load skin!");
		XNotify(L"Failed to load skin!");
		bShouldExit = TRUE;
	}

	while (!bShouldExit) {
		// Render game graphics.
		RenderGame(pDevice);

		// Update XUI
		app.RunFrame();

		// Render XUI
		hr = app.Render();

		// Update XUI Timers
		hr = XuiTimersRun();

		// Present the frame.
		pDevice->Present(NULL, NULL, NULL, NULL);

	}
	// Free resources, unregister custom classes, and exit.
	ByromDbg("Exiting...");
	app.Uninit();
	pDevice->Release();
}


