#include <Math/Vector.h>
#include <Math/VectorFunc.h>
#include <Math/VectorConst.h>
#include <Math/Matrix.h>
#include <Math/MatrixConst.h>
#include <Math/MathFunc.h>

Vector Axes::out(const Vector& a) const
{
	return x * a.x + y * a.y + z * a.z;
}

	
Vector Axes::in(const Vector& a) const
{
	float X = dot(a, x) / square(x);
	float Y = dot(a, y) / square(y);
	float Z = dot(a, z) / square(z);
	return Vector(X, Y, Z);
}


Matrix::Matrix(const Vector& o)
    : axes(UnitAxes)
    , origin(o)
{
}

Vector Matrix::out(const Vector& a) const
{
	return axes.out(a) + origin;
}


Vector Matrix::in(const Vector& a) const
{
	return axes.in(a - origin);
}


Matrix Matrix::out(const Matrix& s) const
{
	return Matrix(
		Axes(
			axes.out(s.axes.x),
			axes.out(s.axes.y),
			axes.out(s.axes.z)
		),
		out(s.origin)
	);				
}


Matrix Matrix::in(const Matrix& s) const
{
	return Matrix(
		Axes(
			axes.in(s.axes.x),
			axes.in(s.axes.y),
			axes.in(s.axes.z)
		),
		in(s.origin)
	);
}
	

Matrix matrixLookAt(const Vector& from, const Vector& at)
{
    Vector z = unit(from - at);
	Vector x = unit(cross(UnitVectorY, z));
    Vector y = unit(cross(z, x));
    return Matrix(Axes(x, y, z), from);
}


Matrix matrixFromOriginAxisZ(const Vector& o, const Vector& z)
{
    int i = maxDim(z);
    Vector u = (i == 0) ? UnitVectorY : (i == 1) ? UnitVectorZ : UnitVectorX;
	Vector x = unit(cross(u, z));
	Vector y = unit(cross(z, x));
	return Matrix(Axes(x, y, z), o);
}

Matrix matrixFromOriginAxisZ(const Vector& o, const Vector& z, const Vector& up)
{
	Vector x = unit(cross(up, z));
	Vector y = unit(cross(z, x));
	return Matrix(Axes(x, y, z), o);
}

Matrix matrixFromOriginAxisY(const Vector& o, const Vector& y)
{
	Vector z = abs(y.y) > abs(y.x) ?  unit(cross(UnitVectorX, y)) : (abs(y.x)>abs(y.z)) ? unit(cross(UnitVectorZ, y)) : unit(cross(UnitVectorX, y));
	Vector x = unit(cross(y, z));
	return Matrix(Axes(x, y, z), o);
}

Matrix Matrix::translate(const Vector& v) const
{
	Matrix r(*this);
	r.origin += v;
	return r;
}

Matrix Matrix::operator * (const Vector& v) const
{
	return Matrix(axes * v, origin);
}

Matrix Matrix::operator + (const Vector& v) const
{
	return Matrix(axes, origin + v);
}
