#pragma once

#include <Math/Vector.h>

struct Plane
{
	Vector n;
	float w;
	Plane() {}
	Plane(const Vector& N, float W): n(N), w(W) {}
	Plane operator -() {return Plane(-n, -w);}
};

