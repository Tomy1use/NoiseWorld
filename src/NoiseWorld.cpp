#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/GL.h>
#include <GL/glext.h>
#include <glFunc.h>
#include <Math/Vector.h>
#include <Math/VectorConst.h>
#include <Math/VectorFunc.h>
#include <Math/MathFunc.h>
#include <Math/MatrixFunc.h>
#include <Math/RandomNumber.h>
#include <Math/Plane.h>
#include <Math/PlaneFunc.h>
#include <stdio.h>
#include <Color.h>
#include <stdlib.h>
#include <PerlinNoise.h>
extern void glFastBox(float L, float B, float K, float R, float T, float F);
extern void glTexBox(float L, float B, float K, float R, float T, float F);


struct MapPatch
{
	Vector* verts;
	Vector boxMin, boxMax;
	MapPatch* children[4];
};

Vector nearestBoxPoint(const Vector& boxMin, const Vector& boxMax, const Vector& p)
{
	float x = p.x<boxMin.x ? boxMin.x : p.x<boxMax.x ? p.x : boxMax.x;
	float y = p.y<boxMin.y ? boxMin.y : p.y<boxMax.y ? p.y : boxMax.y;
	float z = p.z<boxMin.z ? boxMin.z : p.z<boxMax.z ? p.z : boxMax.z;
	return Vector(x, y, z);
}

/*
	take root patch
	if near enough to viewer:
		if not subdivided:
			if maximum level of detail not reached:
				subdivide it
				create subpatches, fill with landscape data
	else:
		if has children, remove children
*/




float curve(float h)
{
	const float z[] = {0, .1f, .2f, .8f, 1.f};
	const int N = sizeof(z)/sizeof(z[0])-1;
	int i = int(h*N);
	float t = h*N - floorf(h*N);
	return lerp(z[i], z[i+1], t);
}

struct Curve
{
	static const int PointCount = 100;
	float value[PointCount];
	Curve()
	{
		for(int i=0; i<PointCount; i++){
			value[i] = curve(float(i) / float(PointCount-1));
		}
	}
	float operator() (float f) const
	{
		if(f < 0.f) return value[0];
		if(f > 1.f) return value[PointCount-1];
		f *= PointCount-1;
		int i = int(f);
		float t = f - floorf(f);
		return lerp(value[i], value[i+1], t);
	}
};

const Vector MapMin(-15000, 0, -15000);
const Vector MapMax(15000, 0, 15000);

const int MapReso = 1000;
Vector* mapVerts = NULL;
Vector* mapNorms = NULL;
float* mapShade = NULL;
Vector& getMapVert(int x, int z) {return mapVerts[z*MapReso+x];}
Vector& getMapNorm(int x, int z) {return mapNorms[z*MapReso+x];}
float& getMapShade(int x, int z) {return mapShade[z*MapReso+x];}
Curve mapCurve;
bool isCurveDirty = false;
PerlinNoise mapNoise;
int mapDispList = 0;

const Vector CloudsMin(-15000, 2000, -15000), CloudsMax(15000, 2000, 15000);
const int CloudsReso = 500;
float* cloudsMap = NULL;
float& getCloudsPoint(int x, int z) {return cloudsMap[z * CloudsReso + x];}
GLuint cloudsTexture = 0;

void calcMapNormals()
{
	if(! mapNorms) mapNorms = new Vector[MapReso*MapReso];
	for(int z=0; z<MapReso; z++){
		for(int x=0; x<MapReso; x++){
			Vector x1 = x > 0 ? getMapVert(x-1, z) : getMapVert(x, z);
			Vector x2 = x < MapReso-1 ? getMapVert(x+1, z) : getMapVert(x, z);
			Vector z1 = z > 0 ? getMapVert(x, z-1) : getMapVert(x, z);
			Vector z2 = z < MapReso-1 ? getMapVert(x, z+1) : getMapVert(x, z);
			getMapNorm(x, z) = unit(cross(z2-z1, x2-x1));
		}
	}
}

void calcMapShade()
{
	const Vector light = unit(Vector(1, 3, 2));
	if(! mapShade) mapShade = new float[MapReso*MapReso];
	for(int z=0; z<MapReso; z++){
		for(int x=0; x<MapReso; x++){
			getMapShade(x, z) = dot(getMapNorm(x, z), light);
		}
	}
}

void createHeightMap()
{
	if(! mapVerts) mapVerts = new Vector[MapReso*MapReso];
	const float periods[] = {
		5, 15, 35, 75, 
		155, 
		355,
		553,
		1122,
	};
	const int periodCount = sizeof(periods) / sizeof(periods[0]);
	float totalPeriod = 0;
	for(int i=0; i<periodCount; i++) totalPeriod += periods[i];
	float invTotalPeriod = 1.f / totalPeriod;
	for(int z=0; z<MapReso; z++){
		for(int x=0; x<MapReso; x++){
			Vector& v = getMapVert(x, z);
			v.x = lerp(MapMin.x, MapMax.x, float(x)/float(MapReso-1));
			v.z = lerp(MapMin.z, MapMax.z, float(z)/float(MapReso-1));
			float h = 0;
			for(int i=0; i<periodCount; i++){
				h += mapNoise(v.x/periods[i], v.z/periods[i]) * periods[i] * invTotalPeriod;
			}
			h = h * .9f + .5f;
			v.y = mapCurve(h) * 500;
		}
	}
}

void createCloudsMap()
{
	if(! cloudsMap) cloudsMap = new float[CloudsReso*CloudsReso];
	const float periods[] = {
		5, 15, 35, 75, 
		155, 
		355,
		573,
		1021,
	};
	const int periodCount = sizeof(periods) / sizeof(periods[0]);
	float totalPeriod = 0;
	for(int i=0; i<periodCount; i++) totalPeriod += periods[i];
	float invTotalPeriod = 1.f / totalPeriod;
	for(int z=0; z<CloudsReso; z++){
		for(int x=0; x<CloudsReso; x++){
			float Px = lerp(CloudsMin.x, CloudsMax.x, float(x)/float(CloudsReso-1));
			float Pz = lerp(CloudsMin.z, CloudsMax.z, float(z)/float(CloudsReso-1));
			float h = 0;
			for(int i=0; i<periodCount; i++){
				h += mapNoise(Px/periods[i], Pz/periods[i]) * periods[i] * invTotalPeriod;
			}
			getCloudsPoint(x, z) = h * 2;
		}
	}
}

void createCloudsTexture()
{
	glGenTextures(1, &cloudsTexture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, cloudsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, CloudsReso, CloudsReso, 0, GL_ALPHA, GL_FLOAT, cloudsMap);
}

void createMapDisplayList()
{
	mapDispList = glGenLists(1);
	glNewList(mapDispList, GL_COMPILE);
		for(int z=0; z<MapReso-1; z++){
			glBegin(GL_QUAD_STRIP);
				for(int x=0; x<MapReso; x++){
					float s1 = getMapShade(x, z), s2 = getMapShade(x, z+1);
					Vector v1 = getMapVert(x, z), v2 = getMapVert(x, z+1);
					Vector grassColor(0,.5f,0), rockColor(.7f,.7f,.7f);
					glColor(v1.y < 100 ? grassColor*s1 : rockColor*s1);
					glVertex(v1);
					glColor(v2.y<100 ? grassColor*s2 : rockColor*s2);
					glVertex(v2);
				}
			glEnd();
		}
		if(0){
			glDisable(GL_CULL_FACE);
			glColor3f(.5f,0,0);
			glBegin(GL_QUADS);
				glVertex3f(-1000, 0, -1000);
				glVertex3f(-1000, 0,  1000);
				glVertex3f( 1000, 0,  1000);
				glVertex3f( 1000, 0, -1000);
			glEnd();
			glColor3f(0,.5f,.5f);
			glBegin(GL_QUADS);
				glVertex3f(-1000,  200, -1000);
				glVertex3f(-1000,  200,  1000);
				glVertex3f( 1000,  200,  1000);
				glVertex3f( 1000,  200, -1000);
			glEnd();
		}
	glEndList();
}

void CreateWorld()
{
	createHeightMap();	
	calcMapNormals();
	calcMapShade();
	createMapDisplayList();
	createCloudsMap();
	createCloudsTexture();
}

void WorldKey(int i)
{
	if(i == 'r'){
	}
	if(i == 't'){
	}
	if(i == 'f'){
	}
}

void renderPeople()
{
	glColor3f(.1f,.5f,0);
	RandomNumber ran(34523049);
	for(int i=0; i<333; i++){
		int x = (++ran)(0, MapReso);
		int z = (++ran)(0, MapReso);
		Vector v = getMapVert(x, z);
		glFastBox(v.x-.4f, v.y, v.z-.4f, v.x+.4f, v.y+2.f, v.z+.4f);
	}
}

void RenderWorld()
{
	float fogColor[] = {.5f, .5f, 1.f, 1.f};
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, .00005f);
	glShadeModel(GL_FLAT);
	glCallList(mapDispList);
	renderPeople();
	{
		// Cumulus clouds
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, cloudsTexture);
		glDisable(GL_CULL_FACE);
		glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glTexCoord2f(0, 0);
			glVertex3f(CloudsMin.x, CloudsMin.y, CloudsMin.z);
			glTexCoord2f(0, 1);
			glVertex3f(CloudsMin.x, CloudsMin.y,  CloudsMax.z);
			glTexCoord2f(1, 1);
			glVertex3f(CloudsMax.x, CloudsMin.y,  CloudsMax.z);
			glTexCoord2f(1, 0);
			glVertex3f(CloudsMax.x, CloudsMin.y, CloudsMin.z);
		glEnd();
	}
}

void RenderWorldGUI()
{
	return;
	float TopY = 1.f;
	float BotY = 0.f;
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBegin(GL_QUAD_STRIP);
		glColor3f(0,.1f,0);
		for(int i=0; i<Curve::PointCount; i++){
			float x = lerp(-1.f, 1.f, float(i)/float(Curve::PointCount-1));
			glVertex2f(x, lerp(BotY, TopY, mapCurve.value[i]));
			glVertex2f(x, BotY);
		}
	glEnd();
}

bool WorldMouseMove(float x, float y)//x, y in [0..1]
{
	return false;
	if(y < .5f){
		float v = 1.f - y / .5f;
		mapCurve.value[int(x*(Curve::PointCount-1))] = v;
		isCurveDirty = true;
		return true;
	}
	return false;
}

void WorldLeftMouseUp()
{
	if(isCurveDirty){
		createHeightMap();
		calcMapNormals();
		calcMapShade();
		isCurveDirty = false;
	}
}
