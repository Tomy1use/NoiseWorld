#pragma once

#include <BoxSide.h>

void glBox(float L, float B, float K, float R, float T, float F, int sides)
{
    glBegin(GL_QUADS);
        if(sides & LefSide){
            glNormal3f(-1, 0, 0);
            glTexCoord2f(B, F); glVertex3f(L, B, K); 
            glTexCoord2f(B, K); glVertex3f(L, B, F); 
            glTexCoord2f(T, K); glVertex3f(L, T, F); 
            glTexCoord2f(T, F); glVertex3f(L, T, K);
        }
        if(sides & BotSide){
            glNormal3f(0, -1, 0);
            glTexCoord2f(R, K); glVertex3f(R, B, K);
            glTexCoord2f(R, F); glVertex3f(R, B, F); 
            glTexCoord2f(L, F); glVertex3f(L, B, F); 
            glTexCoord2f(L, K); glVertex3f(L, B, K);
        }
        if(sides & BckSide){
            glNormal3f(0, 0, -1);
            glTexCoord2f(R, B); glVertex3f(R, B, K); 
            glTexCoord2f(L, B); glVertex3f(L, B, K); 
            glTexCoord2f(L, T); glVertex3f(L, T, K); 
            glTexCoord2f(R, T); glVertex3f(R, T, K);
        }
        if(sides & RigSide){
            glNormal3f(+1, 0, 0);
            glTexCoord2f(B, F); glVertex3f(R, B, F); 
            glTexCoord2f(B, K); glVertex3f(R, B, K); 
            glTexCoord2f(T, K); glVertex3f(R, T, K); 
            glTexCoord2f(T, F); glVertex3f(R, T, F);
        }
        if(sides & TopSide){
            glNormal3f(0, +1, 0);
            glTexCoord2f(L, F); glVertex3f(L, T, F); 
            glTexCoord2f(R, F); glVertex3f(R, T, F); 
            glTexCoord2f(R, K); glVertex3f(R, T, K);
            glTexCoord2f(L, K); glVertex3f(L, T, K);
        }
        if(sides & FrnSide){
            glNormal3f(0, 0, +1);
            glTexCoord2f(L, B); glVertex3f(L, B, F); 
            glTexCoord2f(R, B); glVertex3f(R, B, F); 
            glTexCoord2f(R, T); glVertex3f(R, T, F); 
            glTexCoord2f(L, T); glVertex3f(L, T, F);
        }
    glEnd();
}


void glBox(const Matrix& M, float L, float B, float K, float R, float T, float F, int sides)
{
    glBegin(GL_QUADS);
        if(sides & LefSide){
            glNormal(M.axes.out(-UnitVectorX));
            glTexCoord2f(B, F); glVertex(M.out(Vector(L, B, K))); 
            glTexCoord2f(B, K); glVertex(M.out(Vector(L, B, F))); 
            glTexCoord2f(T, K); glVertex(M.out(Vector(L, T, F))); 
            glTexCoord2f(T, F); glVertex(M.out(Vector(L, T, K)));
        }
        if(sides & BotSide){
            glNormal(M.axes.out(-UnitVectorY));
            glTexCoord2f(R, K); glVertex(M.out(Vector(R, B, K)));
            glTexCoord2f(R, F); glVertex(M.out(Vector(R, B, F))); 
            glTexCoord2f(L, F); glVertex(M.out(Vector(L, B, F))); 
            glTexCoord2f(L, K); glVertex(M.out(Vector(L, B, K)));
        }
        if(sides & BckSide){
            glNormal(M.axes.out(-UnitVectorZ));
            glTexCoord2f(R, B); glVertex(M.out(Vector(R, B, K))); 
            glTexCoord2f(L, B); glVertex(M.out(Vector(L, B, K))); 
            glTexCoord2f(L, T); glVertex(M.out(Vector(L, T, K))); 
            glTexCoord2f(R, T); glVertex(M.out(Vector(R, T, K)));
        }
        if(sides & RigSide){
            glNormal(M.axes.out(UnitVectorX));
            glTexCoord2f(B, F); glVertex(M.out(Vector(R, B, F))); 
            glTexCoord2f(B, K); glVertex(M.out(Vector(R, B, K))); 
            glTexCoord2f(T, K); glVertex(M.out(Vector(R, T, K))); 
            glTexCoord2f(T, F); glVertex(M.out(Vector(R, T, F)));
        }
        if(sides & TopSide){
            glNormal(M.axes.out(UnitVectorY));
            glTexCoord2f(L, F); glVertex(M.out(Vector(L, T, F))); 
            glTexCoord2f(R, F); glVertex(M.out(Vector(R, T, F))); 
            glTexCoord2f(R, K); glVertex(M.out(Vector(R, T, K)));
            glTexCoord2f(L, K); glVertex(M.out(Vector(L, T, K)));
        }
        if(sides & FrnSide){
            glNormal(M.axes.out(UnitVectorZ));
            glTexCoord2f(L, B); glVertex(M.out(Vector(L, B, F))); 
            glTexCoord2f(R, B); glVertex(M.out(Vector(R, B, F))); 
            glTexCoord2f(R, T); glVertex(M.out(Vector(R, T, F))); 
            glTexCoord2f(L, T); glVertex(M.out(Vector(L, T, F)));
        }
    glEnd();
}
