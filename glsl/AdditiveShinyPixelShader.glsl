#version 130

smooth in float strength;

void main()
{
	gl_FragColor = gl_Color * strength;
	
	if(strength > 1){
		gl_FragColor.r += strength - 1;
		gl_FragColor.g += strength - 1;
		gl_FragColor.b += strength - 1;
	}
}
