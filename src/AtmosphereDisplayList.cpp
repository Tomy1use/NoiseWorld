#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <Math/Pi.h>
#include <Math/Vector.h>
#include <Math/MathFunc.h>
#include <math.h>
#include <glFunc.h>

static Vector skyColor(float y)
{
	y = 1 - abs(y);
	y *= .9f;
	y *= y;
	Vector c1(1,1,1), c2(.3f,.3f,1);
	return lerp(c2, c1, y);
}

GLuint createAtmosphereDisplayList()
{
	GLuint dispList= glGenLists(1);
	const int ResoV = 12, ResoH = 12;
	glNewList(dispList, GL_COMPILE);
		for(int j=-ResoV; j<ResoV-1; j++){
			float y1 = float(j) / float(ResoV-1);
			float y2 = float(j+1) / float(ResoV-1);
			float r1 = 1.f - y1 * y1;
			float r2 = 1.f - y2 * y2;
			Vector c1 = skyColor(y1);
			Vector c2 = skyColor(y2);
			glBegin(GL_QUAD_STRIP);
			for(int i=0; i<ResoH; i++){
				float x = cosf(i * 2 * Pi / (ResoH-1));
				float z = sinf(i * 2 * Pi / (ResoH-1));
				glColor(c1);
				glVertex3f(x*r1, y1, z*r1);
				glColor(c2);
				glVertex3f(x*r2, y2, z*r2);
			}
			glEnd();
		}
	glEndList();
	return dispList;
}