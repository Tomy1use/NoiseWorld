#pragma once

#include <Math/Plane.h>
#include <Math/Matrix.h>
#include <Math/Vector.h>
#include <Math/VectorFunc.h>

inline float distance(const Plane& plane, const Vector& point) {return dot(point, plane.n) - plane.w;}

Plane planeFromPoints(const Vector& a, const Vector& b, const Vector& c);
bool makePlaneFromPoints(const Vector& a, const Vector& b, const Vector& c, Plane& p);
Plane planeFromPointNormal(const Vector& p, const Vector& n);
Vector projectVectorOnPlane(const Vector& a, const Plane& p);
Vector normalFromPoints(const Vector& a, const Vector& b, const Vector& c);//not unit length!

Plane transform(const Plane& plane, const Matrix& matrix);

bool planeHitRay(const Plane& plane, const Vector& pos, const Vector& vel, Vector& hit);
bool planeHitLine(const Plane& plane, const Vector& a, const Vector& b, Vector& x);
