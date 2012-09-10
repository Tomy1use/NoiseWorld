#pragma once

void glSkyDome(const Vector& sunDir)
{
    glShadeModel(GL_SMOOTH);
    const Color c1(0xff00ffff), c2(0xff0080ff);
    const int SegmentCount = 33;
    const int SectorCount = 15;
    glBegin(GL_QUAD_STRIP);
    for(int y=-SegmentCount/3; y<SegmentCount-1; y++){
        for(int x=0; x<SectorCount+1; x++){
            float a1 = 2 * Pi * float(x) / float(SectorCount);
            float u1 = Pi/2 * float(y) / float(SegmentCount);
            float u2 = Pi/2 * float(y+1) / float(SegmentCount);
            float r1 = cosf(u1), y1 = sinf(u1);
            float r2 = cosf(u2), y2 = sinf(u2);
            glColor((y+1)<0 ? c1 : lerp(c1, c2, 255*(y+1)/SegmentCount));
            glVertex(Vector(cosf(a1)*r2, y2/2, sinf(a1)*r2));
            glColor(y<0 ? c1 : lerp(c1, c2, 255*y/SegmentCount));
            glVertex(Vector(cosf(a1)*r1, y1/2, sinf(a1)*r1));
        }
    }
    glEnd();
    const Color SunColor(0xffFFffFF);
    const int SunSegCount = 24;
    const float SunRadius = .02f;
    glBegin(GL_TRIANGLE_FAN);
        glColor(SunColor);
        Matrix M = matrixLookAt(-sunDir/2, ZeroVector);
        glVertex(M.out(Vector(0,0,0)*SunRadius));
        for(int i=0; i<=SunSegCount; i++){
            float a = 2 * Pi * i / SunSegCount;
            glVertex(M.out(Vector(cosf(a),-sinf(a),0)*SunRadius));
        }
    glEnd();
}
