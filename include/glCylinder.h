#pragma once

void glCylinder(const Matrix& M, float BotY, float TopY, float BotR, float TopR, int Reso, int sides=TopSide+BotSide)
{
    glBegin(GL_QUAD_STRIP);
        for(int i=0; i<Reso+1; i++){
            float a = 2 * Pi * i / Reso;
            float x = cosf(a), z = sinf(a);
            glNormal(M.axes.out(Vector(x, (BotR-TopR)/(TopY-BotY), z)));
            glTexCoord2f(a*BotR, BotY);
            glVertex(M.out(Vector(x*BotR, BotY, z*BotR)));
            glTexCoord2f(a*TopR, TopY);
            glVertex(M.out(Vector(x*TopR, TopY, z*TopR)));
        }
    glEnd();
    if(sides & TopSide){
        glBegin(GL_TRIANGLE_FAN);
            glNormal(M.axes.out(UnitVectorY));
            for(int i=0; i<Reso; i++){
                float a = 2 * Pi * i / Reso;
                glTexCoord2f(a*TopR, TopY);
                glVertex(M.out(Vector(cosf(a) * TopR, TopY, -sinf(a) * TopR)));
            }
        glEnd();
    }
    if(sides & BotSide){
        glBegin(GL_TRIANGLE_FAN);
            glNormal(M.axes.out(-UnitVectorY));
            for(int i=0; i<Reso; i++){
                float a = 2 * Pi * i / Reso;
                glTexCoord2f(a*BotR, BotY);
                glVertex(M.out(Vector(cosf(a) * BotR, BotY, +sinf(a) * BotR)));
            }
        glEnd();
    }
}

void glCylinder(const Vector& A, const Vector& B, float RadA, float RadB, int Reso, int sides=TopSide+BotSide)
{
    float h = length(B - A);
    Vector z = (B - A) / h;
    glCylinder(matrixFromOriginAxisY(A, z), 0, h, RadA, RadB, Reso, sides);
}
