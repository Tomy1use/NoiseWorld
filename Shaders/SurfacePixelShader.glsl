#version 130

flat in vec4 myColor;
smooth in float shade;
smooth in float fogFactor;
smooth in vec2 cloudsTex;

uniform sampler2D cloudsMap;

void main()
{
	gl_FragColor = myColor;
	if (! gl_FrontFacing){
		gl_FragColor = 1.0 - gl_FragColor;
	}
	float k = texture2D(cloudsMap, cloudsTex).w;
	k = k < .2 ? k/.2 : 1;
	k = (1-k) * .2 + .8;
	k *= shade;
	gl_FragColor.xyz *= k;
	gl_FragColor = mix(vec4(.8,.8,1,1), gl_FragColor, fogFactor);
}

