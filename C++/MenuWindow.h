#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#include "BaseWindow.h"
#endif
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "MainWindow.h"
#endif
#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H
#include "GraphicsWindow.h"
#endif
#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include "OpenGLWindow.h"
#endif

class MenuWindow : public BaseWindow<MenuWindow>
{
	HWND button;
	HWND clockButton;
	HWND openGLButton;

	MainWindow mainWindow;
	GraphicsWindow graphicsWindow;
	OpenGLWindow openGLWindow;

public:
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	MenuWindow() : button(NULL), clockButton(NULL), openGLButton(NULL) {}
};