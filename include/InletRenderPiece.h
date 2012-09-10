#pragma once


struct InletRenderPiece
{
	GLuint dispList;
	~InletRenderPiece()
	{
		glDeleteLists(dispList, 1);
	}
	InletRenderPiece()
		: dispList(0)
	{
		dispList = glGenLists(1);
		glNewList(dispList, GL_COMPILE);
			glBegin(GL_QUAD_STRIP);
				glNormal3f(-1,0,1); glVertex3f(-1, 1, 1); glVertex3f(-1, -1, 1);
				glNormal3f(1,0,1); glVertex3f(1, 1, 1); glVertex3f(1, -1, 1);
				glNormal3f(1,0,0); glVertex3f(1, 1, -1); glVertex3f(1, -1, -1);
				glNormal3f(0,0,-1); glVertex3f(0, +1, 0); glVertex3f(0, -1, 0);
				glNormal3f(-1,0,0); glVertex3f(-1, 1, -1); glVertex3f(-1, -1, -1);
				glNormal3f(-1,0,1); glVertex3f(-1, 1, 1); glVertex3f(-1, -1, 1);
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glNormal3f(0, 1, 0);
				glVertex3f(1, 1, 1);
				glVertex3f(1, 1, -1);
				glVertex3f(0, 1, 0);
				glVertex3f(-1, 1, -1);
				glVertex3f(-1, 1, 1);
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glNormal3f(0, -1, 0);
				glVertex3f(-1, -1, 1);
				glVertex3f(-1, -1, -1);
				glVertex3f(0, -1, 0);
				glVertex3f(1, -1, -1);
				glVertex3f(1, -1, 1);
			glEnd();
		glEndList();
	}
};
