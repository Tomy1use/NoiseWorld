#pragma once


struct glArc
{
    float r1, r2, radius;
    Vector side;
    glArc(const Vector& C, const Vector& A, const Vector& B, float Radius, float T, int Depth)
    {
        assert(Radius > 0);
        radius = Radius;
        r1 = (Radius - T) / Radius;
        r2 = (Radius + T) / Radius;
        side = unit(cross(A-C, B-C));
        strip(-T, +T, r1, r1, C, A, B, Depth);
        strip(+T, -T, r2, r2, C, A, B, Depth);
        strip(-T, -T, r2, r1, C, A, B, Depth);
        strip(+T, +T, r1, r2, C, A, B, Depth);
    }
    void strip(float s1, float s2, float r1, float r2, 
        const Vector& C, const Vector& A, const Vector& B, int depth)
    {
        glBegin(GL_QUAD_STRIP);
            glVertex(lerp(C, A, r1) + side * s1);
            glVertex(lerp(C, A, r2) + side * s2);
            if(depth > 0) recurse(s1, s2, r1, r2, C, A, B, depth-1);
            glVertex(lerp(C, B, r1) + side * s1);
            glVertex(lerp(C, B, r2) + side * s2);
        glEnd();
    }
    void recurse(
        float s1, float s2, float r1, float r2, 
        const Vector& C, const Vector& A, const Vector& B, int depth)
    {
        Vector M = unit((A+B)/2 - C) * radius + C;
        if(depth > 0) recurse(s1, s2, r1, r2, C, A, M, depth-1);
        glVertex(lerp(C, M, r1) + side * s1);
        glVertex(lerp(C, M, r2) + side * s2);
        if(depth > 0) recurse(s1, s2, r1, r2, C, M, B, depth-1);
    }
};
