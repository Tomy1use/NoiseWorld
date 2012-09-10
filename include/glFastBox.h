#pragma once

#include <Math/Vector.h>

void glFastBox(float L, float B, float K, float R, float T, float F);
void glFastBox(const Vector& Min, const Vector& Max);
void glFastBox(const Vector& C, float R);
