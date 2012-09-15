#version 130

smooth out vec4 myColor;
smooth out vec3 vNor;
smooth out vec3 vPos;

void main()
{
	gl_Position = ftransform();
	vNor = gl_NormalMatrix * gl_Normal;
	myColor = gl_Color;
	vPos = vec3(gl_Position);
}
