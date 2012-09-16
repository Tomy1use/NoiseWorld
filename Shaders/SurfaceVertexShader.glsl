#version 130

flat out vec4 myColor;
smooth out float shade;
smooth out float fogFactor;
smooth out vec2 cloudsTex;

uniform vec3 vLightDir;
uniform vec3 boxMin, boxMax;//patch bounds
uniform vec3 cloudsBoxMin, cloudsBoxMax;

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
	
	
	float ds = .01, dt = .01;//TODO: calculate these to get plus/minus 1 texel
	float dx = ds * (boxMax.x - boxMin.x) / (boxMax.y - boxMin.y);
	float dz = dt * (boxMax.z - boxMin.z) / (boxMax.y - boxMin.y);
	float hx1 = texture2D(heightMap, vec2(s-ds,t)).x;
	float hx2 = texture2D(heightMap, vec2(s+ds,t)).x;
	float hz1 = texture2D(heightMap, vec2(s,t-dt)).x;
	float hz2 = texture2D(heightMap, vec2(s,t+dt)).x;
	vec3 N = cross(vec3(0,hz2-hz1,2*dz), vec3(2*dx,hx2-hx1,0));
	vec3 vNor = gl_NormalMatrix * N;
	
	cloudsTex.s = (x - cloudsBoxMin.x) / (cloudsBoxMax.x - cloudsBoxMin.x);
	cloudsTex.t = (z - cloudsBoxMin.z) / (cloudsBoxMax.z - cloudsBoxMin.z);
	
	vec4 matColor = y < 300 ? vec4(0,.6,0,1) : vec4(.6,.6,.6,1);
	float f = .4 - .4 * dot(normalize(vLightDir), normalize(vNor));
	myColor = matColor;
	shade = f;
	fogFactor = clamp(30000.f / length(inView), 0, 1);
}
