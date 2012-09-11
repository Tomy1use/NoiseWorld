#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <Math/Vector.h>
#include <Math/VectorFunc.h>
#include <Math/Pi.h>
#include <glFunc.h>

Vector getVec(float vAng, float hAng)
{
	float y = cosf(vAng);
	float r = sqrtf(1 - y * y);
	float x = r * cosf(hAng);
	float z = r * sinf(hAng);
	return Vector(x, y, z);
}

Vector sampleTrigon(const Vector& a, const Vector& b, const Vector& c, float s, float t)
{
	return (b - a) * s + (c - a) * t + a;
}

Vector sampleStrip(const Vector v[6], float s, float t)
{
	if(t < 1){
		if(s + t < 1){
			return sampleTrigon(v[0], v[1], v[2], s, t);
		}
		return sampleTrigon(v[3], v[2], v[1], 1-s, 1-t);
	}
	t -= 1;
	if(s + t < 1){
		return sampleTrigon(v[2], v[3], v[4], s, t);
	}
	return sampleTrigon(v[5], v[4], v[3], 1-s, 1-t);
}

void emitStrip(Vector v[6], float s1, float s2, float dt, int count, int depth)
{
	if(depth > 0){
		emitStrip(v, s1, (s1+s2)/2, dt/2, count*2-1, depth-1);
		emitStrip(v, (s1+s2)/2, s2, dt/2, count*2-1, depth-1);
	}else{
		glBegin(GL_TRIANGLE_STRIP);
			/*
			static Color colors[] = {
				Color(255,255,0),
				Color(0,255,255),
				Color(255,0,255),
				Color(255,255,255),
				Color(0,255,0)
			};
			static int i = 0;
			glColor(colors[(i++)%5]);
			*/
			for(int i=0; i<count; i++){
				Vector v1 = sampleStrip(v, s1, i*dt);
				Vector v2 = sampleStrip(v, s2, i*dt);
				v1 /= length(v1);
				v2 /= length(v2);
				glNormal(v1);
				glTexCoord3f(v1.x, v1.y, v1.z);
				glVertex(v1);
				glNormal(v2);
				glTexCoord3f(v2.x, v2.y, v2.z);
				glVertex(v2);
			}
		glEnd();
	}
}

GLuint subdivideIcosahedronDisplayList(int depth)
{
	int dispList = glGenLists(1);
	
	glNewList(dispList, GL_COMPILE);
		for(int i=0; i<5; i++){
			float vAng1 = -Pi;
			float vAng2 = -Pi+Pi/3;
			float vAngStep = Pi/3;
			float hAng1 = i*2*Pi/5+Pi/5;
			float hAng2 = i*2*Pi/5;
			float hAngStep = Pi/5;
			Vector v[6];
			for(int j=0; j<3; j++){
				v[j*2+0] = getVec(vAng1+j*vAngStep, hAng1+j*hAngStep);
				v[j*2+1] = getVec(vAng2+j*vAngStep, hAng2+j*hAngStep);
			}
			emitStrip(v, 0, 1, 1, 3, depth);
		}
	glEndList();
	return dispList;
}
