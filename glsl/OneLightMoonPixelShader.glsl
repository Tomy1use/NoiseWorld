#version 130

uniform sampler3D MoonMap;

smooth in vec4 myColor;
smooth in vec3 myTex;

float curve(float f)
{
	float x1 = 0.45;
	float x2 = 0.5;
	float y0 = 0.3;
	float y1 = 0.45;
	float y2 = 0.6;
	float y3 = 0.85;
	if(f < x1) return f / x1 * (y1-y0) + y0;
	if(f < x2) return (f - x1) / (x2 - x1) * (y2 - y1) + y1;
	return (f - x2) / (1 - x2) * (y3 - y2) + y2;
}

void main()
{
	//gl_FragColor = gl_Color;
	gl_FragColor = myColor;
	if (! gl_FrontFacing){
		gl_FragColor = 1.0 - gl_FragColor;
	}	
	float f = texture3D(MoonMap, myTex).x;
	gl_FragColor *= curve(f);
}
