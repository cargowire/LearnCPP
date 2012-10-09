#include "MainWindow.h"

// Start with black as the colour
MainWindow::MainWindow() : red(0), green(0), blue(0) { }

// Override the virtual class name for use in creation
PCWSTR  MainWindow::ClassName() const { return L"Sample Window Class"; }

// Handle messages from the window (except for create which is handled by base)
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
    {
		case WM_DESTROY:
			PostQuitMessage(0); // Add message to the queue to quit the application (return a 0 response from GetMessage)
			return 0;

		case WM_SIZE: // Example of using the size event.  lParam contains 2 16bit numbers packed into one 32
			{
				int width = LOWORD(lParam);  // Macro to get the low-order word.
				int height = HIWORD(lParam); // Macro to get the high-order word.

				// Respond to the message:
				OnSize(m_hwnd, (UINT)wParam, width, height);
			}
        break;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_hwnd, &ps);

				// GDI function
				FillRect(hdc, &ps.rcPaint /* Current update region */, CreateSolidBrush(RGB(red, green, blue)));

				EndPaint(m_hwnd, &ps);
			}
        return 0;

    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam); // Do the default action
}

// Change our colour (just to see a visual difference on screen)
void MainWindow::OnSize(HWND hwnd, UINT flag, int width, int height)
{
	if(red < 255)
		red++;
	else
		red = 0;
			
	if(green < 255)
		green++;
	else
		green = 0;

	if(blue < 255)
		blue++;
	else
		blue = 0;
}