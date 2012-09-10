#pragma once

class NewellNormal
{
	Vector n, first, last;
	int pointCount;
	static Vector iteration(const Vector& A, const Vector& B){
		return Vector((A.y - B.y) * (A.z + B.z), (A.z - B.z) * (A.x + B.x), (A.x - B.x) * (A.y + B.y));
	}
public:
	NewellNormal(): n(ZeroVector), pointCount(0) {}
	void advance(const Vector& p)
	{
		if(pointCount == 0) first = p;
		else n += iteration(last, p);
		pointCount ++;
		last = p;
	}
	Vector result() const
	{
		if(pointCount > 0) return n + iteration(last, first);
		return n;
	}
};
