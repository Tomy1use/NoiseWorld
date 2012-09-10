#include <stdio.h>
#include <Math/Vector.h>
#include <Math/VectorFunc.h>
#include <Surflet.h>

template<class T>
inline void swap(T& a, T&b)
{
	T c = a;
	a = b;
	b = c;
}

static int axisBit(const Vector& p1, const Vector& p2)
{
	Vector a = p2 - p1;
	if(a.x != 0 && a.y == 0 && a.z == 0){
		return 1;
	}
	if(a.x == 0 && a.y != 0 && a.z == 0){
		return 2;
	}
	if(a.x == 0 && a.y == 0 && a.z != 0){
		return 3;
	}
	return 0;
}

static int arcNormBit(const Vector& p1, const Vector& p2)
{
	if(p1.x == p2.x){
		return 1;
	}
	if(p1.y == p2.y){
		return 2;
	}
	if(p1.z == p2.z){
		return 3;
	}
	return 0;
}

static bool arcsCoaxial(const Edge& e1, const Edge& e2)
{
	int n1 = arcNormBit(e1.A, e1.B);
	int n2 = arcNormBit(e2.A, e2.B);
	if(n1 && n2 && n1 == n2){
		if(axisBit(e1.C, e2.C) == n1){
			return true;
		}
	}
	return false;
}

bool torusSurfletFromEdges(TorusSurf& s, Edge e1, Edge e2, Edge e3, const Vector& eye)
{
	if(e1.R && e2.R && e3.R){//3 arcs
		if((e1.A == e2.A || e1.B == e2.A || e1.A == e2.B || e1.B == e2.B) && (e1.A == e3.A || e1.B == e3.A || e1.A == e3.B || e1.B == e3.B)){
			swap(e1, e3);
		}
		if((e2.A == e1.A || e2.B == e1.A || e2.A == e1.B || e2.B == e1.B) && (e2.A == e3.A || e2.B == e3.A || e2.A == e3.B || e2.B == e3.B)){
			swap(e2, e3);
		}
		if(e3.A == e2.A || e3.A == e2.B){
			swap(e1, e2);
		}
		if(e3.A == e1.B){
			swap(e1.A, e1.B);
		}
		if(e3.B == e2.B){
			swap(e2.A, e2.B);
		}
		if(e3.A == e1.A && e3.B == e2.A){
			if(arcsCoaxial(e1, e2)){
				if(axisBit(e1.C, e2.C)){
					s.c1 = e1.C, s.a1 = e1.A, s.b1 = e1.B;
					s.c2 = e2.C, s.a2 = e2.A, s.b2 = e2.B;
					s.r1 = e1.R;
					s.r2 = e2.R;
					if(axisBit(e1.A, e3.C) == axisBit(e1.C, e2.C)){
						s.c3 = e2.C, s.a3 = e1.A + (e2.C - e1.C), s.b3 = e1.B + (e2.C - e1.C);
						s.r3 = e1.R;
					}else{
						s.c3 = e1.C, s.a3 = e2.A + (e1.C - e2.C), s.b3 = e2.B + (e1.C - e2.C);
						s.r3 = e2.R;
					}
					s.R = e3.R;
					if(dot(cross(s.b1-s.a1, s.a2-s.a1), eye-s.a1) < 0){
						swap(s.a1, s.b1);
						swap(s.a2, s.b2);
						swap(s.a3, s.b3);
					}
					return true;
				}
			}
		}
	}
	return false;
}
