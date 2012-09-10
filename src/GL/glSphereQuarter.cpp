#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <Math/Vector.h>
#include <Math/VectorFunc.h>
#include <Math/MathFunc.h>
#include <glFunc.h>

static void recurse(
	const Vector& K, const Vector& A, const Vector& B, const Vector& C, 
	float R, int depth, float norSig)
{
	if(depth > 0){
		Vector D = (B+C)/2, E = (C+A)/2, F = (A+B)/2;
		D = unit(D-K)*R+K, E = unit(E-K)*R+K, F=unit(F-K)*R+K;
		recurse(K, A, F, E, R, depth-1, norSig);
		recurse(K, B, D, F, R, depth-1, norSig);
		recurse(K, C, E, D, R, depth-1, norSig);
		recurse(K, D, E, F, R, depth-1, norSig);
	}else{
		//glTexCoord2f(A.y, A.z);
		glNormal((A-K)*norSig);
		glVertex(A);
		//glTexCoord2f(B.z, B.x);
		glNormal((B-K)*norSig);
		glVertex(B);
		//glTexCoord2f(C.x, C.y);
		glNormal((C-K)*norSig);
		glVertex(C);
	}
}

void glSphereQuarter(const Vector& K, const Vector& A, const Vector& B, const Vector& C,  float radius, int depth)
{
	float norSig = signum(dot(A-K, cross(B-A, C-A)));//flip normal if concave
	glBegin(GL_TRIANGLES);
		recurse(K, A, B, C, radius, depth, norSig);
	glEnd();
}
