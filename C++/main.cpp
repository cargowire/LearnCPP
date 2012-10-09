#include <windows.h>
#include <memory>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnSize(HWND hwnd, UINT flag, int width, int height);

struct StateInfo {
    int r;
	int g;
	int b;
};

int WINAPI wWinMain(HINSTANCE hInstance /* The current application instance handle */,
	HINSTANCE, /* Previous window - no longer any meaning so ignored */
	PWSTR pCmdLine /* Command line arguments as unicode string.  Accessible via GetCommandLine or CommandLineToArgvW */,
	int nCmdShow /* whether app window will be minimised, maximised or shown normally */)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class"; // Wide character array.  L"" for wide string literal
    
    WNDCLASS wc = { }; 

    wc.lpfnWndProc   = WindowProc; // Window Proc handles window events
    wc.hInstance     = hInstance; // The identifier of this application instance
    wc.lpszClassName = CLASS_NAME; // Name of this window class (unique within process, must not clash with built in 'Button' etc

    RegisterClass(&wc); // Register this class for use

	// Create state to pass through to the window
	StateInfo *pState = new (std::nothrow) StateInfo;
	pState->r = 0;
	pState->g = 0;
	pState->b = 0;

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                                  // Optional window styles e.g. transparent etc
        CLASS_NAME,                         // Using our class as defined above
        L"Learn to Program for Windows",    // Wide character string literal for window text (used by default windows as title)
        WS_OVERLAPPEDWINDOW,                // Window style - overlappedwindow bitwise OR combines title bar, sys menu, min/max

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // Default size and position values

        NULL,       // Parent window if this were to be a child   
        NULL,       // Menu
        hInstance,  // Application instance handle
        pState        // Any pointer you want.  When WM_NCCREATE or WM_CREATE are called it can be extracted
        );

	// Handle failure condition - kill app
    if (hwnd == NULL)
    {
        return 0;
    }

	// Make window visible
    ShowWindow(hwnd, nCmdShow /* Window should be minimised or not according to OS */);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg /*Information will be filled in by GetMessage*/, NULL /*filters*/, 0 /*filters*/, 0 /*filters*/)) /* Blocks until next message is returned */
    {
        TranslateMessage(&msg); // Translates message such as into a char from a keyboard press
        DispatchMessage(&msg); // Call proc that is the target of this message (calls window handle)
    }

    return 0;
}

/* CALLBACK is the calling convention.  LRESULTcontains the response to the particular message (context specific) */
LRESULT CALLBACK WindowProc(HWND hwnd /* Handle to the window */,
	UINT uMsg /* Message code e.g. type of message */,
	WPARAM wParam /* First parameter - meaning depends on message code */,
	LPARAM lParam /* Second parameter - meaning depends on message code */)
{
    switch (uMsg)
    {
	case WM_CREATE:
		{
			// When you receive the WM_NCCREATE and WM_CREATE messages, the lParam parameter of each message is a pointer to a
			// CREATESTRUCT structure. The CREATESTRUCT structure, in turn, contains the pointer that you passed into CreateWindowEx.
			CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			StateInfo *pState = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);

			// Store it on the window
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0); // Add message to the queue to quit the application (return a 0 response from GetMessage)
        return 0;

	 case WM_SIZE: // Example of using the size event.  lParam contains 2 16bit numbers packed into one 32
        {
            int width = LOWORD(lParam);  // Macro to get the low-order word.
            int height = HIWORD(lParam); // Macro to get the high-order word.

            // Respond to the message:
            OnSize(hwnd, (UINT)wParam, width, height);
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

			// Get State
			LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
			StateInfo *pState = reinterpret_cast<StateInfo*>(ptr);

			// GDI function
            FillRect(hdc, &ps.rcPaint /* Current update region */, CreateSolidBrush(RGB(pState->r, pState->g, pState->b)));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam); // Do the default action
}

void OnSize(HWND hwnd, UINT flag, int width, int height)
{
    // Change colours on resize (just for something to do using application state)
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	StateInfo *pState = reinterpret_cast<StateInfo*>(ptr);

	if(pState->r < 255)
		pState->r++;
	else
		pState->r = 0;
			
	if(pState->g < 255)
		pState->g++;
	else
		pState->g = 0;

	if(pState->b < 255)
		pState->b++;
	else
		pState->b = 0;
}
 