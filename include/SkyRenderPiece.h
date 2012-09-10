#pragma once

#include <Math/RandomNumber.h>

struct SkyRenderPiece
{
	GLuint dispList;
	
	SkyRenderPiece()
	{
		dispList = glGenLists(1);
		glNewList(dispList, GL_COMPILE);
			float g = (sqrtf(5) - 1) / 2;
			Vector p[] = {
				Vector(-1, -g, 0), Vector(-1, g, 0), Vector(1, -g, 0), Vector(1, g, 0), Vector(0, -1, -g), Vector(0, -1, g), 
				Vector(0, 1, -g), Vector(0, 1, g), Vector(-g, 0, -1), Vector(g, 0, -1), Vector(-g, 0, 1), Vector(g, 0, 1)};
			enum {xy=0, xY, Xy, XY, yz, yZ, Yz, YZ, zx, zX, Zx, ZX};
			short t[] = {xy, xY, zx, xY, xy, Zx, Xy, XY, ZX, XY, Xy, zX, yz, yZ, xy, yZ, yz, Xy, Yz, YZ, XY, YZ, Yz, xY, zx, zX, yz, zX, zx, Yz, 
				Zx, ZX, YZ, ZX, Zx, yZ, ZX, XY, YZ, yz, xy, zx, Yz, XY, zX,  Zx, xy, yZ, YZ, xY, Zx, zX, Xy, yz, yZ, Xy, ZX, zx, xY, Yz};
			glBegin(GL_TRIANGLES);
				for(int i=0; i<20; i++){
					Vector A = p[t[i*3]], B = p[t[i*3+1]], C = p[t[i*3+2]];
					glColor(colorForVertex(A));
					//glNormal(A); 
					glVertex(A); 
					glColor(colorForVertex(B));
					//glNormal(B); 
					glVertex(B);
					glColor(colorForVertex(C)); 
					//glNormal(C); 
					glVertex(C);
				}
			glEnd();
		glEndList();
	}
	Vector colorForVertex(const Vector& v)
	{
		return v * .3f + Vector(.7f);
		//return lerp(Vector(.7f,.7f,1), Vector(.5f,.5f,1), v.y);
	}
	~SkyRenderPiece()
	{
		glDeleteLists(dispList, 1);
	}
};
