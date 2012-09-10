#include <PerlinNoise.h>
#include <Math/Vector.h>
#include <Math/MathFunc.h>
#include <stdlib.h>

static const int GradTableSize = 16;
static Vector grad[GradTableSize] = 
{
	Vector(1,1,0), Vector(-1,1,0), Vector(1,-1,0), Vector(-1,-1,0),
	Vector(1,0,1), Vector(-1,0,1), Vector(1,0,-1), Vector(-1,0,-1),
	Vector(0,1,1), Vector(0,-1,1), Vector(0,1,-1), Vector(0,-1,-1),
	Vector(1,1,0), Vector(-1,1,0), Vector(0,-1,1), Vector(0,-1,-1)
};

PerlinNoise::PerlinNoise()
{
	//RandomNumber r(3452345);
	for(int i=0; i<PermTableSize; i++){
		perm[i] = rand() * PermTableSize / RAND_MAX;
		//perm[i] = int(++r);
	}
	//for(int i=0; i<GradTableSize; i++){
	//	grad[i].x = cosf(i * 2 * Pi / GradTableSize);
	//	grad[i].y = sinf(i * 2 * Pi / GradTableSize);
	//}
}

static float curve(float t)
{
	return t * t * t * (t * (t * 6  - 15) + 10);
}

float PerlinNoise::operator() (float x, float y) const
{
	int i = int(floorf(x)) & 0xff;
	int j = int(floorf(y)) & 0xff;
	float u = x - floorf(x);
	float v = y - floorf(y);
	float s = curve(u);
	float t = curve(v);
	int i11 = perm[(i + perm[j&0xff])&0xff];
	int i12 = perm[(i + perm[(j+1)&0xff])&0xff];
	int i21 = perm[(i+1 + perm[j&0xff])&0xff];
	int i22 = perm[(i+1 + perm[(j+1)&0xff])&0xff];
	Vector& g11 = grad[i11 % GradTableSize];
	Vector& g12 = grad[i12 % GradTableSize];
	Vector& g21 = grad[i21 % GradTableSize];
	Vector& g22 = grad[i22 % GradTableSize];
	float f11 = g11.x * u + g11.y * v;
	float f12 = g12.x * u + g12.y * (v-1);
	float f21 = g21.x * (u-1) + g21.y * v;
	float f22 = g22.x * (u-1) + g22.y * (v-1);
	return lerp(lerp(f11, f12, t), lerp(f21, f22, t), s);
}

float PerlinNoise::operator() (float x, float y, float z) const
{
	int i = int(floorf(x)) & 0xff;
	int j = int(floorf(y)) & 0xff;
	int k = int(floorf(z)) & 0xff;
	float u = x - floorf(x);
	float v = y - floorf(y);
	float w = z - floorf(z);
	float s = curve(u);
	float t = curve(v);
	float p = curve(w);
	int i111 = perm[i + (perm[(j + perm[k%PermTableSize]) % PermTableSize]) % PermTableSize];
	int i112 = perm[i + (perm[(j + perm[(k+1)%PermTableSize]) % PermTableSize]) % PermTableSize];
	int i121 = perm[i + (perm[(j+1 + perm[k%PermTableSize]) % PermTableSize]) % PermTableSize];
	int i122 = perm[i + (perm[(j+1 + perm[(k+1)%PermTableSize]) % PermTableSize]) % PermTableSize];
	int i211 = perm[i+1 + (perm[(j + perm[k%PermTableSize]) % PermTableSize]) % PermTableSize];
	int i212 = perm[i+1 + (perm[(j + perm[(k+1)%PermTableSize]) % PermTableSize]) % PermTableSize];
	int i221 = perm[i+1 + (perm[(j+1 + perm[k%PermTableSize]) % PermTableSize]) % PermTableSize];
	int i222 = perm[i+1 + (perm[(j+1 + perm[(k+1)%PermTableSize]) % PermTableSize]) % PermTableSize];
	Vector& g111 = grad[i111 % GradTableSize];
	Vector& g112 = grad[i112 % GradTableSize];
	Vector& g121 = grad[i121 % GradTableSize];
	Vector& g122 = grad[i122 % GradTableSize];
	Vector& g211 = grad[i211 % GradTableSize];
	Vector& g212 = grad[i212 % GradTableSize];
	Vector& g221 = grad[i221 % GradTableSize];
	Vector& g222 = grad[i222 % GradTableSize];
	float f111 = g111.x * u + g111.y * v + g111.z * w;
	float f112 = g112.x * u + g112.y * v + g112.z * (w-1);
	float f121 = g121.x * u + g121.y * (v-1) + g121.z * w;
	float f122 = g122.x * u + g122.y * (v-1) + g122.z * (w-1);
	float f211 = g211.x * (u-1) + g211.y * v + g211.z * w;
	float f212 = g212.x * (u-1) + g212.y * v + g212.z * (w-1);
	float f221 = g221.x * (u-1) + g221.y * (v-1) + g221.z * w;
	float f222 = g222.x * (u-1) + g222.y * (v-1) + g222.z * (w-1);
	return lerp(lerp(lerp(f111, f112, p), lerp(f121, f122, p), t), lerp(lerp(f211, f212, p), lerp(f221, f222, p), t), s);
}
