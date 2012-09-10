#pragma once

#include <Math/Vector.h>
#include <Math/VectorFunc.h>
#include <Math/Matrix.h>

template<int Reso>
struct MultiPointLine
{
    struct {
        float x[Reso+1], y[Reso+1];//precomputed section circle points
    } section;
    Vector prePrePoint, prePoint, point;
    float prePreRadius, preRadius, radius;
    Matrix prePreMatrix, preMatrix, matrix;
    MultiPointLine()
        : prePreRadius(0)
        , preRadius(0)
        , radius(0)
    {
        for(unsigned int i=0; i<Reso+1; i++){
            section.x[i] = sinf(Pi * 2 * i / Reso);
            section.y[i] = cosf(Pi * 2 * i / Reso);
        }
    }
    template<class QuadAdaptor>
    void draw(QuadAdaptor& quad, const Matrix& m1, const Matrix& m2, float r1, float r2)
    {
        for(unsigned int i=0; i<Reso; i++){
            Vector p4 = m1.out(Vector(section.x[i], section.y[i], 0)*r1);
            Vector p3 = m1.out(Vector(section.x[i+1], section.y[i+1], 0)*r1);
            Vector p2 = m2.out(Vector(section.x[i+1], section.y[i+1], 0)*r2);
            Vector p1 = m2.out(Vector(section.x[i], section.y[i], 0)*r2);
            quad(p1, p2, p3, p4);
        }
    }
    template<class QuadAdaptor>
    void advance(QuadAdaptor& quad, const Vector& Point, float Radius)
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
                draw(quad, prePreMatrix, preMatrix, prePreRadius, preRadius);
            }
        }
    }
    template<class QuadAdaptor>
    void finish(QuadAdaptor& quad)
    {
        if(preRadius){
            matrix = matrixFromOriginAxisZ(point, unit(point-prePoint), preMatrix.axes.y);
            draw(quad, preMatrix, matrix, preRadius, radius);
        }
    }
};
