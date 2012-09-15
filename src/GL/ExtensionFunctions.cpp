#include <gl/ExtensionFunctions.h>
#include <stdio.h>

template<typename Proc>
static bool resolve(Proc& proc, const char* Name)
{
    proc = Proc(wglGetProcAddress(Name));
    printf("%s %x\n", Name, int(proc));
    return !!proc;
}

PFNGLBEGINOCCLUSIONQUERYNVPROC glBeginOcclusionQuery = NULL;
PFNGLENDOCCLUSIONQUERYNVPROC glEndOcclusionQuery = NULL;
PFNGLGETOCCLUSIONQUERYIVNVPROC glGetOcclusionQueryiv = NULL;
PFNGLGENOCCLUSIONQUERIESNVPROC glGenOcclusionQueries = NULL;
PFNGLDELETEOCCLUSIONQUERIESNVPROC glDeleteOcclusionQueries = NULL;
PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERARBPROC glBindBuffer = NULL;
PFNGLBUFFERDATAARBPROC glBufferData = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffers = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffers = NULL;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbuffer = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorage = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = NULL;
PFNGLTEXIMAGE3DPROC glTexImage3D = NULL;

void resolveExtensionFunctions()
{
	resolve(glBeginOcclusionQuery, "glBeginOcclusionQueryNV");
    resolve(glEndOcclusionQuery, "glEndOcclusionQueryNV");
    resolve(glGetOcclusionQueryiv, "glGetOcclusionQueryivNV");
    resolve(glGenOcclusionQueries, "glGenOcclusionQueriesNV");
    resolve(glDeleteOcclusionQueries, "glDeleteOcclusionQueriesNV");
	resolve(glGenBuffers, "glGenBuffersARB");
	resolve(glBindBuffer, "glBindBufferARB");
	resolve(glBufferData, "glBufferDataARB");
	resolve(glActiveTexture, "glActiveTexture");
    resolve(glGenRenderbuffers, "glGenRenderbuffersEXT");
    resolve(glDeleteRenderbuffers, "glDeleteRenderbuffersEXT");
    resolve(glBindRenderbuffer, "glBindRenderbufferEXT");
    resolve(glRenderbufferStorage, "glRenderbufferStorageEXT");
    resolve(glGenFramebuffers, "glGenFramebuffersEXT");
    resolve(glDeleteFramebuffers, "glDeleteFramebuffersEXT");
    resolve(glBindFramebuffer, "glBindFramebufferEXT");
    resolve(glFramebufferTexture2D, "glFramebufferTexture2D");
    resolve(glCheckFramebufferStatus, "glCheckFramebufferStatus");
    resolve(glFramebufferRenderbuffer, "glFramebufferRenderbufferEXT");
	resolve(glCompileShader, "glCompileShader");
	resolve(glGetShaderiv, "glGetShaderiv");
	resolve(glGetShaderInfoLog, "glGetShaderInfoLog");
	resolve(glGetProgramiv, "glGetProgramiv");
	resolve(glGetProgramInfoLog, "glGetProgramInfoLog");
	resolve(glCreateProgram, "glCreateProgram");
	resolve(glCreateShader, "glCreateShader");
	resolve(glShaderSource, "glShaderSource");
	resolve(glAttachShader, "glAttachShader");
	resolve(glLinkProgram, "glLinkProgram");
	resolve(glUseProgram, "glUseProgram");
	resolve(glGetUniformLocation, "glGetUniformLocation");
	resolve(glUniform3f, "glUniform3f");
	resolve(glUniformMatrix4fv, "glUniformMatrix4fv");
	resolve(glUniform1i, "glUniform1i");
	resolve(glProgramParameteri, "glProgramParameteri");
	resolve(glTexImage3D, "glTexImage3D");
}
