#version 130

smooth out vec4 myColor;

uniform vec3 vLightDir;

void main()
{
	gl_Position = ftransform();
	vec3 vNor = gl_NormalMatrix * gl_Normal;
	float f = .5 - .5 * dot(normalize(vLightDir), normalize(vNor));
	gl_FrontColor = gl_Color * f;
	myColor = gl_Color * f;
}
