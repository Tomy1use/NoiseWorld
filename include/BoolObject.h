#pragma once

template<class T>
struct BoolObject
{
    bool yes;
    T object;
    BoolObject(const T& o): yes(true), object(o) {}
    BoolObject(bool yes): yes(yes) {}
    operator bool() {return yes;}
    operator const T& () {return object;}
};
