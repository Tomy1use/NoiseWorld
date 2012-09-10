#pragma once

#include <Math/Vector.h>

template<class TrigonAdaptor>
void sphere(TrigonAdaptor& trigon, const Vector& center, float Radius, float Reso)
{
    int tierCount = max(3, int(Pi / Reso));
    {//caps
        float u1 = lerp(-Pi/2, +Pi/2, 1.f / float(tierCount));
        float u2 = lerp(-Pi/2, +Pi/2, float(tierCount-1) / float(tierCount));
        float r = cosf(u1);
        float y1 = sinf(u1), y2 = sinf(u2);
        int segCount = max(3, int(2 * Pi * r / Reso));
        for(int i=0; i<segCount; i++){
            float u = 2 * Pi * i / segCount;
            float v = 2 * Pi * (i+1) / segCount;
            float Ux = cosf(u) * r, Uz = sinf(u) * r;
            float Vx = cosf(v) * r, Vz = sinf(v) * r;
            trigon(Vector(0,-1,0)*Radius+center, Vector(Ux,y1,Uz)*Radius+center, Vector(Vx,y1,Vz)+center);
            trigon(Vector(0,+1,0)*Radius+center, Vector(Vx,y2,Vz)*Radius+center, Vector(Ux,y2,Uz)+center);
        }
    }
    for(int i=1; i<tierCount-1; i++){
        float u1 = lerp(-Pi/2, +Pi/2, float(i) / float(tierCount));
        float u2 = lerp(-Pi/2, +Pi/2, float(i+1) / float(tierCount));
        float y1 = sinf(u1);
        float y2 = sinf(u2);
        float r1 = cosf(u1);
        float r2 = cosf(u2);
        int segCount1 = max(3, int(2 * Pi * r1 / Reso));
        int segCount2 = max(3, int(2 * Pi * r2 / Reso));
        float dv1 = 2 * Pi / float(segCount1);
        float dv2 = 2 * Pi / float(segCount2);
        float v1 = 0;
        float v2 = 0;
        Vector a1(cosf(v1)*r1, y1, sinf(v1)*r1);
        Vector b1(a1);
        Vector a2(cosf(v2)*r2, y2, sinf(v2)*r2);
        Vector b2(a2);
        for(int k=0; k<segCount1 + segCount2; k++){
            if((v1 + dv1) < (v2 + dv2)){
                v1 += dv1;
                a1 = b1;
                b1 = Vector(cosf(v1)*r1, y1, sinf(v1)*r1);
                trigon(b1*Radius+center, a1*Radius+center, b2*Radius+center);
            }else{
                v2 += dv2;
                a2 = b2;
                b2 = Vector(cosf(v2)*r2, y2, sinf(v2)*r2);
                trigon(a2*Radius+center, b2*Radius+center, b1*Radius+center);
            }
        }
    }
}
