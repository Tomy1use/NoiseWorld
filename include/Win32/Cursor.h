#pragma once

namespace win32
{
    struct Cursor
    {
        POINT point;
        Cursor()
        {
            GetCursorPos(&point);
        }
    };
}
