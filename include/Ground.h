#include <CircleAroundTrigon.h>

struct Ground
{
	struct Point{
		Point* next;
		Vector p, n;
		Point(const Vector& p): next(NULL), p(p), n(UnitVectorY) {}
	};
	struct Trigon{
		Trigon* next;
		Point* a, *b, *c;
		Vector n;
		Trigon(Point* A, Point* B, Point* C, const Vector& N): a(A), b(B), c(C), n(N) {}
	};
	Point* points;
	Trigon* tris;
	Ground(): points(NULL), tris(NULL) {}
	void addPoint(const Vector& p)
	{
		removeTrigonsThatSeePoint(p);
		Ground::Point* a = new Ground::Point(p);
		createTrigonsWithPoint(a);
		a->next = points; points = a;
		//DeleteList(tris); tris = NULL;
		//triangulate();
	}
	bool planeHasPointsOutsideExcept(const Plane& P, const Point* a, const Point* b, const Point* c) const
	{
		for(Point* x = points; x!=NULL; x=x->next){
			if(x == a || x == b || x == c) continue;
			if(distance(P, paraVertex(x->p)) > 0) return true;
		}
		return false;
	}
	static Vector paraVertex(const Vector& v) {return Vector(v.x, (v.x*v.x+v.z*v.z)/1000, v.z);}
	void triangulate()
	{
		//for(Point* a = points; a!=NULL; a=a->next) a->p = paraVertex(a->p);
		for(Point* a = points; a!=NULL; a=a->next){
			for(Point* b = a->next; b!=NULL; b=b->next){
				for(Point* c = b->next; c!=NULL; c=c->next){
					Vector A = paraVertex(a->p), B = paraVertex(b->p), C = paraVertex(c->p);
					Plane plane = planeFromPoints(A, B, C);
					if(plane.n.y > 0) plane = -plane;
					if(planeHasPointsOutsideExcept(plane, a, b, c)) continue;
					Plane p2 = planeFromPoints(a->p, b->p, c->p);
					if(p2.n.y < 0){
						Trigon* t = new Trigon(c, b, a, -unit(p2.n));
						t->next = tris; tris = t;
					}else{
						Trigon* t = new Trigon(a, b, c, unit(p2.n));
						t->next = tris; tris = t;
					}
					//t->n = unit(Vector(rand(), rand(), rand()));
				}
			}
		}
	}
	void removeTrigonsThatSeePoint(const Vector& p)
	{
		Vector P = paraVertex(p);
		Trigon* curTri = tris, *prevTri = NULL;
		while(curTri != NULL){
			Vector A = paraVertex(curTri->c->p), B = paraVertex(curTri->b->p), C = paraVertex(curTri->a->p);
			Plane plane = planeFromPoints(A, B, C);
			if(distance(plane, P) > 0){
				if(prevTri){
					prevTri->next = curTri->next;
					delete curTri;
					curTri = prevTri;
				}else{
					tris = curTri->next;
					delete curTri;
					curTri = tris;
				}
			}
			prevTri = curTri;
			if(curTri) curTri = curTri->next;
		}
	}
	void createTrigonsWithPoint(Point* c)
	{
		Vector C = paraVertex(c->p);
		for(Point* a = points; a!=NULL; a=a->next){
			Vector A = paraVertex(a->p);
			for(Point* b = a->next; b!=NULL; b=b->next){
				Vector B = paraVertex(b->p);
				Plane plane = planeFromPoints(A, B, C);
				if(plane.n.y > 0) plane = -plane;
				if(planeHasPointsOutsideExcept(plane, a, b, c)) continue;
				Plane p2 = planeFromPoints(a->p, b->p, c->p);
				if(p2.n.y < 0){
					Trigon* t = new Trigon(c, b, a, -unit(p2.n));
					t->next = tris; tris = t;
				}else{
					Trigon* t = new Trigon(a, b, c, unit(p2.n));
					t->next = tris; tris = t;
				}
				//t->n = unit(Vector(rand(), rand(), rand()));
			}
		}
	}
};
