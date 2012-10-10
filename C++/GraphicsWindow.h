#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "BaseWindow.h"

template <class T> void SafeRelease(T **ppT);

class GraphicsWindow : public BaseWindow<GraphicsWindow>
{
    ID2D1Factory            *pFactory;
    ID2D1HwndRenderTarget   *pRenderTarget; // Device Dependent
    ID2D1SolidColorBrush    *pBrush; // Device Dependent
	ID2D1SolidColorBrush    *pStrokeBrush; // Device Dependent
    D2D1_ELLIPSE            ellipse; // Device Independent

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
	void	GraphicsWindow::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
    void    OnPaint();
    void    Resize();

public:

    GraphicsWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
    {
    }

    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};