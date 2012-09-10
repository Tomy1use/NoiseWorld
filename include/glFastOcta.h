#pragma once

void glFastOcta(const Vector& center, const Vector& size)
{
    const Vector& P = center, &S = size;
    Vector T(P.x, P.y+S.y, P.z), B(P.x, P.y-S.y, P.z);
    Vector L(P.x-S.x, P.y, P.z), R(P.x+S.x, P.y, P.z);
    Vector K(P.x, P.y, P.z-S.z), F(P.x, P.y, P.z+S.z);
    glBegin(GL_TRIANGLE_FAN);
        glVertex(T);
        glVertex(L);
        glVertex(F);
        glVertex(R);
        glVertex(K);
        glVertex(L);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex(B);
        glVertex(K);
        glVertex(R);
        glVertex(F);
        glVertex(L);
        glVertex(K);
    glEnd();
}

