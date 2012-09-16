#version 130

smooth out vec4 myColor;
smooth out float cutoffValue;
smooth out float fogFactor;

//uniform vec3 vLightDir;
//uniform vec3 boxMin, boxMax;//patch bounds

//uniform sampler2D heightMap;

void main()
{
	vec4 inView = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ProjectionMatrix * inView;
	
	cutoffValue = gl_Color.a;
	
	
	
	/*
	float ds = .01, dt = .01;//TODO: calculate these to get plus/minus 1 texel
	float dx = ds * (boxMax.x - boxMin.x) / (boxMax.y - boxMin.y);
	float dz = dt * (boxMax.z - boxMin.z) / (boxMax.y - boxMin.y);
	float hx1 = texture2D(heightMap, vec2(s-ds,t)).x;
	float hx2 = texture2D(heightMap, vec2(s+ds,t)).x;
	float hz1 = texture2D(heightMap, vec2(s,t-dt)).x;
	float hz2 = texture2D(heightMap, vec2(s,t+dt)).x;
	vec3 N = cross(vec3(0,hz2-hz1,2*dz), vec3(2*dx,hx2-hx1,0));
	vec3 vNor = gl_NormalMatrix * N;
	*/
	
	//vec4 matColor = y < 300 ? vec4(0,.6,0,1) : vec4(.6,.6,.6,1);
	//float f = .4 - .4 * dot(normalize(vLightDir), normalize(vNor));
	
	myColor = gl_Color;
	fogFactor = clamp(30000.f / length(inView), 0, 1);
}
