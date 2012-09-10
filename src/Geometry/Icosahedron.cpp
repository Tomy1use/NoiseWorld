#include <assert.h>
#include <math.h>
#include <Geometry/Icosahedron.h>
#include <Math/Pi.h>
#include <Math/VectorFunc.h>

Icosahedron::Icosahedron()
{
    const float L = 1;//length of edge
    const float K = L * .866f / 2;//half-height of triangle
    float a1 = 0, a2 = 2*Pi/5, a3 = 2*2* Pi/5, a4 = 3*2*Pi/5, a5 = 4*2*Pi/5;
    float c1 = cosf(a1), c2 = cosf(a2), c3 = cosf(a3), c4 = cosf(a4), c5 = cosf(a5);
    float s1 = sinf(a1), s2 = sinf(a2), s3 = sinf(a3), s4 = sinf(a4), s5 = sinf(a5);
    points[A] = unit(Vector(+c1, +K, +s1));
    points[B] = unit(Vector(+c2, +K, +s2));
    points[C] = unit(Vector(+c3, +K, +s3));
    points[D] = unit(Vector(+c4, +K, +s4));
    points[E] = unit(Vector(+c5, +K, +s5));
    points[F] = unit(Vector(-c1, -K, -s1));
    points[G] = unit(Vector(-c2, -K, -s2));
    points[H] = unit(Vector(-c3, -K, -s3));
    points[I] = unit(Vector(-c4, -K, -s4));
    points[J] = unit(Vector(-c5, -K, -s5));
    float R = length(points[A] - points[F]) / 2;
    points[U] = unit(Vector(0, +R, 0));
    points[V] = unit(Vector(0, -R, 0));
}

Vector Icosahedron::trigonPoint(unsigned int t, unsigned int p)
{
    assert(t < TrigonCount);
    assert(p < 3);
    return points[trigonPointIndex(t, p)];
}

int Icosahedron::trigonPointIndex(unsigned int t, unsigned int p)
{
    static const unsigned int TrigonPointIndex[TrigonCount][3] = 
    {
        {A, I, H}, {H, E, A}, {E, H, G}, {G, D, E}, {D, G, F},
        {F, C, D}, {C, F, J}, {J, B, C}, {B, J, I}, {I, A, B},
        {U, B, A}, {U, C, B}, {U, D, C}, {U, E, D}, {U, A, E}, 
        {V, F, G}, {V, G, H}, {V, H, I}, {V, I, J}, {V, J, F}
    };
    return TrigonPointIndex[t][p];
}
