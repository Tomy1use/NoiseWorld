#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <Math/Vector.h>

void glTexBox(float L, float B, float K, float R, float T, float F)
{
    glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glTexCoord2f(L, K); glVertex3f(L, B, K);
		glTexCoord2f(R, K); glVertex3f(R, B, K);
		glTexCoord2f(R, F); glVertex3f(R, B, F);
		glTexCoord2f(L, F); glVertex3f(L, B, F);
		glNormal3f(0, 1, 0);
		glTexCoord2f(L, F); glVertex3f(L, T, F);
		glTexCoord2f(R, F); glVertex3f(R, T, F);
		glTexCoord2f(R, K); glVertex3f(R, T, K);
		glTexCoord2f(L, K); glVertex3f(L, T, K);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(B, K); glVertex3f(L, B, K);
		glTexCoord2f(B, F); glVertex3f(L, B, F);
		glTexCoord2f(T, F); glVertex3f(L, T, F);
		glTexCoord2f(T, K); glVertex3f(L, T, K);
		glNormal3f(1, 0, 0);
		glTexCoord2f(T, K); glVertex3f(R, T, K);
		glTexCoord2f(T, F); glVertex3f(R, T, F);
		glTexCoord2f(R, F); glVertex3f(R, B, F);
		glTexCoord2f(R, K); glVertex3f(R, B, K);
		glNormal3f(0, 0, -1);
		glTexCoord2f(L, B); glVertex3f(L, B, K);
		glTexCoord2f(L, T); glVertex3f(L, T, K);
		glTexCoord2f(R, T); glVertex3f(R, T, K);
		glTexCoord2f(R, B); glVertex3f(R, B, K);
		glNormal3f(0, 0, 1);
		glTexCoord2f(R, B); glVertex3f(R, B, F);
		glTexCoord2f(R, T); glVertex3f(R, T, F);
		glTexCoord2f(L, T); glVertex3f(L, T, F);
		glTexCoord2f(L, B); glVertex3f(L, B, F);
	glEnd();
}

void glTexBox(const Vector& Min, const Vector& Max)
{
    glTexBox(Min.x, Min.y, Min.z, Max.x, Max.y, Max.z);
}
