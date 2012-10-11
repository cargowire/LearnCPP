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
	D2D1_POINT_2F			ptMouse;

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
	void	GraphicsWindow::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
    void    OnPaint();
	void    OnLButtonDown(int pixelX, int pixelY, DWORD flags);
    void    OnLButtonUp();
    void    OnMouseMove(int pixelX, int pixelY, DWORD flags);
    void    Resize();

public:

    GraphicsWindow() :  pFactory(NULL), pRenderTarget(NULL), pBrush(NULL),
        ellipse(D2D1::Ellipse(D2D1::Point2F(), 0, 0)),
        ptMouse(D2D1::Point2F())
    {
    }

    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};