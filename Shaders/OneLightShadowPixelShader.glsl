#version 130

uniform sampler2D ShadowMap;

smooth in vec4 myColor;
smooth in vec4 shaPos;


void main()
{
	gl_FragColor = gl_Color;
	gl_FragColor = myColor;
	
	if (! gl_FrontFacing){
		gl_FragColor = 1.0 - gl_FragColor;
	}
	
	vec4 s = shaPos / shaPos.w * .5 + .5;
	
	
	
	float dX = 1.0 / 1024.0; // 1 / ShadowTextureWidth
	float dY = 1.0 / 1024.0; // 1 / ShadowTextureHeight
	
	float z1 = texture2D(ShadowMap, vec2(s.x, s.y)).x;
	
	float z2 = texture2D(ShadowMap, vec2(s.x-dX, s.y)).x;
	float z3 = texture2D(ShadowMap, vec2(s.x+dX, s.y)).x;
	float z4 = texture2D(ShadowMap, vec2(s.x, s.y-dY)).x;
	float z5 = texture2D(ShadowMap, vec2(s.x, s.y+dY)).x;
	float z6 = texture2D(ShadowMap, vec2(s.x-dX, s.y-dY)).x;
	float z7 = texture2D(ShadowMap, vec2(s.x-dX, s.y+dY)).x;
	float z8 = texture2D(ShadowMap, vec2(s.x+dX, s.y-dY)).x;
	float z9 = texture2D(ShadowMap, vec2(s.x+dX, s.y+dY)).x;
	
	float Bias = .002;
	
	float s1 = (z1+Bias > s.z) ? 1 : 0;
	
	float s2 = (z2+Bias > s.z) ? 1 : 0;
	float s3 = (z3+Bias > s.z) ? 1 : 0;
	float s4 = (z4+Bias > s.z) ? 1 : 0;
	float s5 = (z5+Bias > s.z) ? 1 : 0;
	float s6 = (z6+Bias > s.z) ? 1 : 0;
	float s7 = (z7+Bias > s.z) ? 1 : 0;
	float s8 = (z8+Bias > s.z) ? 1 : 0;
	float s9 = (z9+Bias > s.z) ? 1 : 0;
	
	float shadow = (s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9) / 9;
	
	gl_FragColor = myColor * (shadow * .6 + .3);
}
