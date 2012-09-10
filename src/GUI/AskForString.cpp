#include <stdio.h>
#include <windows.h>

static const int AnswerSize = MAX_PATH;
static char answer[AnswerSize];

namespace gui
{
	const char* AskForString(const char* Question, HWND parentWindow=NULL)
	{
		answer[0] = 0;
		int x = 100, y = 100;
		int w = 300, h = 50;
		if(parentWindow){
			RECT rect;
			GetWindowRect(parentWindow, &rect);
			x = (rect.left + rect.right) / 2;
			y = (rect.top + rect.bottom) / 2;
		}
		HWND window = CreateWindow("EDIT", Question, WS_VISIBLE, x, y, w, h, 
			parentWindow, NULL, NULL, NULL);
		if(! window){
			return NULL;
		}
		SendMessage(window, WM_SETTEXT, WPARAM(0), LPARAM(NULL));
		while(true){
			MSG msg;
			BOOL res = GetMessage(&msg, NULL, 0, 0);
			if(res == 0) continue;
			if(res == -1) {printf("GetMessage() returned -1. Don't know what to do.\n"); break;}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.hwnd == window){
				if(msg.message == WM_KEYDOWN){
					int key = int(msg.wParam);
					if(key == VK_RETURN){
						SendMessage(window, WM_GETTEXT, AnswerSize, LPARAM(answer));
						if(strlen(answer) > 0){
							printf("AskForString -> %s\n", answer);
							DestroyWindow(window);
							break;
						}
					}
				}
			}
		}
		return answer;
	}
}
