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
#include <Math/FrustumFunc.h>
#include <stdio.h>
#include <Color.h>
#include <stdlib.h>
#include <PerlinNoise.h>
extern void glFastBox(float L, float B, float K, float R, float T, float F);
extern void glTexBox(float L, float B, float K, float R, float T, float F);

struct MapPatch;
void putPatchInQueue(MapPatch* patch);

struct MapPatch
{
	Vector color;
	Vector* verts;
	Vector* norms;
	float* shade;
	Vector boxMin, boxMax;
	MapPatch* children;
	int dispList;
	bool arraysReady;
	bool dispListReady;
	MapPatch() {
		ZeroMemory(this, sizeof(MapPatch));\
		static int i = 0;
		
		i += 151251251;
		color.x = float(i & 0xff) / 255.f;
		color.y = float((i>>8)&0xff) / 255.f;
		color.z = float((i>>16)&0xff) / 255.f;
		if(color.x < .3f) color.x = .3f;
		if(color.y < .3f) color.y = .3f;
		if(color.z < .3f) color.z = .3f;
		
	}
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
	const float z[] = {0, .05f, .07f, .12f, .35f, .6f, 1.f};
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

const Vector MapMin(-35000, 0, -35000);
const Vector MapMax(35000, 0, 35000);

const int MapReso = 100;
Vector& get(Vector* map, int x, int z) {return map[z*MapReso+x];}
float& get(float* map, int x, int z) {return map[z*MapReso+x];}
Curve mapCurve;
bool isCurveDirty = false;
PerlinNoise mapNoise;
MapPatch rootMapPatch;

const Vector CloudsMin(-25000, 2000, -25000), CloudsMax(25000, 2000, 25000);
const int CloudsReso = 1000;
float* cloudsMap = NULL;
float& getCloudsPoint(int x, int z) {return cloudsMap[z * CloudsReso + x];}
GLuint cloudsTexture = 0;

GLuint atmoTexture = 0;

//Vector testViewer(3333,0,1111);
//Frustum testFrustum = transform(pyramidFrustum(640, 480, 300),matrixFromOriginAxisZ(testViewer, unit(Vector(0,0,2))));

Vector* calcMapNormals(Vector* verts)
{
	Vector* norms = new Vector[MapReso*MapReso];
	for(int z=0; z<MapReso; z++){
		for(int x=0; x<MapReso; x++){
			Vector x1 = x > 0 ? get(verts, x-1, z) : get(verts, x, z);
			Vector x2 = x < MapReso-1 ? get(verts, x+1, z) : get(verts, x, z);
			Vector z1 = z > 0 ? get(verts, x, z-1) : get(verts, x, z);
			Vector z2 = z < MapReso-1 ? get(verts, x, z+1) : get(verts, x, z);
			get(norms, x, z) = unit(cross(z2-z1, x2-x1));
		}
	}
	return norms;
}

float* calcMapShade(Vector* norms)
{
	const Vector light = unit(Vector(1, 3, 2));
	float* shade = new float[MapReso*MapReso];
	for(int z=0; z<MapReso; z++){
		for(int x=0; x<MapReso; x++){
			get(shade, x, z) = dot(get(norms, x, z), light);
		}
	}
	return shade;
}

Vector* createHeightMap(const Vector& BoxMin, const Vector& BoxMax)
{
	Vector* verts = new Vector[MapReso*MapReso];
	const float periods[] = {
		5, 15, 35, 75, 
		155, 
		355,
		553,
		1122,
		2122,
		4122,
	};
	const int periodCount = sizeof(periods) / sizeof(periods[0]);
	float totalPeriod = 0;
	for(int i=0; i<periodCount; i++) totalPeriod += periods[i];
	float invTotalPeriod = 1.f / totalPeriod;
	for(int z=0; z<MapReso; z++){
		for(int x=0; x<MapReso; x++){
			Vector& v = get(verts, x, z);
			v.x = lerp(BoxMin.x, BoxMax.x, float(x)/float(MapReso-1));
			v.z = lerp(BoxMin.z, BoxMax.z, float(z)/float(MapReso-1));
			float h = 0;
			for(int i=0; i<periodCount; i++){
				h += mapNoise(v.x/periods[i], v.z/periods[i]) * periods[i] * invTotalPeriod;
			}
			h = h * 1.2f + .5f;
			v.y = mapCurve(h) * 1500;
		}
	}
	return verts;
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

void createPatchDisplayList(MapPatch& patch)
{
	patch.dispList = glGenLists(1);
	glNewList(patch.dispList, GL_COMPILE);
		for(int z=0; z<MapReso-1; z++){
			glBegin(GL_QUAD_STRIP);
				for(int x=0; x<MapReso; x++){
					float s1 = get(patch.shade, x, z), s2 = get(patch.shade, x, z+1);
					Vector v1 = get(patch.verts, x, z), v2 = get(patch.verts, x, z+1);
					Vector grassColor(0,.5f,0), rockColor(.5f,.5f,.5f), snowColor(1.f,1.f,1.f);
					//glColor(patch.color * s1);
					glColor((v1.y < 190 ? grassColor : v1.y < 1100 ? rockColor : snowColor)*s1);
					glVertex(v1);
					//glColor(patch.color * s2);
					glColor((v2.y < 190 ? grassColor : v2.y < 1100 ? rockColor : snowColor)*s2);
					glVertex(v2);
				}
			glEnd();
		}
	glEndList();
}

void createPatchArrays(MapPatch& patch)
{
	patch.verts = createHeightMap(patch.boxMin, patch.boxMax);
	patch.norms = calcMapNormals(patch.verts);
	patch.shade = calcMapShade(patch.norms);
}

void initMapPatch(MapPatch& patch, const Vector& BoxMin, const Vector& BoxMax)
{
	patch.boxMin = BoxMin, patch.boxMax = BoxMax;
	createPatchArrays(patch);
	createPatchDisplayList(patch);
}

void deletePatchChildren(MapPatch& patch)
{
	if(patch.children){
		for(int i=0; i<4; i++){
			deletePatchChildren(patch.children[i]);
			if(patch.children[i].dispListReady){
				glDeleteLists(patch.children[i].dispList, 1);
			}
			delete[] patch.children[i].shade;
			delete[] patch.children[i].norms;
			delete[] patch.children[i].verts;
		}
		delete[] patch.children;
		patch.children = NULL;
	}
}

void subdivPatch(MapPatch& patch)
{
	patch.children = new MapPatch[4];
	Vector boxCen = (patch.boxMin + patch.boxMax) / 2;
	for(int i=0; i<4; i++){
		MapPatch& child = patch.children[i];
		child.boxMin = Vector((i % 2) ? boxCen.x : patch.boxMin.x, patch.boxMin.y, (i / 2) ? boxCen.z : patch.boxMin.z);
		child.boxMax = Vector((i % 2) ? patch.boxMax.x : boxCen.x, patch.boxMax.y, (i / 2) ? patch.boxMax.z : boxCen.z);
		putPatchInQueue(&child);
	}
}

const int PatchQueueSize = 250;
MapPatch* patchQueue[PatchQueueSize];
int queuePatchCount = 0;
//TODO: patch queue critical section
CRITICAL_SECTION queueCritSect;

void putPatchInQueue(MapPatch* patch)
{
	EnterCriticalSection(&queueCritSect);
		if(queuePatchCount < PatchQueueSize){
			patchQueue[queuePatchCount++] = patch;
		}else{
			printf("Patch queue overflow\n");
		}
	LeaveCriticalSection(&queueCritSect);
}

void shiftPatchQueue()
{
	EnterCriticalSection(&queueCritSect);
		for(int i=0; i<queuePatchCount-1; i++){
			patchQueue[i] = patchQueue[i+1];
		}
		queuePatchCount--;
	LeaveCriticalSection(&queueCritSect);
}

void patchQueueThreadProc()
{
	while(true){
		if(queuePatchCount > 0){
			createPatchArrays(*patchQueue[0]);
			patchQueue[0]->arraysReady = true;
			shiftPatchQueue();
		}else{
			Sleep(5);
		}
	}
}

void CreateWorld()
{
	createCloudsMap();
	createCloudsTexture();
	initMapPatch(rootMapPatch, MapMin, MapMax);
	//subdivPatch(rootMapPatch);
	//subdivPatch(rootMapPatch.children[0]);
	//subdivPatch(rootMapPatch.children[0].children[3]);
	//subdivPatch(rootMapPatch.children[0].children[3].children[3]);
	//subdivPatch(rootMapPatch.children[0].children[3].children[3].children[3]);
	//subdivPatch(rootMapPatch.children[0].children[3].children[3].children[3].children[3]);
	InitializeCriticalSection(&queueCritSect);
	CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(patchQueueThreadProc), NULL, 0, NULL);
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

bool patchChildrenReady(const MapPatch& p)
{
	if(! p.children) return false;
	for(int i=0; i<4; i++){
		if(! p.children[i].arraysReady) return false;
		if(! p.children[i].dispListReady){
			createPatchDisplayList(p.children[i]);
			p.children[i].dispListReady = true;
		}
	}
	return true;
}

bool isPatchChildrenSafeToDelete(const MapPatch& p)
{
	if(! p.children) return false;
	for(int i=0; i<4; i++){
		if(! isPatchChildrenSafeToDelete(p.children[i])) return false;
		if(! p.children[i].arraysReady) return false;
	}
	return true;
}

void drawPatchRecur(MapPatch& patch, const Frustum& frustum)
{
	Vector c = (patch.boxMin + patch.boxMax) / 2;
	float r = length(patch.boxMax - patch.boxMin) / 2;
	if(! isSpherePartInFrustum(c, r, frustum)){
		return;
	}
	if(patchChildrenReady(patch)){
		for(int i=0; i<4; i++){
			drawPatchRecur(patch.children[i], frustum);
		}
	}else{
		glCallList(patch.dispList);
	}
}

bool isViewerNearPatch(MapPatch& patch, const Vector& viewer)
{
	const float PatchSubdivRadius = (patch.boxMax.x - patch.boxMin.x) * 100.f / float(MapReso);
	Vector p = nearestBoxPoint(patch.boxMin, patch.boxMax, viewer);
	return square(viewer - p) < square(PatchSubdivRadius);
}

void processPatchDetail(MapPatch& patch, const Vector& viewer)
{
	if(patch.children){
		for(int i=0; i<4; i++){
			processPatchDetail(patch.children[i], viewer);
		}
	}
	if(isViewerNearPatch(patch, viewer)){
		if(! patch.children){
			float reso = (patch.boxMax.x - patch.boxMin.x) / float(MapReso-1);
			if(reso > 1.f){
				subdivPatch(patch);
			}
		}
		return;
	}else{
		if(isPatchChildrenSafeToDelete(patch)){
			deletePatchChildren(patch);
		}
	}
}

void RenderWorld(const Vector& viewer, const Frustum& frustum)
{
	processPatchDetail(rootMapPatch, viewer);
	
	float fogColor[] = {.8f, .8f, 1.f, 1.f};
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, .00003f);
	glShadeModel(GL_FLAT);
	drawPatchRecur(rootMapPatch, frustum);
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
		//createHeightMap();
		//calcMapNormals();
		//calcMapShade();
		isCurveDirty = false;
	}
}
