#pragma once 

#include <Surflet.h>

Edge calcArc(const Vector& A, const Vector& P)
{
    Vector V = P - A;
    int i = minDim(V), j = maxDim(V), k = 3 - i - j;
    if(i == j) return Edge();
    float R = abs((k==0) ? V.x : (k==1) ? V.y : V.z);
    if(R <= 0) return Edge();
    Vector C = A + unit(snapToAxis(V, j)) * R;
    Vector B = C + unit(snapToAxis(V, k)) * R;
    return Edge(C, A, B, R);
}

struct Scene
{
    Edge* edges;
    Surf* surfs;
    Scene(): edges(NULL), surfs(NULL)
    {
    }
    ~Scene()
    {
        DeleteList(edges);
        DeleteList(surfs);
    }
    void pop(Edge* e){
        assert(e == edges); 
        if(e) edges = e->next, delete(e);
    }
    void push(Edge* e) {e->next = edges, edges = e;}
    void pop(Surf* s){
        assert(s == surfs);
        if(s) surfs = s->next, delete(s);
    }
    void push(Surf* s) {s->next = surfs, surfs = s;}
    unsigned edgeCount() const {return ListItemCount(edges);}
    unsigned surfCount() const {return ListItemCount(surfs);}
    Edge* edgeBefore(const Edge* e){ return ListItemBefore(e, edges);}
};
