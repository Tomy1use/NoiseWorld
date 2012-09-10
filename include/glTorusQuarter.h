#pragma once


//Technically, it is 1/16 of torus
struct glTorusQuarter
{
    glTorusQuarter(
        const Vector& c1, const Vector& a1, const Vector& b1, float r1,
        const Vector& c2, const Vector& a2, const Vector& b2, float r2,
        const Vector& c3, const Vector& a3, const Vector& b3, float r3,
        float R, int depth, int Depth)
    {
        recurse(c1, a1, b1, r1, c2, a2, b2, r2, c3, a3, b3, r3, R, depth, Depth);
    }
    void recurse(
        const Vector& c1, const Vector& a1, const Vector& b1, float r1,
        const Vector& c2, const Vector& a2, const Vector& b2, float r2,
        const Vector& c3, const Vector& a3, const Vector& b3, float r3,
        float R, int depth, int Depth)
    {
        if(depth > 0){
            Vector m1 = unit((a1+b1)/2 - c1) * r1 + c1;
            Vector m2 = unit((a2+b2)/2 - c2) * r2 + c2;
            Vector m3 = unit((a3+b3)/2 - c3) * r3 + c3;
            recurse(c1, a1, m1, r1, c2, a2, m2, r2, c3, a3, m3, r3, R, depth-1, Depth);
            recurse(c1, m1, b1, r1, c2, m2, b2, r2, c3, m3, b3, r3, R, depth-1, Depth);
        }else{
            glCylinderQuarter(a3, a1, a2, b3, b1, b2, R, Depth);
        }
    }
};
