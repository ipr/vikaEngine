// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"

#include "vikaCode/vikaApp.h"

HINSTANCE g_hInst;                                // current instance
HWND g_hWnd;

bool createWindow(HINSTANCE hInstance, TCHAR *className, TCHAR *title);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

const char *pShortName = "vikaEngine";

int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInst = hInstance;
	if (createWindow(hInstance, _T("vikaEngine"), _T("vika")) == false)
	{
		return 0;
	}

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

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	app.destroy();
	return app.getResult();
}

bool createWindow(HINSTANCE hInstance, TCHAR *className, TCHAR *title)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = NULL;
	wcex.hCursor        = NULL;
	wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = className;
	wcex.hIconSm        = NULL;

	RegisterClassExW(&wcex);

	g_hWnd = CreateWindow(className, title, 
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
						nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return false;
	}

	::ShowWindow(g_hWnd, SW_SHOWNORMAL);
	::UpdateWindow(g_hWnd);
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		/*
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
		*/
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
