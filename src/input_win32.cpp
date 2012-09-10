#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "input.h"
#include "input_win32.h"


void Input::
processMessage(HWND window, UINT message, WPARAM shortParam, LPARAM longParam)
{
	switch(message){
		case WM_KEYDOWN:
			{
				unsigned scanCode = ((unsigned)longParam >> 16) & 0xff;
				if((longParam >> 24) & 1){
					scanCode += 128;
				}
				Keys::down.keys[scanCode] = (char)0xff;
			}
			break;
		case WM_KEYUP:
			{
				unsigned scanCode = ((unsigned)longParam >> 16) & 0xff;
				if((longParam >> 24) & 1){
					scanCode += 128;
				}
				Keys::down.keys[scanCode] = (char)0x00;
			}
			break;
		case WM_MOUSEMOVE:
			Mouse::movX = GET_X_LPARAM(longParam) - Mouse::x;
			Mouse::movY = GET_Y_LPARAM(longParam) - Mouse::y;
			Mouse::x += Mouse::movX;
			Mouse::y += Mouse::movY;
			break;
		case WM_LBUTTONUP:
			Mouse::leftDown = false;
			break;
		case WM_LBUTTONDOWN:
			Mouse::leftDown = true;
			break;
		case WM_RBUTTONUP:
			Mouse::rightDown = false;
			break;
		case WM_RBUTTONDOWN:
			Mouse::rightDown = true;
			break;
	}
}
