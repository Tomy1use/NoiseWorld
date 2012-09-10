#pragma once

struct CubicSpline
{
	Vector A, B, C, D;
	void CubicSpline(const Vector& p1, const Vector& v1, const Vector& p2, const Vector& v2)
	{
		A = p1;
		B = v1;
		C = v1 - v2 + (p2 - p1 - v1) * 3;
		D = p2 - p1 - v1 - C;
	}
	Vector point(const Segment& s, float t)
	{
		return A + B * t + C * t * t + D * t * t * t;
	}
	Vector velocity(const Segment& s, float t)
	{
		return B + C * t * 2 + D * t * t * 3;
	}
};
