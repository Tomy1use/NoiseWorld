#pragma once

#include <gl/gl.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Color.h>

inline void glVertex(const Vector& v)
{
    glVertex3f(v.x, v.y, v.z);
}

inline void glNormal(const Vector& v)
{
    glNormal3f(v.x, v.y, v.z);
}

inline void glColor(const Color& c)
{
    glColor4ub(c.R, c.G, c.B, c.A);
}

inline void glColor(const Vector& v)
{
	glColor3f(v.x, v.y, v.z);
}

inline void glLoadMatrix(const Matrix& m)
{
    float rows[16] = {
        m.axes.x.x, m.axes.x.y, m.axes.x.z, 0,
        m.axes.y.x, m.axes.y.y, m.axes.y.z, 0,
        m.axes.z.x, m.axes.z.y, m.axes.z.z, 0,
        m.origin.x, m.origin.y, m.origin.z, 1
    };
    glLoadMatrixf(rows);
}

inline void glMultMatrix(const Matrix& m)
{
    float rows[16] = {
        m.axes.x.x, m.axes.x.y, m.axes.x.z, 0,
        m.axes.y.x, m.axes.y.y, m.axes.y.z, 0,
        m.axes.z.x, m.axes.z.y, m.axes.z.z, 0,
        m.origin.x, m.origin.y, m.origin.z, 1
    };
    glMultMatrixf(rows);
}

inline void glTranslate(const Vector& v)
{
	glTranslatef(v.x, v.y, v.z);
}
