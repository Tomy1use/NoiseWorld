#pragma once

#include <Math/Vector.h>
#include <Color.h>

static inline Color colorFromNormal(const Vector& v)
{
    return Color((unsigned char)(v.x*127+127), (unsigned char)(v.y*127+127), (unsigned char)(v.z*127+127));
}

