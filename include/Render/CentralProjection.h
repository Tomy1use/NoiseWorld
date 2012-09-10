#pragma once

#include <Math/Vector.h>

struct CentralProjection
{
    int resX, resY;
    float focalDistance;
    float aspect;
    CentralProjection(int resX, int resY, float focalDistance): resX(resX), resY(resY), focalDistance(focalDistance), aspect(float(resX)/float(resY)) {}
    void operator() (const Vector& a, int& x, int& y, float& w) const
    {
        w = -focalDistance / a.z;
        float X = + a.x * w;
        float Y = - a.y * w;
        x = int((X / aspect + 1.f) * resX / 2);
        y = int((Y + 1.f) * resY / 2);
    }
    Vector operator() (int x, int y, float w) const
    {
        float Z = -focalDistance / w;
        float Y = -(float(y) * 2 / resY - 1) / w;
        float X = +(float(x) * 2 / resX - 1) * aspect / w;
        return Vector(X, Y, Z);
    }
};
