#pragma once

struct Vector
{
    float x, y, z;
	
    Vector() {}
	Vector(float X, float Y, float Z): x(X), y(Y), z(Z) {}
	Vector(float X, float Y): x(X), y(Y), z(0) {}
    Vector(int i): x(float(i)), y(float(i)), z(float(i)) {}
    Vector(float f): x(f), y(f), z(f) {}
	
	Vector operator + (const Vector& a) const {return Vector(x+a.x, y+a.y, z+a.z);}
	Vector operator - (const Vector& a) const {return Vector(x-a.x, y-a.y, z-a.z);}
	float operator * (const Vector& a) const {return (x * a.x + y * a.y + z * a.z);}
    bool operator == (const Vector& a) const {return x == a.x && y == a.y && z == a.z;}
	Vector operator * (float f) const {return Vector(x * f, y * f, z * f);}
	Vector operator / (float f) const {return Vector(x / f, y / f, z / f);}
    bool operator < (const Vector& a) const {return x < a.x && y < a.y && z < a.z;}
    bool operator <= (const Vector& a) const {return x <= a.x && y <= a.y && z <= a.z;}
    bool operator > (const Vector& a) const {return x > a.x && y > a.y && z > a.z;}
    bool operator >= (const Vector& a) const {return x >= a.x && y >= a.y && z >= a.z;}
	void operator += (const Vector& a) {x += a.x, y += a.y, z += a.z;}
	void operator -= (const Vector& a) {x -= a.x, y -= a.y, z -= a.z;}
	void operator *= (float f) {x *= f, y *= f, z *= f;}
	void operator /= (float f) {x /= f, y /= f, z /= f;}
	Vector operator - () const {return Vector(-x, -y, -z);}
	Vector operator + () const {return *this;}
};
