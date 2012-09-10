#include <stdlib.h>
#include <math.h>
#include <Math/Vector.h>
#include <Math/VectorConst.h>
#include <Math/VectorFunc.h>
#include <Math/MathFunc.h>

float length(const Vector& v)
{
	return sqrtf(square(v));
}


Vector unit(const Vector& v)
{
	float f = length(v);
	if(f > 0){
		return Vector(v.x / f, v.y / f, v.z / f);
	}
	return ZeroVector;
}


float unify(Vector& v)
{
	float f = length(v);
	if(f > 0){
		v /= f;
	}
	return f;
}


Vector rotateZ(const Vector& v, float angle)
{
	float C = cosf(angle), S = sinf(angle);
	return Vector(v.x * C + v.y * S, v.y * C - v.x * S, v.z);
}


Vector rotateY(const Vector& v, float angle)
{
	float C = cosf(angle), S = sinf(angle);
	return Vector( v.x * C - v.z * S, v.y, v.x * S + v.z * C);
}


Vector rotateX(const Vector& v, float angle)
{
	float C = cosf(angle), S = sinf(angle);
	return Vector(v.x, v.y * C - v.z * S, v.y * S + v.z * C);
}


Vector perpendicular(const Vector& a)
{
    if(abs(a.x) > abs(a.y)){
        if(abs(a.x) > abs(a.z)){
            return cross(a, UnitVectorY);
        }
    }
    if(abs(a.y) > abs(a.z)){
        return cross(a, UnitVectorZ);
    }
    return cross(a, UnitVectorX);
}


Vector vectorOnVector(const Vector& a, const Vector& b)
{
	if(square(b) == 0){
		return b;
	}
	return b * dot(a, b) / square(b);
}


bool isPointBetweenPoints(const Vector& p, const Vector& a, const Vector& b)
{
	Vector v = b - a;
	return ((p - a) * v > 0) && ((b - p) * v > 0);
}


bool isPointBeforeSegment(const Vector& p, const Vector& a, const Vector& b)
{
	return ((a - p) * (b - a) > 0);
}


bool isPointAfterVector(const Vector& p, const Vector& a, const Vector& b)
{
	return ((p - b) * (b - a) > 0);
}


Vector lerp(const Vector& a, const Vector& b, float t)
{
    return a * (1-t) + b * t;
}


int maxDim(const Vector& v)
{
    if(abs(v.x) > abs(v.y) && abs(v.x) > abs(v.z)) return 0;
    if(abs(v.y) > abs(v.z)) return 1;
    return 2;
}

int minDim(const Vector& v)
{
    if(abs(v.x) < abs(v.y) && abs(v.x) < abs(v.z)) return 0;
    if(abs(v.y) < abs(v.z)) return 1;
    return 2;
}

Vector nearestAxis(const Vector& v)
{
    switch(maxDim(v)){
        case 0: return Vector(signum(v.x), 0, 0);
        case 1: return Vector(0, signum(v.y), 0);
        case 2: return Vector(0, 0, signum(v.z));
    }
    assert(false);
    return ZeroVector;
}

Vector snapToAxis(const Vector& v)
{
    return snapToAxis(v, maxDim(v));
}

Vector snapToAxis(const Vector& v, int i)
{
    switch(i){
        case 0: return Vector(v.x, 0, 0);
        case 1: return Vector(0, v.y, 0);
        case 2: return Vector(0, 0, v.z);
    }
    assert(false);
    return ZeroVector;
}

Vector min(const Vector& a, const Vector& b)
{
    return Vector(minimum(a.x, b.x), minimum(a.y, b.y), minimum(a.z, b.z));
}

Vector max(const Vector& a, const Vector& b)
{
    return Vector(maximum(a.x, b.x), maximum(a.y, b.y), maximum(a.z, b.z));
}
