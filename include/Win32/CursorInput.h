#pragma once

#include <windows.h>

struct CursorInput
{
    int x, y, deltaX, deltaY, rightPressX, rightPressY;
    int wheelDelta;
    bool leftButtonDown, rightButtonDown;
    bool rightButtonPress, leftButtonPress;
    CursorInput()
        : x(0), y(0), wheelDelta(0)
        , leftButtonDown(false), rightButtonDown(false)
        , rightButtonPress(false), leftButtonPress(false)
    {
    }
    void beginFrame()
    {
        wheelDelta = 0;
        deltaX = 0;
        deltaY = 0;
        rightButtonPress = false;
        leftButtonPress = false;
    }
    void message(const MSG& m)
    {
        if(m.message == WM_MOUSEMOVE){
            deltaX = GET_X_LPARAM(m.lParam) - x;
            deltaY = GET_Y_LPARAM(m.lParam) - y;
            x = GET_X_LPARAM(m.lParam);
            y = GET_Y_LPARAM(m.lParam);
        }
        if(m.message == WM_MOUSEWHEEL){
            wheelDelta = int(m.wParam);
        }
        if(m.message == WM_LBUTTONDOWN){
            leftButtonDown = true;
            leftButtonPress = true;
            deltaX = deltaY = 0;
			x = GET_X_LPARAM(m.lParam);
            y = GET_Y_LPARAM(m.lParam);
        }
        if(m.message == WM_LBUTTONUP){
            leftButtonDown = false;
        }
        if(m.message == WM_RBUTTONDOWN){
            rightPressX = GET_X_LPARAM(m.lParam);
            rightPressY = GET_Y_LPARAM(m.lParam);
            rightButtonDown = true;
            rightButtonPress = true;
            deltaX = deltaY = 0;
        }
        if(m.message == WM_RBUTTONUP){
            rightButtonDown = false;
        }
    }
};
