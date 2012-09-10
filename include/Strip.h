#pragma once

#include <gl/gl.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>

struct Strip
{
    Vector prePrePoint, prePoint, point;
    float prePreRadius, preRadius, radius;
    Matrix prePreMatrix, preMatrix, matrix;
    float L;
    Strip()
        : prePreRadius(0)
        , preRadius(0)
        , radius(0)
        , L(0)
    {
    }
    void draw(const Matrix& m, float r)
    {
        glNormal(m.axes.y);
        glTexCoord2f(-r, L);
        glVertex(m.origin - m.axes.x * r);
        glTexCoord2f(+r, L);
        glVertex(m.origin + m.axes.x * r);
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
            preMatrix = matrixFromOriginAxisZ(prePoint, unit(Point-prePoint), UnitVectorY);
        }
        if(preRadius){
            draw(preMatrix, preRadius);
        }
        L += length(Point - prePoint);
    }
    void finish()
    {
        if(preRadius){
            matrix = matrixFromOriginAxisZ(point, unit(point-prePoint), preMatrix.axes.y);
            draw(matrix, radius);
        }
    }
};
