#pragma once

namespace cg
{
    struct Parameter
    {
        CGparameter parameter;
        Parameter(CGprogram program, const char* Name)
        {
            parameter = cgGetNamedParameter(program, Name);
            if(! parameter) printf("Failed get named parameter %s\n", Name);
        }
        operator CGparameter() const {return parameter;}
        void set(float x, float y, float z) {
			cgGLSetParameter3f(parameter, x, y, z);
		}
        void operator = (const Vector& v) {
			set(v.x, v.y, v.z);
		}
    };
	
	struct MatrixParameter: Parameter
	{
		MatrixParameter(CGprogram program, const char* Name): Parameter(program, Name) {}
		void operator = (const float* matrix){
			cgSetMatrixParameterfc(parameter, matrix);
		}
	};
	
	struct TextureParameter: Parameter
	{
		TextureParameter(CGprogram program, const char* Name): Parameter(program, Name) {}
		void operator = (const GLuint texture) {
			cgGLEnableTextureParameter(parameter);
			cgGLSetTextureParameter(parameter, texture);
		}
	};
    
    struct Program
    {
        CGprogram program;
        Program(const char* file, CGcontext context, CGprofile profile, const char* entry="main")
        {
			program = cgCreateProgramFromFile(context, CG_SOURCE, file, profile, entry, NULL);
            if(program) cgGLLoadProgram(program);
			else printf("Failed load program from %s\n", file);
        }
        operator CGprogram() {return program;}
    };
    
    void enable(CGprofile profile) {cgGLEnableProfile(profile);}
    void disable(CGprofile profile) {cgGLDisableProfile(profile);}
    void bind(CGprogram program) {cgGLBindProgram(program);}
}
