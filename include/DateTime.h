#pragma once

#include <stdio.h>

struct DateTime
{
	static const int TextSize = 32;
    char shortText[TextSize];
    char longText[TextSize];
    DateTime()
    {
        SYSTEMTIME time;
        GetLocalTime(&time);
        sprintf_s(longText, TextSize, "%04d%02d%02d%02d%02d-%02d%03d",
            time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
            time.wSecond, time.wMilliseconds);
        sprintf_s(shortText, TextSize, "%04d%02d%02d%02d%02d",
            time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
    }
};
