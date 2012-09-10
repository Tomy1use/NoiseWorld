#version 130

smooth in vec2 cTex;

void main()
{
	gl_FragColor = gl_Color;
	float k = abs(step(.5, fract(cTex.y/2)) + step(.5, fract(cTex.x/2)) - 1);
	gl_FragColor.xyz = gl_Color.xyz * (.4 + .2 * k);
}
