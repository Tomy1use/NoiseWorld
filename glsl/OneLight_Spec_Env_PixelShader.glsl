#version 130

smooth in vec4 myColor;
smooth in vec3 vNor;
smooth in vec3 vPos;

uniform vec3 vLightDir;
uniform vec3 vX, vY, vZ;

void main()
{
	float k = .3 - .3 * dot(normalize(vLightDir), normalize(vNor));
	gl_FragColor = myColor * k;
	vec3 rNor = reflect(normalize(vPos), normalize(vNor));
	{
		float eX = abs(dot(vX, rNor));
		float eY = abs(dot(vY, rNor));
		float eZ = abs(dot(vZ, rNor));
		if(eX > eY && eX > eZ){
			gl_FragColor.x += 0.1;
		}else if(eY > eZ){
			gl_FragColor.y += 0.1;
		}else{
			gl_FragColor.z += 0.1;
		}
	}
	float f = max(0, dot(rNor, vLightDir));
	f = f * f * f * f * f * f * f;
	gl_FragColor.xyz += f * vec3(.3, .3, .1);
}
