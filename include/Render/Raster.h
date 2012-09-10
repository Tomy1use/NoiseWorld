#pragma once

template<class Fragment>
struct RasterPoint
{
    int x, y;
    Fragment fragment;
    RasterPoint() {}
    RasterPoint(Fragment f): fragment(f) {}
};


template<class Fragment>
struct Raster
{
    int z;//clock- or counter-clockwise
    RasterPoint<Fragment> left[999], right[999];
    RasterPoint<Fragment> min, max;
    RasterPoint<Fragment> points[10];
    int pointCount;
    
    bool isDegenerate() {return (z == 0);}
    bool isWise() {return (z < 0);}
    
    void calcBounds()
    {
        min = max = points[0];
        int leftIndex = 0, rightIndex = 0;
        for (int i=0; i<pointCount; i++){
            if (points[i].y < min.y){
                min.y = points[i].y;
            }
            if (max.y < points[i].y){
                max.y = points[i].y;
            }
            if (points[i].x < min.x){
                min.x = points[i].x;
                leftIndex = i;
            }
            if (max.x < points[i].x){
                max.x = points[i].x;
                rightIndex = i;
            }
        }
        if(min.y >= 999 || max.y >= 999){
            error("Raster does not have enough lines\n");
        }
    }
    
    void counterClockwise()
    {
        calcBounds();
        if (min.y == max.y){
            return;
        }
        if (min.x == max.x){
            return;
        }
        z = -1;//means CCW
        
        for(int i=0; i<pointCount; i++){
            int j = (i+1) % pointCount;//??? works only once? special case?
            if (points[i].y == points[j].y){
                //horizontal
            }else{
                //not horizontal
                int dy = points[j].y - points[i].y;
                if (dy > 0){
                    //down the screen
                    int dx = points[j].x - points[i].x;
                    Fragment df = (points[j].fragment - points[i].fragment) / dy;
                    int dy = points[j].y - points[i].y;
                    int x = points[i].x;
                    Fragment f = points[i].fragment;
                    //float s = points[i].s;
                    //float t = points[i].t;
                    int c = 0;
                    if (dx < 0){
                        dx = -dx;
                        for (int y=points[i].y; y<points[j].y; y++){
                            left[y].x = x;
                            left[y].fragment = f;
                            c += dx;
                            while (c > dy){
                                c -= dy;
                                x --;
                            }
                            f += df;
                        }
                    }else{
                        int c = 0;
                        int x = points[i].x;
                        Fragment f = points[i].fragment;
                        for (int y=points[i].y; y<points[j].y; y++){
                            left[y].x = x;
                            left[y].fragment = f;
                            c += dx;
                            while (c > dy){
                                c -= dy;
                                x ++;
                            }
                            f += df;
                        }
                    }//dx > 0
                }else{
                    //up the screen
                    int dx = points[i].x - points[j].x;
                    Fragment df = (points[j].fragment - points[i].fragment) / dy;
                    int c = 0;
                    int x = points[j].x;
                    Fragment f = points[j].fragment;
                    dy = -dy;
                    if (dx < 0){
                        dx = -dx;
                        for (int y=points[j].y; y<points[i].y; y++){
                            right[y].x = x;
                            right[y].fragment = f;
                            c += dx;
                            while (c > dy){
                                c -= dy;
                                x--;
                            }
                            f += df;
                        }
                    }else{
                        for (int y=points[j].y; y<points[i].y; y++){
                            right[y].x = x;
                            right[y].fragment = f;
                            c += dx;
                            while (c > dy){
                                c -= dy;
                                x++;
                            }
                            f += df;
                        }
                    }
                }//up the screen
            }//not horizontal
        }//for points;
    }
    
    void calculateZ()
    {
        //use Newell's approach to 
        //calculate z component
        //of onscreen polygon's normal 
        //to find out it's clockwiseness
        z = 0;
        for (int i=0; i<pointCount; i++){
            int j = (i+1) % pointCount;
            z += (points[i].x - points[j].x) * (points[i].y + points[j].y);
        }
    }
    
    void calculate()
    {
        calculateZ();
        if (z < 0){
            counterClockwise();
        }
    }
};
