#pragma once

#include <stdio.h>

namespace gl
{
    template<typename Proc>
    bool resolve(Proc& proc, const char* Name)
    {
        proc = Proc(wglGetProcAddress(Name));
        printf("%s %x\n", Name, int(proc));
        return !!proc;
        return false;
    }
}
