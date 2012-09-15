#version 130

smooth out vec2 cTex;

void main()
{
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	cTex = gl_MultiTexCoord0.xy;
}
