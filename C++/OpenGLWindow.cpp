#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "OpenGLWindow.h"

LRESULT OpenGLWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_CREATE:
		// Create an appropriate render context
        hRC = SetUpOpenGL(m_hwnd);
        return 0;

    case WM_SIZE:
        hDC = GetDC(m_hwnd); // Get device context
        wglMakeCurrent(hDC,hRC);

		// get the new size of the client window
        // note that we size according to the height,
        // not the smaller of the height or width.
        glnWidth = (GLsizei) LOWORD(lParam);
        glnHeight = (GLsizei) HIWORD(lParam);
        gldAspect = (GLdouble) glnWidth/(GLdouble)glnHeight;

        // set up a projection matrix to fill the client window
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity(); // clear matrix

        // a perspective-view matrix...
        gluPerspective(
            30.0,   // Field-of-view angle
            gldAspect, // Aspect ratio of view volume
            1.0,   // Distance to near clipping plane
            10.0 ); // Distance to far clipping plane

		// Set the viewport size
        glViewport(0 /* x */, 0 /* y */, glnWidth, glnHeight );
		
		// Deselect the RC. If using more than oen call 'wglGetCurrentContext' and 'wglGetCurrentDC()' beforehand to reinstate after
        wglMakeCurrent( NULL, NULL );

        ReleaseDC(m_hwnd, hDC);

        return 0;

    case WM_PAINT:
			OnPaint();
            return 0;

    case WM_DESTROY:
            // Clean up and terminate.
            wglDeleteContext( hRC );
            PostQuitMessage( 0 );
            return 0;

    }

    return DefWindowProc( m_hwnd, uMsg, wParam, lParam );
}

void OpenGLWindow::OnPaint()
{
	PAINTSTRUCT ps;
    hDC = BeginPaint(m_hwnd, &ps);

	// Make the render context current and associate with the DC (in case of having multiple openGL render contexts running)
    wglMakeCurrent(hDC, hRC);

    DrawOpenGLScene();

	// Copy from off-screen buffer
	SwapBuffers(hDC);

    // Deselect the RC. If using more than oen call 'wglGetCurrentContext' and 'wglGetCurrentDC()' beforehand to reinstate after
    wglMakeCurrent( NULL, NULL );

    EndPaint(m_hwnd, &ps);
}

// Select a pixel format and then create a rendering context from it.
HGLRC OpenGLWindow::SetUpOpenGL( HWND hWnd )
{
    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR), // strcut size 
        1,                              // Version number
        PFD_DRAW_TO_WINDOW |    // Flags, draw to a window,
            PFD_SUPPORT_OPENGL
			| PFD_DOUBLEBUFFER, // use OpenGL
        PFD_TYPE_RGBA,          // RGBA pixel values
        24,                     // 24-bit color
        0, 0, 0,                // RGB bits & shift sizes.
        0, 0, 0,                // 
        0, 0,                   // No alpha buffer info
        0, 0, 0, 0, 0,          // No accumulation buffer
        32,                     // 32-bit depth buffer
        0,                      // No stencil buffer
        0,                      // No auxiliary buffers
        PFD_MAIN_PLANE,         // Layer type
        0,                      // Reserved (must be 0)
        0,                      // No layer mask
        0,                      // No visible mask
        0                       // No damage mask
    };

    int nMyPixelFormatID;
    HDC hDC;
    HGLRC hRC;

    hDC = GetDC( hWnd );
    nMyPixelFormatID = ChoosePixelFormat( hDC, &pfd );

    SetPixelFormat( hDC, nMyPixelFormatID, &pfd );

    hRC = wglCreateContext( hDC );
    ReleaseDC( hWnd, hDC );

    return hRC;
}

void OpenGLWindow::DrawOpenGLScene( )
{    
    // Define the modelview transformation.
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity(); // clear our matrix before we perform the translate to view the scene (otherwise the translate would multiply)

    // Move the viewpoint out to where we can see everything
    glTranslatef( 0.0f, 0.0f, -5.0f );

    glBegin(GL_TRIANGLES /* Treats each triplet of vertices as an independent triangle */);
        glColor3f(1.0,0.0,0.0);//red color for the triangle
        glVertex3f(0.0,0.0,0);
        glVertex3f(0.0,1.0,0);
        glVertex3f(1.0,0.0,0);

        glColor3f(0.0,1.0,0.0);//Green color for the triangle
        glVertex3f(0.0,0.0,0);
        glVertex3f(0.0,1.0,0);
        glVertex3f(-1.0,0.0,0);

        glColor3f(0.0,0.0,1.0);//Blue color for the triangle
        glVertex3f(1.0,0.0,0);
        glVertex3f(0.0,-1.0,0);
        glVertex3f(-1.0,0.0,0);
    glEnd();

    glFlush();
}