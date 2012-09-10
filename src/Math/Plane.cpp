#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Math/Plane.h>
#include <Math/PlaneFunc.h>

inline Vector newellIteration(const Vector& A, const Vector& B)
{
	float x = (A.y - B.y) * (A.z + B.z);
	float y = (A.z - B.z) * (A.x + B.x);
	float z = (A.x - B.x) * (A.y + B.y);
	return Vector(x, y, z);
}

Vector normalFromPoints(const Vector& a, const Vector& b, const Vector& c)
{
	return newellIteration(a, b) + newellIteration(b, c) + newellIteration(c, a);
}

Plane planeFromPoints(const Vector& a, const Vector& b, const Vector& c)
{
	Vector n = unit(normalFromPoints(a, b, c));
	float w = dot(n, a);
	return Plane(n, w);
}

bool makePlaneFromPoints(const Vector& a, const Vector& b, const Vector& c, Plane& p)
{
	Vector n = normalFromPoints(a, b, c);
	float L = length(n);
	if(L > 0){
		p.n = n / L;
		p.w = dot(p.n, a);
		return true;
	}
	return false;
}

Plane planeFromPointNormal(const Vector& p, const Vector& n)
{
    return Plane(n, dot(p, n));
}

bool planeHitRay(const Plane& plane, const Vector& org, const Vector& vel, Vector& hit)
{
	float speed = -dot(vel, plane.n);
	if(speed == 0){
		return false;
	}
	float time = distance(plane, org) / speed;
	hit = vel * time + org;
	return true;
}


bool planeHitLine(const Plane& plane, const Vector& a, const Vector& b, Vector& x)
{
    float da = distance(plane, a);
    float db = distance(plane, b);
    if(da * db >= 0){
        return false;
    }
    x = a + (b - a) * da / (da - db);
    return true;
}


Vector projectVectorOnPlane(const Vector& a, const Plane& p)
{
	return a - p.n * (a * p.n - p.w);
}


Plane transform(const Plane& plane, const Matrix& matrix)
{
    Vector N = matrix.axes.out(plane.n);
    float W = dot(N, matrix.out(plane.n * plane.w));
    return Plane(N, W);
}
