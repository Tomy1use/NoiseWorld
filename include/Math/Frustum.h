#pragma once

#include <Math/Plane.h>
#include <Math/Vector.h>

struct Frustum
{
    Plane planes[5];
    
    Frustum() {}
    Frustum(const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5)
    {
        planes[0] = p1, planes[1] = p2, planes[2] = p3, planes[3] = p4, planes[4] = p5;
    }
};

