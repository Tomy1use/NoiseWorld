#pragma once

#include <CutEarTriangulation.h>

struct BuildingRenderPiece
{
	GLuint dispList;
	BuildingRenderPiece(Building* b)
	{
		dispList = glGenLists(1);
		glNewList(dispList, GL_COMPILE);
		
		for(int s=0; s<b->profilePointCount-1; s++){
			if(s % 4 == 2) glColor3f(.6f,.6f,.6f); else glColor3f(.9f,.9f,.9f);
			float tX = 0;
			glBegin(GL_QUAD_STRIP);
				for(int p=0; p<=b->basePointCount; p++){
					int i = p % b->basePointCount;
					Vector N = b->pointNormals[i];
					N.y = b->profile[s].z;
					glNormal(N);
					glTexCoord2f(tX, b->profile[s].y);
					glVertex(b->basePoints[i] + UnitVectorY * b->profile[s].y + b->pointNormals[i] * b->profile[s].x);
					glTexCoord2f(tX, b->profile[s+1].y);
					glVertex(b->basePoints[i] + UnitVectorY * b->profile[s+1].y + b->pointNormals[i] * b->profile[s+1].x);
					tX += length(b->basePoints[(p+1)%b->basePointCount] - b->basePoints[p%b->basePointCount]);
				}
				printf("\n");
			glEnd();
		}
		
		typedef CutEarTriangulation<Building::MaxBasePoints> Triangulation;
		Triangulation T;
		for(int i=b->basePointCount-1; i>=0; i--){
			int s = b->profilePointCount-1;
			Vector P = b->basePoints[i] + UnitVectorY * b->profile[s].y + b->pointNormals[i] * b->profile[s].x;
			T.addPoint(P);
		}
		T.compute();
		
		glNormal3f(0,1,0);
		glBegin(GL_TRIANGLES);
			for(int i=0; i<T.trigonCount; i++){
				Vector& A = T.tris[i].a, &B = T.tris[i].b, &C = T.tris[i].c;
				glTexCoord2f(A.x, A.z); glVertex(A);
				glTexCoord2f(B.x, B.z); glVertex(B);
				glTexCoord2f(C.x, C.z); glVertex(C);
			}
		glEnd();
		
		glEndList();
	}
	~BuildingRenderPiece()
	{
		glDeleteLists(dispList, 1);
	}
};
