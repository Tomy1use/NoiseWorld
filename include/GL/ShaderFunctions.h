#pragma once

#include <Math/Vector.h>

void setShaderVector(int program, const char* param, const Vector& v);
void setShaderTexture(int program, const char* param, int t);
