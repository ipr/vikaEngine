// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"

#include "vikaCode/vikaApp.h"

HINSTANCE g_hInst;                                // current instance
HWND g_hWnd;

const char *pShortName = "vikaEngine";

int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInst = hInstance;

	vikaApp app(pShortName);
	if (app.create() == false)
	{
		app.destroy();
		return app.getResult();
	}

	// do stuff
	app.enumeratePhysicalDevices();
	app.getQueueProperties();
	app.prepareLogicalDevice();

	//app.createSurface(g_hInst, g_hWnd);

	/*
    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	*/

	app.destroy();
	return app.getResult();
}

