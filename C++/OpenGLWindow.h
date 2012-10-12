#include <Windows.h>
#include <GL/gl.h>

#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#include "BaseWindow.h"
#endif

#ifndef DRAWINGMODE_H
#define DRAWINGMODE_H
#include "DrawingMode.h"
#endif

class OpenGLWindow : public BaseWindow<OpenGLWindow>
{
	void OnPaint();
	void OnFrame();

	HGLRC SetUpOpenGL(HWND hwnd);
	void DrawOpenGLScene(void);

	HDC hDC;
    HGLRC hRC; // Note this is STATIC!
    GLdouble gldAspect;
    GLsizei glnWidth, glnHeight;
	UINT_PTR m_nTimerID;
	bool animate;
	float rotation;
	bool render3D;

public:

    OpenGLWindow() : render3D(false)
    {
    }

    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};