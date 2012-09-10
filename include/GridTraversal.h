#pragma once

struct GridTraversal
{
    int MinX, MinY, MaxX, MaxY, begX, begY;
    int Lx, Rx, Ty, By;
    int x, y;
    GridTraversal(int MinX, int MinY, int MaxX, int MaxY, int BegX, int BegY)
        : MinX(MinX), MinY(MinY), MaxX(MaxX), MaxY(MaxY)
    {
        x = Lx = Rx = begX = minimum(MaxX-1, maximum(MinX, BegX));
        y = By = Ty = begY = minimum(MaxY-1, maximum(MinY, BegY));
    }
    void operator ++()
    {
        if(Rx < MaxX-1){
            x = ++Rx;
        }else if(Lx > MinX){
            x = --Lx;
        }else{
            if(Ty < MaxY-1){
                y = ++Ty;
            }else if(By > MinY){
                y = --By;
            }else{
                return;//done
            }
            x = Lx = Rx = begX;
        }
    }
    operator bool()
    {
        return Lx > MinX || Rx < MaxX-1 || By > MinY || Ty < MaxY-1;
    }
};
