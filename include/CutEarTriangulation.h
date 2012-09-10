#pragma once

#include <Math/NewellNormal.h>

template<int PointArraySize>
struct CutEarTriangulation
{
	struct Point{
		Point* next;
		Vector p;
	};
	struct Trigon{
		Vector a, b, c;
	};
	int pointCount;
	Point points[PointArraySize];
	Point* polygon;
	Trigon tris[PointArraySize];
	int trigonCount;
	Vector polyNor;
	CutEarTriangulation()
		: pointCount(0)
		, polygon(0)
		, trigonCount(0)
	{
	}
	void addPoint(const Vector& p)
	{
		if(pointCount >= PointArraySize){
			printf("CutEarTriangulation: too many points!\n");
			return;
		}
		points[pointCount].p = p;
		points[pointCount].next = &points[pointCount+1];
		pointCount++;
		printf("addPoint %d\n", pointCount);
	}
	void compute()
	{
		if(pointCount < 3){
			printf("CutEarTriangulation: not enough points to triangulate\n");
			return;
		}
		points[pointCount-1].next = &points[0];
		polygon = &points[0];
		{
			NewellNormal N;
			for(int i=0; i<pointCount; i++) N.advance(points[i].p);
			polyNor = N.result();
		}
		int restPointCount = pointCount;
		Point* a = polygon;
		while(restPointCount > 3){
			Point* b = a->next;
			Point* c = b->next;
			if(isEar(a, b, c)){
				emitTrigon(a, b, c);
				a->next = c;
				polygon = a = c;
				restPointCount--;
			}else{
				a = a->next;
			}
		}
		if(restPointCount == 3) emitTrigon(a, a->next, a->next->next);
		else printf("rest point count %d\n", restPointCount);
	}
	bool isInward(const Vector& A, const Vector& B, const Vector& C)
	{
		return dot(cross(B-A, C-B), polyNor) > 0;
	}
	bool isEar(const Point* A, const Point* B, const Point* C)
	{
		if(! isInward(A->p, B->p, C->p)) return false;
		Vector e1 = B->p - A->p, e2 = C->p - B->p, e3 = A->p - C->p;
		Vector n1(cross(e1, polyNor)), n2(cross(e2, polyNor)), n3(cross(e3, polyNor));
		Point* P = polygon; do {
			if(P != A && P != B && P != C){
				if(dot(P->p-A->p, n1) < 0 && dot(P->p-B->p, n2) < 0 && dot(P->p-C->p, n3) < 0){
					return false;
				}
			}
			P = P->next;
		} while(P != polygon);
		return true;
	}
	void emitTrigon(const Point* a, const Point* b, const Point* c)
	{
		tris[trigonCount].a = a->p;
		tris[trigonCount].b = b->p;
		tris[trigonCount].c = c->p;
		trigonCount++;
	}
};
