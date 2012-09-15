#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glext.h>

extern void resolveExtensionFunctions();

extern PFNGLBEGINOCCLUSIONQUERYNVPROC glBeginOcclusionQuery;
extern PFNGLENDOCCLUSIONQUERYNVPROC glEndOcclusionQuery;
extern PFNGLGETOCCLUSIONQUERYIVNVPROC glGetOcclusionQueryiv;
extern PFNGLGENOCCLUSIONQUERIESNVPROC glGenOcclusionQueries;
extern PFNGLDELETEOCCLUSIONQUERIESNVPROC glDeleteOcclusionQueries;
extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLBUFFERDATAARBPROC glBufferData;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffers;
extern PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffers;
extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbuffer;
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorage;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer;
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
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
extern PFNGLTEXIMAGE3DPROC glTexImage3D;
