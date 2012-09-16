#version 130

flat in vec4 myColor;
smooth in float fogFactor;

void main()
{
	gl_FragColor = gl_Color;
	gl_FragColor = myColor;
	
	if (! gl_FrontFacing){
		gl_FragColor = 1.0 - gl_FragColor;
	}
	
	gl_FragColor = mix(vec4(.8,.8,1,1), gl_FragColor, fogFactor);
}

