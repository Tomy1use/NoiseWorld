#pragma once

#include <Math/Vector.h>

struct Axes
{
	Vector x, y, z;
	
	Axes() {}
	Axes(Vector X, Vector Y, Vector Z): x(X), y(Y), z(Z) {}
	Axes(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz):
		x(xx, xy, xz), y(yx, yy, yz), z(zx, zy, zz) {}
	
	Vector out(const Vector& a) const;
	Vector in(const Vector& a) const;
	Axes in(const Axes& a) const {return Axes(in(a.x), in(a.y), in(a.z));}
	Axes out(const Axes& a) const {return Axes(out(a.x), out(a.y), out(a.z));}
	
	Axes operator - (const Axes& a) const {return Axes(x-a.x, y-a.y, z-a.z);}
    void operator *= (float f) {x *= f, y *= f, z *= f;}
    Axes operator * (float f) const {return Axes(x * f, y * f, z * f);}
	Axes operator * (const Vector& v) const {return Axes(x * v.x, y * v.y, z * v.z);}
};
