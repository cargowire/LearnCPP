#include <windows.h>
#include <memory>

#include "GraphicsWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance /* The current application instance handle */,
	HINSTANCE, /* Previous window - no longer any meaning so ignored */
	PWSTR pCmdLine /* Command line arguments as unicode string.  Accessible via GetCommandLine or CommandLineToArgvW */,
	int nCmdShow /* whether app window will be minimised, maximised or shown normally */)
{
    GraphicsWindow win;

    if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}