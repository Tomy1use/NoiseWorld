#pragma once

#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Render/Raster.h>

template<class Vertex, class VertexShader, class PixelShader, class Material, class Fragment, class Projection>
struct Renderer
{
    Raster<Fragment>& raster;
    PixelShader& pixelShader;
    VertexShader& vertexShader;
    const Matrix& viewMatrix;
    const Projection& projection;
    const Frustum& frustum;
    
    Renderer(Raster<Fragment>& raster, PixelShader& pixelShader, VertexShader& vertexShader, 
                    const Matrix& viewMatrix, const Projection& projection, const Frustum& frustum)
        : raster(raster)
        , pixelShader(pixelShader)
        , vertexShader(vertexShader)
        , viewMatrix(viewMatrix)
        , projection(projection)
        , frustum(frustum)
    {
    }
    
    void drawClippedPolygon(unsigned int pointCount, Vertex points[], const Material& material)
    {
        raster.pointCount = pointCount;
        for(unsigned int i=0; i<pointCount; i++){
            projection(points[i], raster.points[i].x, raster.points[i].y, raster.points[i].fragment.w);
            vertexShader(points[i], raster.points[i].fragment);
        }
        raster.calculate();
        if(! raster.isDegenerate()){
            if(raster.isWise()){
                for(int y=raster.min.y; y<raster.max.y; y++){
                    RasterPoint<Fragment>& left = raster.left[y];
                    RasterPoint<Fragment>& right = raster.right[y];
                    Fragment f = left.fragment;
                    Fragment df = (right.fragment - left.fragment) / (right.x - left.x);
                    for (int x=left.x; x<right.x; x++){
                        pixelShader(x, y, f, material);
                        f += df;
                    }
                }
            }
        }
    }
    
    void drawQuad(const Vertex& A, const Vertex& B, const Vertex& C, const Vertex& D, const Material& material)
    {
        unsigned int pointCount = 4;
        Vertex points[5][9] = {{A, B, C, D}};
        for(unsigned int i=0; i<pointCount; i++){
            points[0][i].p = viewMatrix.in(points[0][i].p);
        }
        for(unsigned int i=0; i<4; i++){
            pointCount = clipPolyByPlane(pointCount, points[i], frustum.planes[i], points[i+1]);
            if(pointCount == 0){
                return;
            }
        }
        drawClippedPolygon(pointCount, points[4], material);
    }
    
    void drawTrigon(const Vertex& A, const Vertex& B, const Vertex& C, const Material& material)
    {
        unsigned int pointCount = 3;
        Vertex points[5][9] = {{A, B, C}};
        for(unsigned int i=0; i<pointCount; i++){
            points[0][i].p = viewMatrix.in(points[0][i].p);
        }
        for(unsigned int i=0; i<4; i++){
            pointCount = clipPolyByPlane(pointCount, points[i], frustum.planes[i], points[i+1]);
            if(pointCount == 0){
                return;
            }
        }
        drawClippedPolygon(pointCount, points[4], material);
    }
};
