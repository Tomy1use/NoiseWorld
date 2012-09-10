#version 130

smooth out float strength;

void main()
{
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	
	vec3 vNor = normalize(gl_NormalMatrix * gl_Normal);
	strength = vNor.z * 2;
}
