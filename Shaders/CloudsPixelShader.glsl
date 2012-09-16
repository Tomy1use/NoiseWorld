#version 130

smooth in vec4 myColor;
smooth in float cutoffValue;
smooth in float fogFactor;

void main()
{
	if(cutoffValue < 0){
		discard;
	}
	gl_FragColor = mix(vec4(.8,.8,1,1), myColor, fogFactor);
}
