// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"

#include "vikaCode/vikaApp.h"

HINSTANCE g_hInst;                                // current instance
HWND g_hWnd;

bool createWindow(const TCHAR *className, const TCHAR *title);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

const char *pShortName = "vikaEngine";
const char *pEngineName = "vikaEngine";
const TCHAR *wClassName = _T("vikaEngine");
const TCHAR *wTitleName = _T("vika");

int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInst = hInstance;
	if (createWindow(wClassName, wTitleName) == false)
	{
		return 0;
	}

	LARGE_INTEGER liStartClock, liPerfFreq;
	::QueryPerformanceCounter(&liStartClock);
	::QueryPerformanceFrequency(&liPerfFreq);

	vikaApp app(pShortName, pEngineName);
	if (app.create() == false)
	{
		app.destroy();
		return app.getResult();
	}

	// do stuff
	app.enumeratePhysicalDevices();

	// creating surface is currently crashing, something uninitialized still?
	//app.createSurface(g_hInst, g_hWnd);

	app.createLogicalDevice();

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

bool createWindow(const TCHAR *className, const TCHAR *title)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = g_hInst;
	wcex.hIcon          = NULL;
    wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = className;
	wcex.hIconSm        = NULL;

	RegisterClassExW(&wcex);

	g_hWnd = CreateWindow(className, title, 
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, 
						CW_USEDEFAULT, 0, 
						nullptr, nullptr, 
						g_hInst, nullptr);

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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_ERASEBKGND:
		return -1;
		break;

	case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

		/* stuff that is usually needed for windowed mode
    case WM_SIZE:
	case WM_ENTERSIZEMOVE:
	case WM_EXITSIZEMOVE:
	case WM_MOVE:
		break;
		*/

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
