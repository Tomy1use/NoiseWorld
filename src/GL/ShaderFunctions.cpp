#define WIN23_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/ExtensionFunctions.h>
#include <GL/ShaderFunctions.h>

void setShaderVector(int program, const char* param, const Vector& v)
{
	int location = glGetUniformLocation(program, param);
	glUniform3f(location, v.x, v.y, v.z);
}

void setShaderTexture(int program, const char* param, int t)
{
	int location = glGetUniformLocation(program, param);
	glUniform1i(location, t);
}
