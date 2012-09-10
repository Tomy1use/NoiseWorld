#pragma once

#include <Math/Vector.h>
#include <Math/Axes.h>

struct Matrix
{
	Axes axes;
	Vector origin;
	
	Matrix() {}
	Matrix(const Axes& a, const Vector& o): axes(a), origin(o) {}
    Matrix(const Vector& o);
    
	Vector out(const Vector& a) const;
	Vector in(const Vector& a) const;
	Matrix out(const Matrix& s) const;
	Matrix in(const Matrix& s) const;
	
	Matrix translate(float x, float y, float z) const {return translate(Vector(x, y, z));}
	Matrix translate(const Vector& v) const;
	Matrix operator * (const Vector& v) const;
	Matrix operator + (const Vector& v) const;
	
	Matrix operator - (const Matrix& a) {return Matrix(axes-a.axes, origin-a.origin);}
};
