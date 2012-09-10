#pragma once


struct glCylinderQuarter
{
    float norSig;
    glCylinderQuarter(
        const Vector& c1, const Vector& a1, const Vector& b1, 
        const Vector& c2, const Vector& a2, const Vector& b2,
        float R, int depth)
    {
        norSig = signum(dot(a1-c1, cross(a2-a1, b1-a1)));//flip normal if concave
        glBegin(GL_QUAD_STRIP);
            glNormal((a1-c1)*norSig);
            glVertex(a1);
            glNormal((a2-c2)*norSig);
            glVertex(a2);
            if(depth > 0) recurse(c1, a1, b1, c2, a2, b2, R, depth-1);
            glNormal((b1-c1)*norSig);
            glVertex(b1);
            glNormal((b2-c2)*norSig);
            glVertex(b2);
        glEnd();
    }
    void recurse(
        const Vector& c1, const Vector& a1, const Vector& b1, 
        const Vector& c2, const Vector& a2, const Vector& b2,
        float R, int depth)
    {
        Vector m1 = unit((a1+b1)/2 - c1) * R + c1;
        Vector m2 = unit((a2+b2)/2 - c2) * R + c2;
        if(depth > 0) recurse(c1, a1, m1, c2, a2, m2, R, depth-1);
        glNormal((m1-c1)*norSig);
        glVertex(m1);
        glNormal((m2-c2)*norSig);
        glVertex(m2);
        if(depth > 0) recurse(c1, m1, b1, c2, m2, b2, R, depth-1);
    }
};
