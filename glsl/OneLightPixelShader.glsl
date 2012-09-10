#version 130

smooth in vec4 myColor;

void main()
{
	gl_FragColor = gl_Color;
	gl_FragColor = myColor;
	
	if (! gl_FrontFacing){
		gl_FragColor = 1.0 - gl_FragColor;
	}
}
