#include <windows.h>

struct StateInfo {
    int r;
	int g;
	int b;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnSize(HWND hwnd, UINT flag, int width, int height);
inline StateInfo* GetAppState(HWND hwnd);