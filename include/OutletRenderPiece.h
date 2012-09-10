#pragma once


struct OutletRenderPiece
{
	GLuint dispList;
	~OutletRenderPiece()
	{
		glDeleteLists(dispList, 1);
	}
	OutletRenderPiece()
		: dispList(0)
	{
		dispList = glGenLists(1);
		glNewList(dispList, GL_COMPILE);
			glBegin(GL_QUAD_STRIP);
				glNormal3f(-1,0,1); glVertex3f(-1, 1, 0); glVertex3f(-1, -1, 0);
				glNormal3f(1,0,1); glVertex3f(0, 1, 1); glVertex3f(0, -1, 1);
				glNormal3f(1,0,0); glVertex3f(1, 1, 0); glVertex3f(1, -1, 0);
				glNormal3f(0,0,-1); glVertex3f(1, +1, -1); glVertex3f(1, -1, -1);
				glNormal3f(-1,0,0); glVertex3f(-1, 1, -1); glVertex3f(-1, -1, -1);
				glNormal3f(-1,0,1); glVertex3f(-1, 1, 0); glVertex3f(-1, -1, 0);
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glNormal3f(0, 1, 0);
				glVertex3f(1, 1, 0);
				glVertex3f(1, 1, -1);
				glVertex3f(-1, 1, -1);
				glVertex3f(-1, 1, 0);
				glVertex3f(0, 1, 1);
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glNormal3f(0, -1, 0);
				glVertex3f(0, -1, 1);
				glVertex3f(-1, -1, 0);
				glVertex3f(-1, -1, -1);
				glVertex3f(1, -1, -1);
				glVertex3f(1, -1, 0);
			glEnd();
		glEndList();
	}
};

/*
glBegin(GL_QUAD_STRIP);
	glColor3f(1,0,0);
	glNormal3f(-1,0,1);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 0);
	glTexCoord2f(0, -1); glVertex3f(-1, -1, 0);
	glColor3f(0,1,0);
	glNormal3f(1,0,1);
	glTexCoord2f(1, 1); glVertex3f(0, 1, 1);
	glTexCoord2f(1, -1); glVertex3f(0, -1, 1);
	glColor3f(0,0,1);
	glNormal3f(1,0,0);
	glTexCoord2f(0, 1); glVertex3f(1, 1, 0);
	glTexCoord2f(0, -1); glVertex3f(1, -1, 0);
	glColor3f(1,1,0);
	glNormal3f(0,0,-1);
	glTexCoord2f(-1, 1); glVertex3f(1, +1, -1);
	glTexCoord2f(-1, -1); glVertex3f(1, -1, -1);
	glColor3f(1,0,1);
	glNormal3f(-1,0,0);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(1, -1); glVertex3f(-1, -1, -1);
	glColor3f(0,1,1);
	glNormal3f(-1,0,1);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 0);
	glTexCoord2f(0, -1); glVertex3f(-1, -1, 0);
glEnd();
glColor3f(1,1,1);
glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,1,0);
	glTexCoord2f(1, 0); glVertex3f(1, 1, 0);
	glTexCoord2f(1, -1); glVertex3f(1, 1, -1);
	glTexCoord2f(-1, -1); glVertex3f(-1, 1, -1);
	glTexCoord2f(-1, 0); glVertex3f(-1, 1, 0);
	glTexCoord2f(0, 1); glVertex3f(0, 1, 1);
glEnd();
glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,-1,0);
	glTexCoord2f(0, 1); glVertex3f(0, -1, 1);
	glTexCoord2f(-1, 0); glVertex3f(-1, -1, 0);
	glTexCoord2f(-1, -1); glVertex3f(-1, -1, -1);
	glTexCoord2f(1, -1); glVertex3f(1, -1, -1);
	glTexCoord2f(1, 0); glVertex3f(1, -1, 0);
glEnd();
*/
