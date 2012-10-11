#include <windows.h>
#include <memory>

#include "resource.h"
#include "OpenGLWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance /* The current application instance handle */,
	HINSTANCE, /* Previous window - no longer any meaning so ignored */
	PWSTR pCmdLine /* Command line arguments as unicode string.  Accessible via GetCommandLine or CommandLineToArgvW */,
	int nCmdShow /* whether app window will be minimised, maximised or shown normally */)
{
    OpenGLWindow win;

    if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

	// Load Accelerators
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
		// Attempt to translate an accelerator and action it.
		if (!TranslateAccelerator(win.Window(), hAccel, &msg))
        {
			// Failure to translate an accelerator = zero so continue normal translate/dispatch flow
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }

    return 0;
}