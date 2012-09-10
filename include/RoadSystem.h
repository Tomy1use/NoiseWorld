#pragma once

struct RenderObject;
struct RoadSegment;

class RoadNode
{
	Vector p, v;
public:
	RoadNode* parent;
	RoadNode* next;
	RoadSegment* back, *forw;
	RoadNode* left, *right;
	RoadNode(const Vector& p, RoadNode* parent)
		: p(p)
		, v(ZeroVector)
		, parent(parent ? parent->parent ? parent->parent : parent : NULL)
		, next(NULL)
		, back(NULL), forw(NULL), left(NULL), right(NULL) {}
	const Vector& velocity() const {return parent ? parent->velocity() : v;}
	const Vector& point() const {return p;}
	void setVelocity(const Vector& value) {v = value;}
	void setPoint(const Vector& value) {p = value;}
};

struct RoadSegment
{
	RoadSegment* next;
	RoadSegment* parent;
	float offset;
	RoadNode* back, *forw;
	Vector A, B, C, D;
	float cachedLength;
	RenderObject* renderObject;
	RoadSegment()
		: next(NULL)
		, parent(NULL)
		, offset(0)
		, back(NULL)
		, forw(NULL)
		, cachedLength(0) 
		, renderObject(NULL)
	{}
	Vector right(float t) const
	{
		return unit(cross(velocity(t), UnitVectorY));
	}
	Vector point(float t) const
	{
		if(parent) return parent->point(t) + parent->right(t) * offset;
		return A + B * t + C * t * t + D * t * t * t;
	}
	Vector velocity(float t) const
	{
		if(parent) return parent->velocity(t);
		return B + C * t * 2 + D * t * t * 3;
	}
	float resoLength(int reso)
	{
		float L = 0;
		Vector a = point(0);
		for(int i=1; i<=reso; i++){
			Vector b = point(float(i)/float(reso));
			L += length(b - a);
			a = b;
		}
		//printf("resoLength %.2f\n", L);
		return L;
	}
	float getLength()
	{
		if(cachedLength == 0){
			cachedLength = resoLength(16);//tests show that this resolution gives < 1% error
			/*
			printf("Calculating road segment length\n");
			float L = length(point(0) - point(1));
			printf("Initial %.2f\n", L);
			for(int reso=2; ; reso *= 2){
				cachedLength = resoLength(reso);
				printf("Reso %d: %.4f, ratio %.4f\n", reso, cachedLength, (cachedLength-L)/L);
				if(abs((cachedLength-L)/L) < .01f) break;
				L = cachedLength;
			}
			*/
		}
		return cachedLength;
	}
};

const float RoadWidth = 4.f;
const float RoadHeight = .5f;

struct RoadSystem
{
	RoadNode* nodes;
	RoadSegment* segments;
	RoadNode* drawNode;
	int drawNodeSlot;//-1 back, +1 forw
	
	RoadSystem()
		: nodes(NULL)
		, segments(NULL)
		, drawNode(NULL)
		, drawNodeSlot(-1)
	{
	}
	~RoadSystem()
	{
		DeleteList(segments);
		DeleteList(nodes);
	}
	RoadNode* nodeInRadius(const Vector& p, float R)
	{
		for(RoadNode* n=nodes; n!=NULL; n=n->next){
			if(square(n->point() - p) < square(R)) return n;
		}
		return NULL;
	}
	static float nodeSlotSize() {return .5f;}
	static Vector nodeSlot(const RoadNode* n, float dir) {
		return n->point() + unit(n->velocity()) * 2.f * nodeSlotSize() * dir;
	}
	RoadNode* nodeSlotInRadius(const Vector& p, float R, int *nSlot) const{
		for(RoadNode* n=nodes; n!=NULL; n=n->next){
			if(n->back && n->forw) continue;
			if(! n->forw) if(square(nodeSlot(n, +1.f)-p) < square(R)) {*nSlot=1; return n;}
			if(! n->back) if(square(nodeSlot(n, -1.f)-p) < square(R)) {*nSlot=-1; return n;}
		}
		return NULL;
	}
	void onClick(const Vector& p)
	{
		int nSlot = 0;
		RoadNode* n = nodeSlotInRadius(p, 1.f, &nSlot);
		//printf("%x: %d\n", int(n), nSlot);
		if(! n){
			n = new RoadNode(p, NULL);
			//printf("+ %x\n", int(n));
			n->next = nodes; nodes = n;
			nSlot = drawNode ? -1 : +1;
		}
		if(drawNode){
			addSegment(drawNode, drawNodeSlot, n, nSlot);
			if(n->back && n->forw) n = NULL;
			nSlot = -nSlot;
		}
		drawNode = n;
		drawNodeSlot = nSlot;
	}
	void onMove(const Vector& p)
	{
		if(drawNode){
			drawNode->setPoint(p);
			calcNodeVelocity(drawNode);
			calcSegment(drawNode->back);
			calcSegment(drawNode->forw);
		}
	}
	void addSegment(RoadNode* n1, int slot1, RoadNode* n2, int slot2)
	{
		//printf("addSegment %x: %d, %x: %d\n", int(n1), slot1, int(n2), slot2);
		RoadSegment* s = new RoadSegment();
		s->next = segments; segments = s;
		if(slot1 < 0){
			if(n1->back) printf("Error: node slot not empty\n");
			n1->back = s;
		}else{
			if(n1->forw) printf("Error: node slot not empty\n");
			n1->forw = s;
		}
		if(slot2 < 0){
			if(n2->back) printf("Error: node slot not empty\n");
			n2->back = s;
		}else{
			if(n2->forw) printf("Error: node slot not empty\n");
			n2->forw = s;
		}
		s->back = n1; s->forw = n2;
		
		findSegmentParent(s);
		
		calcNodeVelocity(n1);
		calcNodeVelocity(n2);
		
		if(! s->parent){
			calcSegment(s);
			calcSegment(otherSegment(s->back, s));
			calcSegment(otherSegment(s->forw, s));
			
			s->getLength();
		}
	}
	void calcNodeVelocity(RoadNode* n)
	{
		RoadNode* n1 = otherNode(n->back, n);
		RoadNode* n2 = otherNode(n->forw, n);
		if(n1 && n2){
			Vector v = unit(n2->point() - n1->point()) * (length(n2->point()-n->point()) + length(n->point()-n1->point()))/2;
			//Vector v = unit(unit(n2->point()-n->point()) + unit(n->point()-n1->point())) * (n->forw->length + n->back->length)/2;
			n->setVelocity(v);
			if(n->left == NULL || n->right == NULL){
				Vector r = unit(cross(n->velocity(), UnitVectorY));
				if(n->left == NULL){
					n->left = new RoadNode(n->point() - r*RoadWidth, n);
					n->left->right = n;
					n->left->next = nodes; nodes = n->left;
				}
				if(n->right == NULL){
					n->right = new RoadNode(n->point() + r*RoadWidth, n);
					n->right->left = n;
					n->right->next = nodes; nodes = n->right;
				}
			}
		}
	}
	void calcSegment(RoadSegment* s)
	{
		if(! s) return;
		Vector p1 = s->back->point();
		Vector p2 = s->forw->point();
		Vector v1 = nodeVelocity(s->back, s);
		Vector v2 = -nodeVelocity(s->forw, s);
		s->A = p1;
		s->B = v1;
		s->C = v1 - v2 + p2 * 3 - p1 * 3 - v1 * 3;
		s->D = p2 - p1 - v1 - s->C;
	}
	static RoadNode* otherNode(RoadSegment* s, RoadNode* n)
	{
		if(s == NULL) return NULL;
		if(s->forw == n) return s->back;
		if(s->back == n) return s->forw;
		printf("otherNode: asking wrong segment\n");
		return NULL;
	}
	static RoadSegment* otherSegment(RoadNode* n, RoadSegment* s)
	{
		if(n == NULL) return NULL;
		if(n->forw == s) return n->back;
		if(n->back == s) return n->forw;
		printf("otherSegment: asking wrong node\n");
		return NULL;
	}
	static Vector nodeVelocity(RoadNode* n, RoadSegment* s)
	{
		if(n->forw == s) return n->velocity();
		if(n->back == s) return -n->velocity();
		printf("nodeVelocity: asking wrong node\n");
		return ZeroVector;
	}
	void findSegmentParent(RoadSegment* s)
	{
		if(! (s->back && s->forw)) return;
		if(! (s->back->parent && s->forw->parent)) return;
		RoadSegment* p = 0;
		if(s->back->parent->back == s->forw->parent->back) p = s->back->parent->back;
		if(s->back->parent->back == s->forw->parent->forw) p = s->back->parent->back;
		if(s->back->parent->forw == s->forw->parent->back) p = s->back->parent->forw;
		if(s->back->parent->forw == s->forw->parent->forw) p = s->back->parent->forw;
		if(p){
			s->parent = p;
			Vector r = s->back->point() - s->back->parent->point();
			Vector pR = (s->back->parent == p->back) ? p->right(0) : p->right(1);
			s->offset = dot(r, pR);
		}
	}
};
