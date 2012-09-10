 #version 120
 #extension GL_ARB_geometry_shader4 : enable
 #extension GL_EXT_gpu_shader4 : enable
 
 
 void emitVertex(vec4 v)
 {
	gl_Position = gl_ModelViewProjectionMatrix * v;
	//gl_Position = v;
	EmitVertex();
 }
 
 void main()
 {
	vec4 v0 = gl_PositionIn[0], v1 = gl_PositionIn[1], v2 = gl_PositionIn[2];
	vec4 dx = (v1 - v0) / 6, dz = (v2 - v0) / 6;
	for(int z=0; z<6; z++){
		for(int x=0; x<6; x++){
			emitVertex(v0 + x * dx + z * dz);
			emitVertex(v0 + x*dx + (z+1)*dz);
			emitVertex(v0 + (x+1)*dx + z*dz);
			EndPrimitive();
		}
	}
 }
 