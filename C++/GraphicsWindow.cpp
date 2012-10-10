#include <windows.h>
#include <d2d1.h> // Contains helper functions/classes such as ColorF, Matrix3x2F for transforms and initializers for Direct2D structures

#include "GraphicsWindow.h"

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// Recalculate drawing layout when the size of the window changes.
void GraphicsWindow::CalculateLayout()
{
    if (pRenderTarget != NULL)
    {
        D2D1_SIZE_F size = pRenderTarget->GetSize(); // Size returned in DIP not pixels
        const float x = size.width / 2;
        const float y = size.height / 2;
        const float radius = min(x, y);
		// Update the ellipse to match the new render target size
        ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
    }
}

HRESULT GraphicsWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;

	// Only create once (due to expense) at the beginning and then if resources are discarded due to unavailabilty of device
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc); // Returned in pixels not Device Independent Pixels (DIPs)

        D2D1_SIZE_U size = D2D1::SizeU(rc.right /* Width */, rc.bottom /* Height */);

		// Get the render target for this window
        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(), // Defaultoptions
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush); // Setup the member brush for usage

			if (SUCCEEDED(hr))
            {
				const D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0);
				hr = pRenderTarget->CreateSolidColorBrush(color, &pStrokeBrush); // Setup the member brush for stroke usage

				if (SUCCEEDED(hr))
				{
					CalculateLayout();
				}
			}
        }
    }
    return hr;
}

void GraphicsWindow::DiscardGraphicsResources()
{
	// Discard the target and the brush (as the brush is target dependent)
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
	SafeRelease(&pStrokeBrush);
}

void GraphicsWindow::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth)
{
    pRenderTarget->SetTransform(
        D2D1::Matrix3x2F::Rotation(fAngle, ellipse.point)
            );

    // endPoint defines one end of the hand.
    D2D_POINT_2F endPoint = D2D1::Point2F(
        ellipse.point.x,
        ellipse.point.y - (ellipse.radiusY * fHandLength)
        );

    // Draw a line from the center of the ellipse to endPoint.
    pRenderTarget->DrawLine(
        ellipse.point, endPoint, pStrokeBrush, fStrokeWidth);
}


void GraphicsWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::SkyBlue) ); // Clear the device to blue
        pRenderTarget->FillEllipse(ellipse, pBrush); // Draw a circle using the members ellipse (that is adjusted by resize)

		// Draw hands
		SYSTEMTIME time;
		GetLocalTime(&time);

		// 60 minutes = 30 degrees, 1 minute = 0.5 degree
		const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f); 
		const float fMinuteAngle =(360.0f / 60) * (time.wMinute);
		const float fSecondAngle = (360.0f / 60) * (time.wSecond);

		DrawClockHand(0.6f,  fHourAngle,   6);
		DrawClockHand(0.85f, fMinuteAngle, 4);
		DrawClockHand(0.85f, fSecondAngle, 2);

		// Restore the identity transformation (as the transform is at this level not per scene item
		pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );

        hr = pRenderTarget->EndDraw(); // If an error occurs during any of the draw commands (begin, clear, fillellipse) it is returned here
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
			// If we failed for some reason, including if the device has become unavailable and we need to recreate then discard current resources
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void GraphicsWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size); // Update the render target to match the window
        CalculateLayout(); // Update the items within this window now that we've resized the render target
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

LRESULT GraphicsWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
		// Should be created before the first WM_PAINT message
        if (FAILED(D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaint();
        return 0;


    case WM_SIZE:
        Resize();
        return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}