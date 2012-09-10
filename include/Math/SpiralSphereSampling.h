#pragma once

#include <math.h>
#include <Math/Pi.h>

struct SpiralSphereSampling
{
    float radius, step;
    float a, u;
    SpiralSphereSampling(float Radius, float Step): radius(Radius), step(Step), a(0), u(0) {}
    operator bool() {return (u < Pi/2);}
    float x() {return cosf(u) * cosf(a) * radius;}
    float z() {return cosf(u) * sinf(a) * radius;}
    float y() {return sinf(u) * radius;}
    void operator ++()
    {
        assert(u < Pi/2);
        if(u < Pi/2){
            float da = step / (radius * cosf(u));
            float du = da * step / (radius * 2 * Pi);
            a += da;
            u += du;
        }
    }
};
