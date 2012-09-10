#pragma once

#include <windows.h>

namespace gl
{
    struct RenderContext
    {
        HWND window;
        HDC deviceContext;
        HGLRC renderContext;
        HGLRC createRenderContext(HDC device)
        {
            PIXELFORMATDESCRIPTOR pfd;
            ZeroMemory( &pfd, sizeof( pfd ) );
            pfd.nSize = sizeof( pfd );
            pfd.nVersion = 1;
            pfd.dwFlags = 
                PFD_DRAW_TO_WINDOW | 
                PFD_SUPPORT_OPENGL |
                PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 24;
            pfd.cDepthBits = 16;
            pfd.iLayerType = PFD_MAIN_PLANE;
            int format = ChoosePixelFormat(device, &pfd);
            SetPixelFormat(device, format, &pfd);
            return wglCreateContext(device);
        }
        RenderContext(HWND window)
            : window(window)
            , deviceContext(GetDC(window))
            , renderContext(createRenderContext(deviceContext))
        {
            if(renderContext){
                wglMakeCurrent(deviceContext, renderContext);
            }
        }
        ~RenderContext()
        {
            if(renderContext){
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(renderContext);
            }
            ReleaseDC(window, deviceContext);
        }
    };
}
