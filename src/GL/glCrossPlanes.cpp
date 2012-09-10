#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <Math/Vector.h>
#include <glFunc.h>

void glCrossPlanes(const Vector& P, float S)
{
    Vector planeX[] = {Vector(P.x,P.y-S,P.z+S), Vector(P.x,P.y-S,P.z-S), Vector(P.x,P.y+S,P.z-S), Vector(P.x,P.y+S,P.z+S)};
    Vector planeY[] = {Vector(P.x-S,P.y,P.z+S), Vector(P.x+S,P.y,P.z+S), Vector(P.x+S,P.y,P.z-S), Vector(P.x-S,P.y,P.z-S)};
    Vector planeZ[] = {Vector(P.x-S,P.y-S,P.z), Vector(P.x+S,P.y-S,P.z), Vector(P.x+S,P.y+S,P.z), Vector(P.x-S,P.y+S,P.z)};
    glBegin(GL_QUADS);
        glColor3f(1.f, .8f, .8f);
        glNormal3f(+1, 0, 0);
        for(int i=0; i<4; i++){
			glTexCoord2f(planeX[i].y, planeX[i].z);
			glVertex(planeX[i]);
		}
        glNormal3f(-1, 0, 0);
        for(int i=0; i<4; i++){
			glTexCoord2f(planeX[i].y, planeX[i].z);
			glVertex(planeX[3-i]);
		}
        glColor3f(.8f, 1.f, .8f);
        glNormal3f(0, +1, 0);
        for(int i=0; i<4; i++){
			glTexCoord2f(planeY[i].z, planeY[i].x);
			glVertex(planeY[i]);
		}
        glNormal3f(0, -1, 0);
        for(int i=0; i<4; i++){
			glTexCoord2f(planeY[i].z, planeY[i].x);
			glVertex(planeY[3-i]);
		}
        glColor3f(.8f, 1.f, 1.f);
        glNormal3f(0, 0, +1);
        for(int i=0; i<4; i++){
			glTexCoord2f(planeZ[i].x, planeZ[i].y);
			glVertex(planeZ[i]);
		}
        glNormal3f(0, 0, -1);
        for(int i=0; i<4; i++){
			glTexCoord2f(planeZ[i].x, planeZ[i].y);
			glVertex(planeZ[3-i]);
		}
    glEnd();
}
