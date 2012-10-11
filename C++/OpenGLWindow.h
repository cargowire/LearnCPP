#include <Windows.h>
#include <GL/gl.h>

#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#include "BaseWindow.h"
#endif

class OpenGLWindow : public BaseWindow<OpenGLWindow>
{
	void OnPaint();

	HGLRC SetUpOpenGL(HWND hwnd);
	void DrawOpenGLScene(void);

	HDC hDC;
    HGLRC hRC; // Note this is STATIC!
    GLdouble gldAspect;
    GLsizei glnWidth, glnHeight;

public:

    OpenGLWindow()
    {
    }

    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};