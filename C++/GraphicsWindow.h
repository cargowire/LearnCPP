#include <windows.h>
#pragma comment(lib, "d2d1")
#include <list> // For standard library list
#include <memory> // For shared_ptr

#include "BaseWindow.h"
#include "DrawableEllipse.h"
#include "DrawingMode.h"

template <class T> void SafeRelease(T **ppT);

class GraphicsWindow : public BaseWindow<GraphicsWindow>
{
	ID2D1Factory            *pFactory;
    ID2D1HwndRenderTarget   *pRenderTarget; // Device Dependent
    ID2D1SolidColorBrush    *pBrush; // Device Dependent
	D2D1_POINT_2F			ptMouse;
	HCURSOR                 hCursor;
	size_t                  nextColor;
	Mode					mode;
	std::list<std::shared_ptr<DrawableEllipse>> ellipses; // List of ellipses to draw
	std::list<std::shared_ptr<DrawableEllipse>>::iterator   selection; // Currently selected ellipsis (for movement)

    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
	void GraphicsWindow::DrawClockHand(std::shared_ptr<DrawableEllipse> clockface, float fHandLength, float fAngle, float fStrokeWidth);
    void    OnPaint();
	void    OnLButtonDown(int pixelX, int pixelY, DWORD flags);
    void    OnLButtonUp();
    void    OnMouseMove(int pixelX, int pixelY, DWORD flags);
    void    Resize();
	void GraphicsWindow::SetMode(Mode m);
	std::shared_ptr<DrawableEllipse> Selection();
	void MoveSelectionUp();
	void MoveSelectionDown();
	void    ClearSelection();

	HRESULT InsertEllipse(float x, float y);
	BOOL HitTest(float x, float y);

public:

    GraphicsWindow() :  pFactory(NULL), pRenderTarget(NULL), pBrush(NULL),
        ptMouse(D2D1::Point2F()),
		nextColor(0),
		selection(ellipses.end())
    {
    }

    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};