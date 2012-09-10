#pragma once

#include <windows.h>
#include <string.h>

struct Text
{
    const char* text;
    HFONT font;
    int x, y;
    Text(): text(NULL), font(NULL), x(0), y(0) {}
    ~Text() {}
    void setFont(const char* family, int size)
    {
        font = CreateFont(
            size, 0,0,0,0,0,0,0, RUSSIAN_CHARSET,
            0,0, DEFAULT_QUALITY, 0, family);
        if(! font){
            error("Failed create font %s, %d", family, size);
        }
    }
    void setText(const char* value) {text = value;}
    void setPoint(int X, int Y) {x = X, y = Y;}
    void draw(HDC device)
    {
        if(text){
            SelectObject(device, font);
            RECT textRect = {0, 0, 0, 0};
            DrawText(device, text, strlen(text), &textRect, DT_CALCRECT);
            int textWidth = textRect.right - textRect.left;
            int textHeight = textRect.bottom - textRect.top;
            RECT outRect = {x, y, x+textWidth, y+textHeight};
            DrawText(device, text, strlen(text), &outRect, 0);
        }
    }
};
