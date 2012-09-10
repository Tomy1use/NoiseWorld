#pragma once

#include <error.h>

template<class Point>
unsigned int clipPolyByPlane(unsigned int pointCount, const Point points[], const Plane& plane, Point outPoints[])
{
    if(pointCount == 0){
        return 0;
    }
    unsigned int outPointCount = 0;
    unsigned int u = 0, v;
    float distU = distance(plane, points[u]), distV;
    if(distU < 0){
        while(u < pointCount){
            v = (u+1) % pointCount;//??? works only once? special case?
            distV = distance(plane, points[v]);
            if(distV > 0){//neg 2 pos
                float f = -distU / (distV - distU);
                outPoints[outPointCount++] = lerp(points[u], points[v], f);
                distU = distV;
                u ++;
                while(u < pointCount){//??? till neg?
                    outPoints[outPointCount++] = points[u];
                    v = (u+1) % pointCount;//??? works only once? special case?
                    distV = distance(plane, points[v]);
                    if(distV < 0){//pos 2 neg
                        float f = -distU / (distV - distU);
                        outPoints[outPointCount++] = lerp(points[u], points[v], f);
                        return outPointCount;
                    }//pos to neg
                    distU = distV;
                    u ++;
                }//pos till count
                //!!! SHOULD NOT BE HERE !!!
                error("clipPointByPlane: should not be here.");
            }//neg to pos
            distU = distV;
            u ++;
        }//neg till count
        return 0;
    }else{
        while (u < pointCount){
            outPoints[outPointCount++] = points[u];
            v = (u+1) % pointCount;//??? works only once? special case?
            distV = distance(plane, points[v]);
            if (distV < 0){//pos 2 neg
                float f = -distU / (distV - distU);
                outPoints[outPointCount++] = lerp(points[u], points[v], f);
                distU = distV;
                u ++;
                while (u < pointCount){//??? till pos?
                    v = (u+1) % pointCount;//??? works only once? special case?
                    distV = distance(plane, points[v]);
                    if (distV > 0){//neg 2 pos
                        float f = -distU / (distV - distU);
                        outPoints[outPointCount++] = lerp(points[u], points[v], f);
                        distU = distV;
                        u ++;
                        while(u < pointCount){
                            outPoints[outPointCount++] = points[u];
                            u ++;
                        }//pos till count
                        return outPointCount;
                    }//neg to pos
                    distU = distV;
                    u ++;
                }//neg till count
                //!!! SHOULD NOT BE HERE !!!
                error("clipPolyByPlane: should not be here.");
            }//pos to neg
            distU = distV;
            u ++;
        }//pos till count
        return outPointCount;
    }//first point positive
}
