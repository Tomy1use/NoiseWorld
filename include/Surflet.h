#pragma once

#include <Math/Pi.h>
#include <Math/Vector.h>
#include <Math/VectorFunc.h>

struct Edge
{
    Vector C, A, B;
    float R, L;
    Edge* next;
    Edge(): C(0), A(0), B(0), R(0), L(0) {}
    Edge(const Vector& A, const Vector& B)//line constructor
        : C(0), A(A), B(B), R(0)
        , L(length(A-B))
        , next(0)
	{
    }
    Edge(const Vector& C, const Vector& A, const Vector& B, float R)//arc constructor
        : C(C), A(A), B(B), R(R)
        , L(R ? R*Pi/2 : length(A-B))
        , next(0)
	{
    }
    void operator += (const Vector& v)
    {
        C += v, A +=v, B += v;
    }
};

enum SurfType {NoSurfType=0, SphereSurfType, QuadSurfType, CylinderSurfType, TorusSurfType};

struct Surf
{
    Surf* next;
    SurfType type;
};

struct SphereSurf: Surf
{
    Vector k, a, b, c;
    float r;
};

struct QuadSurf: Surf
{
    Vector a, b, c, d;
};

struct CylinderSurf: Surf
{
    Vector c1, a1, b1;
    Vector c2, a2, b2;
    float r;
};

struct TorusSurf: Surf
{
	Vector c1, a1, b1;
	Vector c2, a2, b2;
	Vector c3, a3, b3;
	float r1, r2, r3;
	float R;
};
