#include <windows.h>
#include <stdio.h>



void error(char* s, ...)
{
    char text[999];
    va_list args;
    va_start(args, s);
    vsprintf(text, s, args);
    va_end(args);
    MessageBox(NULL, text, "Error", MB_OK);
}

