#pragma once

#include <BoolObject.h>
#include <RoadSystem.h>
#include <Ground.h>
#include <AskForString.h>
#include <Building.h>
#include <GUI/GUIFunc.h>

typedef BoolObject<Vector> BoolPoint;

struct Action
{
    enum Type {
        NoAction, 
        PutEdge, 
        PutSurf, 
        DeleteEdge, 
        DeleteSelection, 
        MoveSelection, 
        CloneSelection
    };
    virtual int type() = 0;
    Action* next;
};

struct PutEdgeAction: Action
{
    Edge* edge;
    int type() {return PutEdge;}
    PutEdgeAction(Edge* e): edge(e) {}
};

struct PutSurfAction: Action
{
    Surf* surf;
    int type() {return PutSurf;}
    PutSurfAction(Surf* s): surf(s) {}
};

struct DeleteEdgeAction: Action
{
    Edge* edge, *before;
    int type() {return DeleteEdge;}
    DeleteEdgeAction(): edge(0), before(0) {}
    DeleteEdgeAction(Edge* e, Edge* f): edge(e), before(f) {}
    virtual ~DeleteEdgeAction() {delete(edge);}
};

struct Selection
{
    static const int EdgeArraySize = 128;
    Edge* edge[EdgeArraySize];
	int edgeCount;
    Selection()
	{
		clear();
	}
    void clear()
    {
        edgeCount = 0;
    }
    void push(Edge* e)
    {
		if(edgeCount < EdgeArraySize){
			edge[edgeCount++] = e;
		}else{
			MessageBox(0, "Edge selection array is full", "Shapecraft", 0);
		}
    }
};

struct MoveSelectionAction: Action
{
    typedef Edge* EdgePtr;
    int edgeCount;
    EdgePtr* edges;
    Vector src, dest;
    int type() {return MoveSelection;}
    MoveSelectionAction(const Selection& selection, const Vector& src, const Vector& dest)
        : edgeCount(selection.edgeCount)
        , edges(new EdgePtr[edgeCount])
        , src(src), dest(dest)
    {
        for(int i=0; i<edgeCount; i++) edges[i] = selection.edge[i];
    }
    virtual ~MoveSelectionAction() {delete[] edges;}
};

struct CloneSelectionAction: Action
{
    int edgeCount;
    Edge* firstEdge, *lastEdge;
    int type() {return CloneSelection;}
    CloneSelectionAction(const Selection& selection, const Vector& src, const Vector& dest)
        : edgeCount(selection.edgeCount)
        , firstEdge(NULL), lastEdge(NULL)
    {
        for(int i=0; i<edgeCount; i++){
            Edge* e = new Edge(*selection.edge[i]);
            *e += (dest - src);
            e->next = firstEdge, firstEdge = e;
            if(! lastEdge) lastEdge = e;
        }
    }
    virtual ~CloneSelectionAction()
    {
        Edge* e = firstEdge;
        while(e){
            Edge* n = (e==lastEdge) ? NULL : e->next;
            delete(e);
            e = n;
        }
    }
};

struct DeleteSelectionAction: Action
{
    int edgeCount;
    DeleteEdgeAction* edgeActs;
    int type() {return DeleteSelection;}
    DeleteSelectionAction(const Selection& selection)
        : edgeCount(selection.edgeCount)
        , edgeActs(edgeCount > 0 ? new DeleteEdgeAction[edgeCount] : NULL)
    {
    }
    virtual ~DeleteSelectionAction()
    {
        if(edgeActs) delete[] edgeActs;
    }
};

struct Box
{
    Vector min, max;
};


struct Pedestrian
{
	Pedestrian* next;
	Human human;
	Matrix matrix;
	Pedestrian(): next(NULL), matrix(UnitMatrix) {}
};

struct Tree
{
	Tree* next;
	Vector pos;
	float height;
	Tree(const Vector& p, float h): next(NULL), pos(p), height(h) {}
	Matrix croneMatrix() {
		float croneHeight = height * 0.7f;
		return UnitMatrix * Vector(croneHeight/4,croneHeight/2,croneHeight/4) 
			+ pos + Vector(0, height-croneHeight/2);
	}
};

struct Editor
{
	HWND window;
    Vector pivot;
    int snapGridSize;
    bool loopMode;
    Vector prevLoopPoint;
    unsigned int bestPick;
    bool snapGridOn;
    Selection selection;
    BoolPoint snapGridHit;
    Action* actions;
    bool selBoxIs;
    struct {Vector min, max;} selBox;
	RoadSystem roads;
	bool snapToGrid;
	enum Mode{
		MeshMode, RoadMode, BuildingMode
	};
	Mode mode;
	Building* buildings;
	Building* curBuilding;
	Pedestrian* peds;
	Ground ground;
	Tree* trees;
	
	Editor(HWND window)
		: window(window)
        , pivot(ZeroVector)
        , snapGridSize(128)
        , loopMode(false)
        , prevLoopPoint(ZeroVector)
        , bestPick(0)
        , snapGridOn(true)
        , snapGridHit(false)
        , actions(NULL)
        , selBoxIs(false)
		, snapToGrid(true)
		, mode(RoadMode)
		, buildings(NULL)
		, curBuilding(NULL)
		, peds(NULL)
		, trees(NULL)
    {
    }
    ~Editor()
    {
		DeleteList(trees);
		DeleteList(buildings);
        DeleteList(actions);
    }
    void onNewScene()
    {
        DeleteList(actions); actions = NULL;
        selection.clear();
        loopMode = false;
        bestPick = 0;
        selBoxIs = false;
    }
    void push(Action* a)
    {
        a->next = actions;
        actions = a;
    }
    void pop(Action* a)
    {
        assert(a == actions);
        if(a){
            actions = a->next;
            delete(a);
        }
    }
    void undoRedo(Action* a, Scene* scene, bool redo)
    {
        if(! a) return;
        if(a->type() == Action::PutEdge){
            PutEdgeAction* e = static_cast<PutEdgeAction*>(a);
            if(redo){
                scene->push(e->edge);
                prevLoopPoint = e->edge->B;
            }else{
                prevLoopPoint = e->edge->A;
                scene->pop(e->edge);
            }
        }
        if(a->type() == Action::PutSurf){
            PutSurfAction* s = static_cast<PutSurfAction*>(a);
            if(redo) scene->push(s->surf);
            else scene->pop(s->surf);
        }
        if(a->type() == Action::DeleteEdge){
            DeleteEdgeAction* d = static_cast<DeleteEdgeAction*>(a);
            if(redo){
                if(d->before){
                    d->before->next = d->edge->next;
                }else{
                    assert(scene->edges == d->edge);
                    scene->edges = d->edge->next;
                }
            }else{
                if(d->before) d->before->next = d->edge;
                else scene->edges = d->edge;
            }
        }
        if(a->type() == Action::DeleteSelection){
            DeleteSelectionAction* s = static_cast<DeleteSelectionAction*>(a);
            if(redo){
                for(int i=0; i<s->edgeCount; i++){
                    s->edgeActs[i].edge = selection.edge[i];
                    s->edgeActs[i].before = scene->edgeBefore(selection.edge[i]);
                    undoRedo(&s->edgeActs[i], scene, true);
                }
            }else{
                for(int i=s->edgeCount-1; i>=0; i--) undoRedo(&s->edgeActs[i], scene, false);
            }
        }
        if(a->type() == Action::MoveSelection){
            MoveSelectionAction* m = static_cast<MoveSelectionAction*>(a);
            Vector move = redo ? m->dest-m->src : m->src-m->dest;
            for(int i=0; i<m->edgeCount; i++) *m->edges[i] += move;
            pivot = redo ? m->dest : m->src;
        }
        if(a->type() == Action::CloneSelection){
            CloneSelectionAction* c = static_cast<CloneSelectionAction*>(a);
            if(redo){
                c->lastEdge->next = scene->edges;
                scene->edges = c->firstEdge;
            }else{
                scene->edges = c->lastEdge->next;
            }
        }
    }
    void pushAndRedo(Action* a, Scene* scene)
    {
        push(a);
        undoRedo(a, scene, true);
    }
    void undoAndPop(Action* a, Scene* scene)
    {
        undoRedo(a, scene, false);
        pop(a);
    }
	void finishBuilding(Building* b)
	{
		b->storyCount = 0;
		while(b->storyCount <= 0 || 999 < b->storyCount){
			const char* a = gui::AskForString("How many stories?", window);
			if(a){
				b->storyCount = atoi(a);
			}else{
				b->storyCount = 15;
			}
		}
		b->next = buildings; buildings = b;
		b->fixPointOrder();
		b->cutAngles();
		b->calcNormals();
		b->calcProfile();
	}
    void processInput(Scene* scene, const Vector& camera)
    {
        if(snapGridOn){
            if(Keys::pressed.leftBracket) if(snapGridSize > 1) snapGridSize /= 2;
            if(Keys::pressed.rightBracket) if(snapGridSize < 65536) snapGridSize *= 2;
        }
        if(Keys::pressed.c){
            BoolPoint h = snapPoint();
            if(h) pivot = h;
        }
        if(Keys::pressed.num1){
            BoolPoint h = snapPoint();
            if(h){
                if(loopMode){
                    Vector V = (const Vector&)(h) - prevLoopPoint;
                    if(square(V) > 0){
                        bool isLine = (square(V.x + V.y + V.z) == square(V));//only one coordinate is not zero
                        Edge* e = isLine ? new Edge(prevLoopPoint, h) : new Edge(calcArc(prevLoopPoint, h));
                        pushAndRedo(new PutEdgeAction(e), scene);
                    }
                }else{
                    prevLoopPoint = h;
                    loopMode = true;
                }
            }
        }
        if(Keys::pressed.escape){
            loopMode = false;
            selBoxIs = false;
			roads.drawNode = NULL;
        }
        if(Keys::pressed.num4){
			if(selection.edgeCount >= 3){
				Edge* e1 = selection.edge[selection.edgeCount-3];
				Edge* e2 = selection.edge[selection.edgeCount-2];
				Edge* e3 = selection.edge[selection.edgeCount-1];
				TorusSurf s;
				if(torusSurfletFromEdges(s, *e1, *e2, *e3, camera)){
					TorusSurf* newSurf = new TorusSurf;
					*newSurf = s;
					newSurf->type = TorusSurfType;
					pushAndRedo(new PutSurfAction(newSurf), scene);
				}
			}else if(selection.edgeCount >= 2){
				Edge* e1 = selection.edge[selection.edgeCount-2];
				Edge* e2 = selection.edge[selection.edgeCount-1];
				SurfFromEdges s(e1, e2, camera);
				Surf* S = NULL;
				if(s.surfType == SphereSurfType) S = new SphereSurf(s.sphere);
				if(s.surfType == QuadSurfType) S = new QuadSurf(s.quad);
				if(s.surfType == CylinderSurfType) S = new CylinderSurf(s.cylinder);
				if(s.surfType == TorusSurfType) S = new TorusSurf(s.torus);
				if(S) pushAndRedo(new PutSurfAction(S), scene);
			}
        }
		if(Keys::pressed.num5){
			BoolPoint p = snapPoint();
            if(p) roads.onClick(p);
		}
		if(Keys::pressed.num6){
			BoolPoint p = snapPoint();
            if(p) roads.onMove(p);
		}
		if(Keys::pressed.num7){
			BoolPoint p = snapPoint();
			if(p){
				if(! curBuilding) curBuilding = new Building();
				curBuilding->basePoints[curBuilding->basePointCount++] = p;
				if(curBuilding->basePointCount >= Building::MaxBasePoints){
					finishBuilding(curBuilding);
					curBuilding = NULL;
				}
			}
		}
		if(Keys::pressed.num8){
			BoolPoint p = snapPoint();
			if(p){
				Pedestrian* a = new Pedestrian();
				a->matrix = UnitMatrix.translate(p);
				a->next = peds; peds = a;
			}
		}
		if(Keys::pressed.num9){
			BoolPoint p = snapPoint(); 
			if(p) ground.addPoint(p);
		}
		if(Keys::pressed.num0){
			BoolPoint p = snapPoint(); 
			if(p){
				static RandomNumber random(723902);
				Tree* t = new Tree(p, (++random)(7.f, 10.f));
				t->next = trees; trees = t;
			}
		}
        if(Keys::pressed.z){
            undoAndPop(actions, scene);
        }
        if(Keys::pressed.x){
            if(selection.edgeCount > 0){
                pushAndRedo(new DeleteSelectionAction(selection), scene);
                selection.clear();
            }
        }
        if(Keys::pressed.m){
            if(selection.edgeCount > 0){
                BoolPoint dest = snapPoint();
                if(dest) pushAndRedo(new MoveSelectionAction(selection, pivot, dest), scene);
            }
        }
        if(Keys::pressed.k){
            if(selection.edgeCount > 0){
                BoolPoint dest = snapPoint();
                if(dest) pushAndRedo(new CloneSelectionAction(selection, pivot, dest), scene);
            }
        }
        if(Keys::pressed.b){
            BoolPoint p = snapPoint();
            if(p){
                if(selBoxIs){
					selBox.min = minimum(selBox.min, p.object);
					selBox.max = maximum(selBox.max, p.object);
                }else{
                    selBox.min = selBox.max = p;
                    selBoxIs = true;
                }
            }
        }
        if(Keys::pressed.Return){
            selectBox(selBox.min, selBox.max, scene);
            selBoxIs = false;
			roads.drawNode = NULL;
			if(curBuilding){
				finishBuilding(curBuilding);
				curBuilding = NULL;
			}
        }
    }
    bool boxHasPoint(const Vector& min, const Vector& max, const Vector& p)
    {
        return(min <= p && p <= max);
    }
    void selectBox(const Vector& min, const Vector& max, Scene* scene)
    {
        selection.clear();
        for(Edge* e=scene->edges; e!=NULL; e=e->next){
            if(boxHasPoint(min, max, e->A))
            if(boxHasPoint(min, max, e->B))
            //if(boxHasPoint(min, max, e->C))
                selection.push(e);
        }
    }
	BoolPoint snapPoint()
    {
        if(snapGridOn) return snapGridHit;
        if(! bestPick) return false;
        return *(const Vector*)bestPick;
    }
    BoolPoint calcSnapGridHit(const Vector& ray, const Plane& snapPlane, const Matrix& camera)
    {
        Vector hit = ZeroVector;
        if(! planeHitRay(snapPlane, camera.origin, ray, hit)) return false;
		if(snapToGrid){
			int i = maxDim(camera.axes.z);
			if(i != 0) hit.x = floor((hit.x - pivot.x) / float(snapGridSize) * 128 + .5f) * snapGridSize / 128 + pivot.x;
			if(i != 1) hit.y = floor((hit.y - pivot.y) / float(snapGridSize) * 128 + .5f) * snapGridSize / 128 + pivot.y;
			if(i != 2) hit.z = floor((hit.z - pivot.z) / float(snapGridSize) * 128 + .5f) * snapGridSize / 128 + pivot.z;
		}
        return hit;
    }
	void setMode(Mode value)
	{
		mode = value;
	}
};
