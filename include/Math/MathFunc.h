#pragma once

#include <assert.h>
#include <math.h>

#undef min
#undef max

static inline float clamp(float a, float b, float t)
{
    return (t < a) ? a : (t < b) ? t : b;
}

static inline int clamp(int a, int b, int t)
{
    return (t < a) ? a : (t < b) ? t : b;
}

template<class Foo>
Foo lerp(const Foo& a, const Foo& b, float t);

template<>
static inline float lerp(const float& a, const float& b, float t)
{
    return a * (1.f - t) + b * t;
}

static inline float absolute(float a)
{
    return a < 0 ? -a : a;
}

static inline float norm(float a, float b, float t)
{
    assert(b - a != 0);
    return (t - a) / (b - a);
}

template<class Foo>
static inline Foo square(const Foo& f)
{
    return f * f;
}

template<class Foo>
static inline Foo minimum(const Foo& a, const Foo& b)
{
    return a < b ? a : b;
}

template<class Foo>
static inline Foo maximum(const Foo& a, const Foo& b)
{
    return a < b ? b : a;
}

template<class Foo>
static inline Foo signum(const Foo& f) {return f < 0 ? Foo(-1) : f > 0 ? Foo(1) : Foo(0);}

#include <Math/Pi.h>

static inline float smoothstep(float a, float b, float t)
{
    return lerp(a, b, (sinf(lerp(-Pi/2, +Pi/2, t)) + 1) / 2);
}

template<class Foo>
static inline void swap(Foo& a, Foo& b)
{
    Foo x = a;
    a = b, b = x;
}
