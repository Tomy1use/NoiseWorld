#pragma once

#include <DataFile.h>

struct SceneFile
{
	void saveScene(const Scene* scene, const char* fileName)
	{
		DataFile f(fopen(fileName, "w"));
		if(! f) {printf("Failed open %s\n", fileName); return;}
		f << "scene {\n";
		if(scene->edges){
			f << "edges [\n";
			for(Edge* e=scene->edges; e!=NULL; e=e->next){
				if(e->R){
					f << "{c" << e->C << "a" << e->A << "b" << e->B << "}\n";
				}else{
					f << "{a" << e->A << "b" << e->B << "}\n";
				}
			}
			f << "]\n";
		}
		if(scene->surfs){
			f << "surfs [\n";
			for(Surf* s=scene->surfs; s!=NULL; s=s->next){
				f << '{';
				if(s->type == SphereSurfType){
					SphereSurf* S = static_cast<SphereSurf*>(s);
					f << "type Sphere, center" << S->k << "a" << S->a << "b" << S->b << "c" << S->c << "radius" << S->r;
				}
				if(s->type == QuadSurfType){
					QuadSurf* S = static_cast<QuadSurf*>(s);
					f << "type Quad, a" << S->a << "b" << S->b << "c" << S->c << "d" << S->d;
				}
				if(s->type == CylinderSurfType){
					CylinderSurf* S = static_cast<CylinderSurf*>(s);
					f << "type Cylinder";
					f << "a1" << S->a1 << "b1" << S->b1 << "c1" << S->c1;
					f << "a2" << S->a2 << "b2" << S->b2 << "c2" << S->c2;
					f << "radius" << S->r;
				}
				if(s->type == TorusSurfType){
					f << "type Torus";
					//TODO
				}
				f << "}\n";
			}
			f << "]\n";
		}
		f << "}\n";
		fclose(f);
		char s[MAX_PATH]; sprintf(s, "Saved %s", fileName);
		MessageBox(0, s, "Shapecraft", 0);
	}
};
