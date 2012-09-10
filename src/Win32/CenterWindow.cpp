#include <windows.h>

void centerWindow(HWND window, int width, int height)
{
	RECT fullRect, clientRect;
	GetWindowRect(window, &fullRect);
	GetClientRect(window, &clientRect);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int fullWidth = fullRect.right - fullRect.left;
	int fullHeight = fullRect.bottom - fullRect.top;
	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom - clientRect.top;
	int borderWidth = fullWidth - clientWidth;
	int borderHeight = fullHeight - clientHeight;
	fullWidth = width + borderWidth;
	fullHeight = height + borderHeight;
	int left = (screenWidth - fullWidth) / 2;
	int top = (screenHeight - fullHeight) / 2;
	MoveWindow(window, left, top, fullWidth, fullHeight, true);
}
