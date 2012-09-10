#pragma once

#include <math.h>
#include <Math/Pi.h>

struct SpiralCircleSampling
{
    float step;
    float a, r;
    SpiralCircleSampling(float Radius, float Step): step(Step), a(0), r(Radius) {}
    float x() {return cosf(a) * r;}
    float z() {return sinf(a) * r;}
    operator bool() {return (r > 0);}
    void operator ++()
    {
        assert(r > 0);
        if(r > 0){
            float da = step / r;
            float dr = da * step / (2 * Pi);
            a += da;
            r -= dr;
        }
    }
};
