#pragma once

#include <Math/Vector.h>

GLuint createShader(const char* VertexShaderFileName, const char* PixelShaderFileName, const char* GeometryShaderFileName);

void setShaderVector(int program, const char* param, const Vector& v);
void setShaderTexture(int program, const char* param, int texUnitNumber);
