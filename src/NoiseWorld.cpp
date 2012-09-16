#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/GL.h>
#include <GL/ExtensionFunctions.h>
#include <GL/ShaderFunctions.h>
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

struct Frus
{
	static const int PlaneCount = 5;
	Plane p[PlaneCount];
};

bool isBoxBehindPlane(const Vector& BoxMin, const Vector& BoxMax, const Plane& p)
{
	for(int v=0; v<8; v++){
		float x = (v & 1) ? BoxMax.x : BoxMin.x;
		float y = (v & 2) ? BoxMax.y : BoxMin.y;
		float z = (v & 4) ? BoxMax.z : BoxMin.z;
		if(distance(p, Vector(x,y,z)) > 0) return false;
	}
	return true;
}

bool isBoxPartInFrus(const Vector& BoxMin, const Vector& BoxMax, const Frus& f)
{
	for(int i=0; i<Frus::PlaneCount; i++){
		if(isBoxBehindPlane(BoxMin, BoxMax, f.p[i])){
			return false;
		}
	}
	return true;
}

struct MapPatch;
void putPatchInQueue(MapPatch* patch);

struct MapPatch
{
	Vector color;
	float* height;
	Vector boxMin, boxMax;
	MapPatch* children;
	GLuint heightTexture;
	bool arraysReady;
	bool dispListReady;
	MapPatch() {
		ZeroMemory(this, sizeof(MapPatch));
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
const Vector MapMax(35000, 3000, 35000);

const int MapReso = 100;
Vector& get(Vector* map, int x, int z) {return map[z*MapReso+x];}
float& get(float* map, int x, int z) {return map[z*MapReso+x];}
Curve mapCurve;
bool isCurveDirty = false;
PerlinNoise mapNoise;
MapPatch rootMapPatch;
GLuint surfaceShader = 0;
int patchDispList = 0;

const Vector CloudsMin(-25000, 2000, -25000), CloudsMax(25000, 2000, 25000);
const int CloudsReso = 1000;
float* cloudsMap = NULL;
float& getCloudsPoint(int x, int z) {return cloudsMap[z * CloudsReso + x];}
GLuint cloudsTexture = 0;
GLuint cloudsShader = 0;

float* createHeightMap(const Vector& BoxMin, const Vector& BoxMax, float& outMinY, float& outMaxY)
{
	float* height = new float[MapReso*MapReso];
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
	float minY = 1000000, maxY = -1000000;
	for(int z=0; z<MapReso; z++){
		for(int x=0; x<MapReso; x++){
			float h = 0;
			for(int i=0; i<periodCount; i++){
				float X = lerp(BoxMin.x, BoxMax.x, float(x)/float(MapReso-1));
				float Z = lerp(BoxMin.z, BoxMax.z, float(z)/float(MapReso-1));
				h += mapNoise(X/periods[i], Z/periods[i]) * periods[i] * invTotalPeriod;
			}
			h = h * 1.f + .5f;
			h = mapCurve(h) * 2500;
			get(height, x, z) = h;
			if(h < minY) minY = h;
			if(maxY < h) maxY = h;
		}
	}
	for(int i=0; i<MapReso*MapReso; i++){
		height[i] = (height[i] - minY) / (maxY - minY);
	}
	outMinY = minY;
	outMaxY = maxY;
	return height;
}

void createCloudsMap()
{
	if(! cloudsMap) cloudsMap = new float[CloudsReso*CloudsReso];
	const float periods[] = {
		//5, 15, 35, 
		75, 
		155, 
		355,
		573,
		1021,
		2555,
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
			getCloudsPoint(x, z) = h * 3;
		}
	}
}

void createCloudsTexture()
{
	glGenTextures(1, &cloudsTexture);
    glBindTexture(GL_TEXTURE_2D, cloudsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, CloudsReso, CloudsReso, 0, GL_ALPHA, GL_FLOAT, cloudsMap);
}

void createPatchDisplayList()
{
	patchDispList = glGenLists(1);
	glNewList(patchDispList, GL_COMPILE);
		glColor3f(.5f,.5f,.5f);
		for(int z=0; z<MapReso-1; z++){
			glBegin(GL_QUAD_STRIP);
				for(int x=0; x<MapReso; x++){
					float X = float(x)/float(MapReso-1);
					glVertex3f(X, 0, float(z)/float(MapReso-1));
					glVertex3f(X, 0, float(z+1)/float(MapReso-1));
				}
			glEnd();
		}
	glEndList();
}

void createPatchDisplayList(MapPatch& patch)
{
	glGenTextures(1, &patch.heightTexture);
	glBindTexture(GL_TEXTURE_2D, patch.heightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, MapReso, MapReso, 0, GL_LUMINANCE, GL_FLOAT, patch.height);
}

void createPatchArrays(MapPatch& patch)
{
	patch.height = createHeightMap(patch.boxMin, patch.boxMax, patch.boxMin.y, patch.boxMax.y);
}

void initRootMapPatch(MapPatch& patch, const Vector& BoxMin, const Vector& BoxMax)
{
	patch.boxMin = BoxMin, patch.boxMax = BoxMax;
	createPatchArrays(patch);
	createPatchDisplayList(patch);
	patch.arraysReady = true;
}

void deletePatchChildren(MapPatch& patch)
{
	if(patch.children){
		for(int i=0; i<4; i++){
			deletePatchChildren(patch.children[i]);
			if(patch.children[i].dispListReady){
				//glDeleteTextures(1, &patch.heightTexture);
			}
			delete[] patch.children[i].height;
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

const int PatchQueueSize = 1000;
MapPatch* patchQueue[PatchQueueSize];
int queuePatchCount = 0;
//TODO: patch queue critical section
CRITICAL_SECTION queueCritSect;

void putPatchInQueue(MapPatch* patch)
{
	EnterCriticalSection(&queueCritSect);
		if(queuePatchCount < PatchQueueSize){
			patchQueue[queuePatchCount++] = patch;
			//printf("%d\n", queuePatchCount);
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
		//printf("%d\n", queuePatchCount);
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
	surfaceShader = createShader("Shaders/SurfaceVertexShader.glsl", "Shaders/SurfacePixelShader.glsl", NULL);
	cloudsShader = createShader("Shaders/CloudsVertexShader.glsl", "Shaders/CloudsPixelShader.glsl", NULL);
	
	createCloudsMap();
	createCloudsTexture();
	initRootMapPatch(rootMapPatch, MapMin, MapMax);
	createPatchDisplayList();
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
	if(! p.children) return true;
	for(int i=0; i<4; i++){
		if(! isPatchChildrenSafeToDelete(p.children[i])) return false;
		if(! p.children[i].arraysReady) return false;
	}
	return true;
}

void drawPatchRecur(MapPatch& patch, const Frus& frus)
{
	if(! isBoxPartInFrus(patch.boxMin, patch.boxMax, frus)){
		return;
	}
	if(patchChildrenReady(patch)){
		for(int i=0; i<4; i++){
			drawPatchRecur(patch.children[i], frus);
		}
	}else{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, patch.heightTexture);
		setShaderVector(surfaceShader, "boxMin", patch.boxMin);
		setShaderVector(surfaceShader, "boxMax", patch.boxMax);
		setShaderTexture(surfaceShader, "heightMap", 0);
		//glCallList(patch.dispList);
		glCallList(patchDispList);
	}
}

bool isViewerNearPatch(MapPatch& patch, const Vector& viewer)
{
	const float PatchSubdivRadius = (patch.boxMax.x - patch.boxMin.x) * 100.f / float(MapReso-1);
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
			if(patch.arraysReady){
				float reso = (patch.boxMax.x - patch.boxMin.x) / float(MapReso-1);
				if(reso > 1.f){
					subdivPatch(patch);
				}
			}
		}
		return;
	}else{
		if(patch.children){
			if(isPatchChildrenSafeToDelete(patch)){
				deletePatchChildren(patch);
			}
		}
	}
}



Matrix makeGroundMatrix(const Matrix& M)
{
	Matrix G;
	G.origin = M.origin;
	G.origin.y = 300;
	G.axes.x = M.axes.x;
	G.axes.y = UnitVectorY;
	G.axes.z = unit(cross(G.axes.x, G.axes.y));
	return G;
}

void drawFrustumForMatrix(const Matrix& M, float aspect, float fov)
{
	float d = 15000;
	float x = tanf(fov/2);
	float y = x / aspect;
	Vector v[4] = {
		M.origin + (-M.axes.z - M.axes.x * x - M.axes.y * y) * d,
		M.origin + (-M.axes.z - M.axes.x * x + M.axes.y * y) * d,
		M.origin + (-M.axes.z  + M.axes.x * x + M.axes.y * y) * d,
		M.origin + (-M.axes.z + M.axes.x * x - M.axes.y * y) * d,
	};
	glBegin(GL_LINES);
		for(int i=0; i<4; i++){
			glVertex(M.origin); glVertex(v[i]);
			glVertex(v[i]); glVertex(v[(i+1)%4]);
		}
	glEnd();
}

Frus calcFrus(const Matrix& M, float aspect, float fov)
{
	float x = tanf(fov/2);
	float y = x / aspect;
	Vector v[4] = {
		M.origin - M.axes.z - M.axes.y * y - M.axes.x * x,
		M.origin - M.axes.z + M.axes.y * y - M.axes.x * x,
		M.origin - M.axes.z + M.axes.y * y + M.axes.x * x,
		M.origin - M.axes.z - M.axes.y * y + M.axes.x * x,
	};
	Frus f;
	f.p[0] = Plane(-M.axes.z, dot(-M.axes.z, M.origin));
	for(int i=0; i<4; i++){
		f.p[i+1] = planeFromPoints(v[i], v[(i+1)%4], M.origin);
	}
	return f;
}

void drawClouds(const Vector& viewer)
{
	glShadeModel(GL_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	Vector cMin(-13500,2000,-13500), cMax(13500,2000,13500);
	float dX = 100, dZ = 100;
	for(float z=cMin.z; z<cMax.z; z+=dZ){
		int j1 = int((z - CloudsMin.z) / (CloudsMax.z-CloudsMin.z) * (CloudsReso-1));
		int j2 = int((z+dZ - CloudsMin.z) / (CloudsMax.z-CloudsMin.z) * (CloudsReso-1));
		
		glBegin(GL_QUAD_STRIP);
		for(float x=cMin.x; x<cMax.x; x+=dX){
			int i = int((x - CloudsMin.x) / (CloudsMax.x-CloudsMin.x) * (CloudsReso-1));
			float k1 = cloudsMap[j1 * CloudsReso + i];
			float k2 = cloudsMap[j2 * CloudsReso + i];
			float s1 = k1 * .3f + .7f;
			float s2 = k2 * .3f + .7f;
			Vector v1(x, cMin.y+k1*500, z);
			Vector v2(x, cMin.y+k2*500, z+dZ);
			glColor4f(s1,s1,s1,k1);
			glVertex(v1);
			glColor4f(s2,s2,s2,k2);
			glVertex(v2);
		}
		glEnd();
		
		glBegin(GL_QUAD_STRIP);
		for(float x=cMin.x; x<cMax.x; x+=dX){
			int i = int((x - CloudsMin.x) / (CloudsMax.x-CloudsMin.x) * (CloudsReso-1));
			float k1 = cloudsMap[j1 * CloudsReso + i];
			float k2 = cloudsMap[j2 * CloudsReso + i];
			float s1 = k1 * -.3f + .7f;
			float s2 = k2 * -.3f + .7f;
			Vector v1(x, cMin.y-k1*100, z);
			Vector v2(x, cMin.y-k2*100, z+dZ);
			glColor4f(s2,s2,s2,k2);
			glVertex(v2);
			glColor4f(s1,s1,s1,k1);
			glVertex(v1);
		}
		glEnd();
	}
}

void drawCumulusClouds()
{
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

void RenderWorld(const Matrix& cameraMatrix, float aspect, float fov, const Vector& vLightDir)
{
	Matrix G = cameraMatrix;// makeGroundMatrix(cameraMatrix);
	Frus f = calcFrus(G, aspect, fov);
	
	processPatchDetail(rootMapPatch, G.origin);
	
	float fogColor[] = {.8f, .8f, 1.f, 1.f};
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, .00003f);
	glShadeModel(GL_FLAT);
	
	
	glUseProgram(surfaceShader);
	setShaderVector(surfaceShader, "vLightDir", vLightDir);
	drawPatchRecur(rootMapPatch, f);
	
	glUseProgram(cloudsShader);
	//drawCumulusClouds();
	drawClouds(cameraMatrix.origin);
	glDisable(GL_BLEND);
	glColor3f(1,1,1);
	drawFrustumForMatrix(G, aspect, fov);
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
