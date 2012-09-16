#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/GL.h>
#include <GL/ExtensionFunctions.h>
#include <GL/ShaderFunctions.h>
#include <glFunc.h>
#include <PerlinNoise.h>
#include <Math/MathFunc.h>
#include <Math/VectorConst.h>
#include <Math/MatrixConst.h>
extern GLuint subdivideIcosahedronDisplayList(int depth);
extern GLuint createAtmosphereDisplayList();

static int moonShader = 0;
static GLuint moonTexture = 0;
static GLuint moonDispList = 0;
static GLuint subIcoDispList2 = 0;
static GLuint subIcoDispList3 = 0;
static GLuint additiveShinyShader = 0;
static GLuint atmoDispList = 0;

void createMoonTexture()
{
	PerlinNoise per;
	const int TexReso = 32;
	float* texMap = new float[TexReso*TexReso*TexReso];
	float minT=100, maxT=-100;
	for(int z=0; z<TexReso; z++){
		for(int y=0; y<TexReso; y++){
			for(int x=0; x<TexReso; x++){
				float f = float(rand()) / float(RAND_MAX);
				float X = float(x)/float(TexReso), Y = float(y)/float(TexReso), Z = float(z)/float(TexReso);
				float t = per(X*32, Y*32, Z*32) / 32 + per(X*15, Y*16, Z*16) / 16 + per(X*8, Y*8, Z*8) / 8 + per(X*4, Y*4, Z*4) / 4;
				texMap[z*TexReso*TexReso+y*TexReso+x] = t;
				if(t < minT) minT = t;
				if(maxT < t) maxT = t;
			}
		}
	}
	for(int i=0; i<TexReso*TexReso*TexReso; i++){
		texMap[i] = (texMap[i] - minT) / (maxT - minT);
	}
	glGenTextures(1, &moonTexture);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, moonTexture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, TexReso, TexReso, TexReso, 0, GL_LUMINANCE, GL_FLOAT, texMap);
	glDisable(GL_TEXTURE_3D);
	delete[] texMap;
}

void createSkyDome()
{
	subIcoDispList2 = subdivideIcosahedronDisplayList(2);
	subIcoDispList3 = subdivideIcosahedronDisplayList(3);
	moonShader = createShader("Shaders/OneLightMoonVertexShader.glsl", "Shaders/OneLightMoonPixelShader.glsl", NULL);
	additiveShinyShader = createShader("Shaders/AdditiveShinyVertexShader.glsl", "Shaders/AdditiveShinyPixelShader.glsl", NULL);
	createMoonTexture();
	atmoDispList = createAtmosphereDisplayList();
}

void drawMoon(const Matrix& cameraMatrix, const Vector& sunDir)
{
	glCullFace(GL_BACK);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, moonTexture);
	glUseProgram(moonShader);
	setShaderVector(moonShader, "vLightDir", cameraMatrix.axes.in(sunDir));
	setShaderTexture(moonShader, "MoonMap", 0);
	glPushMatrix();
	glTranslate(Vector(-10, 3, 7));
		glColor3f(.3f, .3f, .3f);
		glCallList(subIcoDispList3);
	glPopMatrix();
	glDisable(GL_TEXTURE_3D);
}

void drawSun(const Vector& sunDir)
{
	glCullFace(GL_BACK);
	glUseProgram(additiveShinyShader);
	glPushMatrix();
		glTranslate(-sunDir * 20);
		glColor3f(.3f, .3f, 0);
		glCallList(subIcoDispList2);
	glPopMatrix();
}

void drawAtmosphere()
{
	glUseProgram(0);
	glDisable(GL_FOG);
	glCullFace(GL_FRONT);
	glShadeModel(GL_SMOOTH);
	glPushMatrix();
		glScalef(5,5,5);
		glCallList(atmoDispList);
	glPopMatrix();
}

void drawHighAltitudeClouds()
{
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	extern GLuint cloudsTexture;
	glBindTexture(GL_TEXTURE_2D, cloudsTexture);
	glDisable(GL_FOG);
	glShadeModel(GL_SMOOTH);
	glColor4f(1,1,1,.5f);
	glPushMatrix();
		glScalef(55,3,55);
		//draw canopy
		const int ResoX = 9, ResoZ = 9;
		for(int j=0; j<ResoZ-1; j++){
			float v1 = float(j) / float(ResoZ-1);
			float v2 = float(j+1) / float(ResoZ-1);
			float z1 = lerp(-1.f, 1.f, v1);
			float z2 = lerp(-1.f, 1.f, v2);
			glBegin(GL_QUAD_STRIP);
				for(int i=0; i<ResoX; i++){
					float u = float(i) / float(ResoX-1);
					float x = lerp(-1.f, 1.f, u);
					float y1 = (1-x*x) * (1-z1*z1);
					float y2 = (1-x*x) * (1-z2*z2);
					glTexCoord2f(u, v1);
					glVertex3f(x, y1, z1);
					glTexCoord2f(u, v2);
					glVertex3f(x, y2, z2);
				}
			glEnd();
		}
	glPopMatrix();
}

void drawSkyDome(const Matrix& cameraMatrix, const Vector& sunDir)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrix(Matrix(cameraMatrix.axes, ZeroVector).in(UnitMatrix));
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	drawMoon(cameraMatrix, sunDir);
	drawSun(sunDir);
	drawAtmosphere();
	drawHighAltitudeClouds();
}
