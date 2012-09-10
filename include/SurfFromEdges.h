#pragma once



struct SurfFromEdges
{
    Vector c1, c2, a1, a2, b1, b2;
    float r1, r2, l1, l2;
    Vector camera;
    Vector a, b, c, d;
    
    SurfType surfType;
    SphereSurf sphere;
    QuadSurf quad;
    CylinderSurf cylinder;
    TorusSurf torus;
    
    void swapEdges()
    {
        swap(r1, r2);
        swap(l1, l2);
        swap(a1, a2);
        swap(b1, b2);
        swap(c1, c2);
    }
    SurfFromEdges(const Edge* e1, const Edge* e2, const Vector& camera)
        : surfType(NoSurfType)
    {
        if(! (e1 && e2)) return;
        c1 = e1->C, c2 = e2->C, a1 = e1->A, a2 = e2->A, b1 = e1->B, b2 = e2->B;
        r1 = e1->R, r2 = e2->R, l1 = e1->L, l2 = e2->L;
        this->camera = camera;
        if(trySphere()) return;
        if(tryQuad()) return;
        if(tryCylinder()) return;
        if(tryTorus()) return;
    }
    bool trySphere()
    {
        if(r1 && r2){//both arcs
            if(c1 == c2){
                Vector k = c1;
                if(r1 == r2){
                    if(a1==a2 || a1==b2 || b1==a2 || b1==b2){//have common point
                        Vector a = a1, b = b1, c = (a1==a2) ? b2 : (a1==b2) ? a2 : (b1==a2) ? b2 : a2;
                        if(dot(a-k, b-k)==0 && dot(b-k, c-k)==0 && dot(c-k, a-k)==0){//not coplanar
                            if(dot(cross(b-a, c-a), camera-a) < 0) swap(a, c);
                            sphere.k = k;
                            sphere.a = a;
                            sphere.b = b;
                            sphere.c = c;
                            sphere.r = r1;
                            sphere.type = surfType = SphereSurfType;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    void doQuad()
    {
        Vector n = cross(b-a, d-a);
        if(dot(n, camera-a) < 0){
            n = -n, swap(a, d), swap(b, c);
        }
        /*
        float as, at, bs, bt, cs, ct, ds, dt;
        switch(maxDim(n)){
            case 0: as=a.y, at=a.z, bs=b.y, bt=b.z, cs=c.y, ct=c.z, ds=d.y, dt=d.z;
            case 1: as=a.z, at=a.x, bs=b.z, bt=b.x, cs=c.z, ct=c.x, ds=d.z, dt=d.x;
            default: as=a.x, at=a.y, bs=b.x, bt=b.y, cs=c.x, ct=c.y, ds=d.x, dt=d.y;
        }
        */
        quad.a = a;
        quad.b = b;
        quad.c = c;
        quad.d = d;
    }
    bool tryQuad()
    {
        if(r1==0 && r2==0){//both lines
            if(a1==a2 || a1==b2 || b1==a2 || b1==b2){//common point
                if(dot(b1-a1, b2-a2) == 0){//perpendicular
                    if(a1 == a2) a = a1, b = b1, d = b2, c = a + (b1-a1) + (b2-a2);
                    if(a1 == b2) a = a1, b = b1, d = a2, c = a + (b1-a1) + (a2-b2);
                    if(b1 == a2) a = a1, b = b1, c = b2, d = a + (b2-a2);
                    if(b1 == b2) a = a1, b = b1, c = a2, d = a + (a2-b2);
                    doQuad();
                    quad.type = surfType = QuadSurfType;
                    return true;
                }
            }else{
                if(l1 == l2){//equal length
                    float f = dot(b1-a1, b2-a2);
                    if(f != 0){//parallel
                        if(f < 0) swap(a2, b2);
                        if(dot(a2-a1, b1-a1)==0 && dot(b2-b1, b1-a1)==0){//project onto each other
                            a = a1, b = b1, c = b2, d = a2;
                            doQuad();
                            quad.type = surfType = QuadSurfType;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    bool tryCylinder()
    {
        if(r1==0 && r2!=0 || r1!=0 && r2==0){//line and arc
            if(r1 != 0) swapEdges();//now edge1 is a line, edge2 is an arc
            if(a1==a2 || a1==b2 || b1==a2 || b1==b2){//common point
                if(dot(b1-a1, b2-a2) == 0){//perpendicular
                    {//make a1 == a2
                        if(a1 == b2) swap(a2, b2);
                        if(b1 == a2) swap(a1, b1);
                        if(b1 == b2) swap(a1, b1), swap(a2, b2);
                    }
                    Vector v = b1 - a1;
                    Vector n = cross(b1-a1, b2-a2);
                    if(dot(n, camera-a1) > 0){
                        a2 += v, b2 += v, c2 += v, v = -v;
                    }
                    cylinder.c1 = c2 + v;
                    cylinder.a1 = a2 + v;
                    cylinder.b1 = b2 + v;
                    cylinder.c2 = c2;
                    cylinder.a2 = a2;
                    cylinder.b2 = b2;
                    cylinder.r = r2;
                    cylinder.type = surfType = CylinderSurfType;
                    return true;
                }
            }
        }
        if(r1 && r2){//both arcs
            float f = dot(b1-a1, b2-a2);
            if(f != 0){//parallel
                if(f < 0) swap(a2, b2);
                if(dot(a2-a1, b1-a1)==0 && dot(b2-b1, b1-a1)==0){//project onto each other
                    if(dot(c2-c1, c1-a1) == 0){//coaxial
                        Vector n = cross(b1-a1, a2-a1);
                        if(dot(n, camera-a1) > 0){
                            swap(a1, a2), swap(b1, b2), swap(c1, c2);
                        }
                        cylinder.c1 = c1;
                        cylinder.a1 = a1;
                        cylinder.b1 = b1;
                        cylinder.c2 = c2;
                        cylinder.a2 = a2;
                        cylinder.b2 = b2;
                        cylinder.r = r1;
                        cylinder.type = surfType = CylinderSurfType;
                        return true;
                    }
                }
            }
        }
        return false;
    }
    bool tryTorus()
    {
        return false;
    }
};
