#pragma once

static inline Vector cross(const Vector& A, const Vector& B) {return Vector(A.y*B.z-A.z*B.y, A.z*B.x-A.x*B.z, A.x*B.y-A.y*B.x);}
static inline float dot(const Vector& A, const Vector& B) {return A.x * B.x + A.y * B.y + A.z * B.z;}
static inline float square(const Vector& v) {return dot(v, v);}
float length(const Vector& v);
Vector unit(const Vector& v);
float unify(Vector& v);
Vector rotateX(const Vector& v, float angle);
Vector rotateY(const Vector& v, float angle);
Vector rotateZ(const Vector& v, float angle);
Vector randomSignedVector();
Vector randomUnsignedVector();
Vector perpendicular(const Vector& a);
Vector vectorOnVector(const Vector& a, const Vector& b);//deprecated
static inline Vector project(const Vector& a, const Vector& b) {return vectorOnVector(a, b);}
Vector closestPointOfTrigon(const Vector& v, const Vector& a, const Vector& b, const Vector& c);
bool isPointBetweenPoints(const Vector& p, const Vector& a, const Vector& b);
bool isPointBeforeSegment(const Vector& p, const Vector& a, const Vector& b);
bool isPointAfterVector(const Vector& p, const Vector& a, const Vector& b);
static inline Vector VectorXZ(const Vector& v) {return Vector(v.x, 0, v.z);}
static inline Vector VectorSetY(const Vector& v, float Y) {return Vector(v.x, Y, v.z);}
Vector lerp(const Vector& a, const Vector& b, float t);
//Vector min(const Vector& a, const Vector& b);
//Vector max(const Vector& a, const Vector& b);
template<class RandomNumber>
Vector randomVector(float a, float b, RandomNumber& r) {return Vector((r++)(a, b), (r++)(a, b), (r++)(a, b));}
int maxDim(const Vector& v);//0: X,  1: Y,  2: Z
int minDim(const Vector& v);
Vector nearestAxis(const Vector& v);//the same as unit(snapToAxis(v))
Vector snapToAxis(const Vector& v);
Vector snapToAxis(const Vector& v, int i);
