#version 130

uniform vec3 vLightDir;
uniform mat4 ViewToLightMatrix;

smooth out vec4 myColor;
smooth out vec4 shaPos;

void main()
{
	
	gl_Position = ftransform();
	
	shaPos = ViewToLightMatrix * gl_ModelViewMatrix * gl_Vertex;
	
	vec3 vNor = gl_NormalMatrix * gl_Normal;
	float f = .5 - .5 * dot(normalize(vLightDir), normalize(vNor));
	gl_FrontColor = gl_Color * f;
	myColor = gl_Color * f;
}
