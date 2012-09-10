#pragma once

#include <Math/Vector.h>
#include <Math/VectorFunc.h>
#include <Math/Matrix.h>
#include <gl/gl.h>

template<int Reso>
struct glGeneralCylinder
{
    struct {
        float x[Reso+1], y[Reso+1];//precomputed section circle points
    } section;
    Vector prePrePoint, prePoint, point;
    float prePreRadius, preRadius, radius;
    Matrix prePreMatrix, preMatrix, matrix;
    glGeneralCylinder()
        : prePreRadius(0)
        , preRadius(0)
        , radius(0)
    {
        for(unsigned int i=0; i<Reso+1; i++){
            section.x[i] = sinf(Pi * 2 * i / Reso);
            section.y[i] = cosf(Pi * 2 * i / Reso);
        }
    }
    void draw(const Matrix& m1, const Matrix& m2, float r1, float r2)
    {
        glBegin(GL_QUAD_STRIP);
        for(unsigned int i=0; i<=Reso; i++){
            Vector p1(section.x[i%Reso], section.y[i%Reso], 0);
            Vector p2(section.x[i%Reso], section.y[i%Reso], 0);
            glNormal(m1.axes.out(p1));
            glVertex(m1.out(p1*r1));
            glNormal(m2.axes.out(p2));
            glVertex(m2.out(p2*r2));
        }
        glEnd();
    }
    void advance(const Vector& Point, float Radius)
    {
        prePreMatrix = preMatrix;
        prePrePoint = prePoint;
        prePreRadius = preRadius;
        prePoint = point;
        preRadius = radius;
        point = Point;
        radius = Radius;
        if(prePreRadius){
            preMatrix = matrixFromOriginAxisZ(prePoint, unit(Point-prePrePoint), preMatrix.axes.y);
        }else{
            preMatrix = matrixFromOriginAxisZ(prePoint, unit(Point-prePoint), unit(perpendicular(Point-prePoint)));
        }
        if(preRadius){
            if(prePreRadius){
                draw(prePreMatrix, preMatrix, prePreRadius, preRadius);
            }
        }
    }
    void finish()
    {
        if(preRadius){
            matrix = matrixFromOriginAxisZ(point, unit(point-prePoint), preMatrix.axes.y);
            draw(preMatrix, matrix, preRadius, radius);
        }
    }
};
