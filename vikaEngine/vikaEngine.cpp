#include "stdafx.h"

#include "vikaCode/vikaApp.h"

HINSTANCE g_hInst;                                // current instance

const char *pShortName = "vikaEngine";

int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInst = hInstance;

	vikaApp app(pShortName);
	//if (app.getResult() ...

	return app.getResult();
}

