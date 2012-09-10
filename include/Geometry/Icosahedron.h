#pragma once

#include <Math/Vector.h>

struct Icosahedron
{
    static const unsigned int PointCount = 12;
    static const unsigned int TrigonCount = 20;
    enum {A=0, B, C, D, E, F, G, H, I, J, U, V};
    Vector points[PointCount];
    Vector trigonPoint(unsigned int t, unsigned int p);
    int trigonPointIndex(unsigned int t, unsigned int p);
    Icosahedron();
};
