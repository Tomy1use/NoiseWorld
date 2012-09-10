#pragma once

struct CircleAroundTrigon
{
	Vector center;
	float radiusSquare;
	CircleAroundTrigon(const Vector& A, const Vector& B, const Vector& C)
		: center(0), radiusSquare(0)
	{
		float alpha = 
			(C.z - B.z) * (A.x * A.x + A.z * A.z) + 
			(A.z - C.z) * (B.x * B.x + B.z * B.z) + 
			(B.z - A.z) * (C.x * C.x + C.z * C.z);
		float betha = 2 * (C.x * (B.z - A.z) + A.x * (C.z - B.z) + B.x * (A.z - C.z));
		center.x = alpha / betha;
		center.z = (((A.x * A.x + A.z*A.z) - (B.x * B.x + B.z * B.z)) / 2 - center.x * (A.x - B.x)) / (A.z - B.z);
		center.y = 0;
		radiusSquare = square(Vector(A.x,0,A.z) - center);
	}
};
