#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glext.h>
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;

struct FileAsString
{
	FILE* f;
	unsigned int length;
	char* text;//length+1
	FileAsString(const char* fileName)
		: f(NULL)
		, length(0)
		, text(NULL)
	{
		f = fopen(fileName, "rb");
		if(! f){
			printf("Failed open file: %s\n", fileName);
			return;
		}
		fseek(f, -1, SEEK_END);
		length = ftell(f);
		if(length < 1){
			printf("File too small: %s\n", fileName);
			return;
		}
		fseek(f, 0, SEEK_SET);
		text = new char[length+1];
		if(1 != fread(text, length, 1, f)){
			printf("Failed read from file: %s\n", fileName);
			return;
		}
		text[length] = 0;
	}
	~FileAsString()
	{
		if(text) delete[] text;
		if(f) fclose(f);
	}
	operator const char* () const {return text;}
};

bool compileAndAttach(GLuint program, int shaderType, const char* srcFileName)
{
	printf("Compile %s\n", srcFileName);
	GLuint shader = glCreateShader(shaderType);
	FileAsString srcText(srcFileName);
	if(srcText.text){
		glShaderSource(shader, 1, &srcText.text, NULL);
		glCompileShader(shader);
		int ok = false;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
		if(ok){
			glAttachShader(program, shader);
			return true;
		}
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0){
			char* logText = new char[infoLogLength+1];
			glGetShaderInfoLog(shader, infoLogLength+1, &infoLogLength, logText);
			logText[infoLogLength] = 0;
			printf(logText);
			delete[] logText;
		}
	}
	return false;
}

GLuint createShaderProgram(const char* vertexShaderFileName, const char* pixelShaderFileName, const char* geometryShaderFileName)
{
	GLuint program = glCreateProgram();
	if(vertexShaderFileName){
		compileAndAttach(program, GL_VERTEX_SHADER, vertexShaderFileName);
	}
	if(pixelShaderFileName){
		compileAndAttach(program, GL_FRAGMENT_SHADER, pixelShaderFileName);
	}
	if(geometryShaderFileName){
		if(compileAndAttach(program, GL_GEOMETRY_SHADER, geometryShaderFileName)){
			glProgramParameteri(program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
			glProgramParameteri(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);
			glProgramParameteri(program, GL_GEOMETRY_VERTICES_OUT_EXT, 6*6*3);
		}
	}
	glLinkProgram(program);
	int ok = false;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	if(ok){
		printf("Program link OK\n");
	}else{
		int infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* logText = new char[infoLogLength+1];
		if(infoLogLength > 0){
			glGetProgramInfoLog(program, infoLogLength+1, &infoLogLength, logText);
			logText[infoLogLength] = 0;
			printf(logText);
			delete[] logText;
		}
	}
	return program;
}
