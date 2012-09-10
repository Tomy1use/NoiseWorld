#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <Math/Vector.h>

void glFastBox(float L, float B, float K, float R, float T, float F)
{
	glNormal3f(-1,-1,-1);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(L, B, K);
        glVertex3f(R, B, K);
        glVertex3f(R, B, F);
        glVertex3f(L, B, F);
        glVertex3f(L, T, F);
        glVertex3f(L, T, K);
        glVertex3f(R, T, K);
        glVertex3f(R, B, K);
    glEnd();
	glNormal3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(R, T, F);
        glVertex3f(R, B, F);
        glVertex3f(R, B, K);
        glVertex3f(R, T, K);
        glVertex3f(L, T, K);
        glVertex3f(L, T, F);
        glVertex3f(L, B, F);
        glVertex3f(R, B, F);
    glEnd();
}

void glFastBox(const Vector& Min, const Vector& Max)
{
    glFastBox(Min.x, Min.y, Min.z, Max.x, Max.y, Max.z);
}

void glFastBox(const Vector& C, float R)
{
	glFastBox(C.x-R, C.y-R, C.z-R, C.x+R, C.y+R, C.z+R);
}
