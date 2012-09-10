#include <Math/Frustum.h>
#include <Math/FrustumFunc.h>
#include <Math/PlaneFunc.h>

Frustum transform(const Frustum& in, const Matrix& matrix)
{
    return Frustum(
        transform(in.planes[0], matrix), 
        transform(in.planes[1], matrix), 
        transform(in.planes[2], matrix),
        transform(in.planes[3], matrix),
		transform(in.planes[4], matrix));
}

bool isSpherePartInFrustum(const Vector& center, float radius, const Frustum& frustum)
{
    for(int i=0; i<5; i++){
        if(distance(frustum.planes[i], center) < -radius){
            return false;
        }
    }
    return true;
}


Frustum pyramidFrustum(float width, float height, float focalDistance)
{
    Vector A(-width/2, -height/2, focalDistance);
    Vector B(-width/2, +height/2, focalDistance);
    Vector C(+width/2, +height/2, focalDistance);
    Vector D(+width/2, -height/2, focalDistance);
    Vector E(0, 0, 0);
    return Frustum(
        Plane(Vector(0,0,-1), 0),
		planeFromPoints(A, B, E),
        planeFromPoints(B, C, E),
        planeFromPoints(C, D, E),
        planeFromPoints(D, A, E));
}


Frustum corridorFrustum(float width, float height)
{
    return Frustum(
        Plane(Vector(+1, 0, 0), -width/2+.001f),
        Plane(Vector(-1, 0, 0), -width/2+.001f),
        Plane(Vector(0, +1, 0), -height/2+.001f),
        Plane(Vector(0, -1, 0), -width/2+.001f),
		Plane(Vector(0,0,-1), 0)
	);
}
