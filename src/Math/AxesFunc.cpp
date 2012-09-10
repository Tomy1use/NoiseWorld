#include <Math/Axes.h>
#include <Math/VectorFunc.h>
#include <Math/VectorConst.h>

Axes rotateX(const Axes& a, float f)
{
    return Axes(rotateX(a.x, f), rotateX(a.y, f), rotateX(a.z, f));
}

Axes rotateY(const Axes& a, float f)
{
    return Axes(rotateY(a.x, f), rotateY(a.y, f), rotateY(a.z, f));
}

Axes rotateZ(const Axes& a, float f)
{
    return Axes(rotateZ(a.x, f), rotateZ(a.y, f), rotateZ(a.z, f));
}

Axes axesFromAxisZ(const Vector& z)
{
    int i = maxDim(z);
    Vector u = (i == 0) ? UnitVectorY : (i == 1) ? UnitVectorZ : UnitVectorX;
	Vector x = unit(cross(u, z));
	Vector y = unit(cross(z, x));
    return Axes(x, y, z);
}
