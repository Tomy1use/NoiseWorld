#pragma once

#include <Math/Vector.h>

struct ParallelProjection
{
    int resX, resY;
    float width, height;
    ParallelProjection(int resX, int resY, float width, float height): resX(resX), resY(resY), width(width), height(height) {}
    void operator() (const Vector& A, int& x, int& y, float& w) const
    {
        x = int(A.x * resX / width) + resX/2;
        y = int(-A.y * resY / height) + resY/2;
        w = -A.z;
    }
    Vector operator() (int x, int y, float w) const
    {
        return Vector((x-resX/2) * width / resX, -(y-resY/2) * height / resY, -w);
    }
};
