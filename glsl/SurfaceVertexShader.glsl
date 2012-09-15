#version 130

flat out vec4 myColor;
smooth out float fogFactor;

uniform vec3 vLightDir;
uniform vec3 boxMin, boxMax;//patch bounds

uniform sampler2D heightMap;

void main()
{
	float s = gl_Vertex.x;
	float t = gl_Vertex.z;
	float x = boxMin.x + (boxMax.x - boxMin.x) * gl_Vertex.x;
	float z = boxMin.z + (boxMax.z - boxMin.z) * gl_Vertex.z;
	float h = texture2D(heightMap, vec2(s,t)).x;
	float y = h * (boxMax.y - boxMin.y) + boxMin.y;
	vec4 inView = gl_ModelViewMatrix * vec4(x, y, z, 1);
	gl_Position = gl_ProjectionMatrix * inView;
	float hx = texture2D(heightMap, vec2(s+.01,t)).x;
	float hz = texture2D(heightMap, vec2(s,t+.01)).x;
	vec3 N = cross(vec3(0,hz-h,.01), vec3(.01,hx-h,0));
	vec4 matColor = y < 200 ? vec4(0,.6,0,1) : vec4(.6,.6,.6,1);
	vec3 vNor = gl_NormalMatrix * N;
	float f = .5 - .5 * dot(normalize(vLightDir), normalize(vNor));
	myColor = matColor * f;
	fogFactor = clamp(30000.f / length(inView), 0, 1);
}
