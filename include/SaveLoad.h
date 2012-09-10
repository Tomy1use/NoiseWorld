#pragma once

static const unsigned MaxEdgeCount = 999999;
static const unsigned MaxSurfCount = 999999;

struct SaveLoad
{
    struct File
    {
        FILE* f;
        bool ok;
        File(FILE* f): f(f), ok(true) {}
        ~File() {if(f) fclose(f);}
        operator FILE* () {return f;}
        template<typename Foo> 
        File& operator << (const Foo& foo)
        {
            ok &= (fwrite(&foo, sizeof(Foo), 1, f) == 1);
            return *this;
        }
        template<typename Foo> 
        File& operator >> (Foo& foo)
        {
            ok &= (fread(&foo, sizeof(Foo), 1, f) == 1);
            return *this;
        }
    };
    char lastPath[MAX_PATH];
    SaveLoad()
    {
        lastPath[0] = 0;
    }
    Scene* loadScene(const char* filename)
    {
        File f(fopen(filename, "rb"));
        if(! f){
            printf("Failed open %s\n", filename);
            return NULL;
        }
        unsigned edgeCount = 0;
        if(! (f >> edgeCount).ok){
            printf("Failed read edge count\n");
            return NULL;
        }
        if(edgeCount >= MaxEdgeCount){
            printf("Edge count is wrong\n");
            return NULL;
        }
        Scene* scene = new Scene();
        for(unsigned i=0; i<edgeCount; i++){
            Vector C, A, B;
            float R = 0;
            if(! (f >> C >> A >> B >> R).ok){
                printf("Failed read edge. Scene is incomplete\n");
                return scene;
            }
            scene->push(new Edge(C, A, B, R));
        }
        if(feof(f)){
            return scene;
        }
        unsigned surfCount = 0;
        if(! (f >> surfCount).ok){
            printf("Failed read surf count\n");
            return scene;
        }
        if(surfCount >= MaxSurfCount){
            printf("Surf count is wrong\n");
            return scene;
        }
        for(unsigned i=0; i<surfCount; i++){
            int surfType = 0;
            if(! (f >> surfType).ok){
                printf("Failed read surfType. Scene is incomplete\n");
                return scene;
            }
            if(surfType == SphereSurfType){
                if(! tryReadSurf<SphereSurf>(f, scene)) return scene;
            }else if(surfType == QuadSurfType){
                if(! tryReadSurf<QuadSurf>(f, scene)) return scene;
            }else if(surfType == CylinderSurfType){
                if(! tryReadSurf<CylinderSurf>(f, scene)) return scene;
            }else if(surfType == TorusSurfType){
                if(! tryReadSurf<TorusSurf>(f, scene)) return scene;
            }else{
                printf("Unknown surf type. Scene is incomplete\n");
                return scene;
            }
        }
        return scene;
    }
    template<typename SurfClass>
    bool tryReadSurf(File& f, Scene* scene)
    {
        SurfClass s;
        if(! (f >> s).ok){
            printf("Failed read surf. Scene is incomplete\n");
            return false;
        }
        scene->push(new SurfClass(s));
        return true;
    }
    void saveScene(const Scene* scene, const char* filename)
    {
        if(! (scene && scene->edges)){
            MessageBox(0, "Scene is empty, not saving", "Shapecraft", 0);
            return;
        }
        File f(fopen(filename, "wb"));
        if(! f){
            printf("Failed open %s\n", filename);
            return;
        }
        f << scene->edgeCount();
        for(Edge* e=scene->edges; e; e=e->next){
            f << e->C << e->A << e->B << e->R;
        }
        f << scene->surfCount();
        for(Surf* s=scene->surfs; s; s=s->next){
            f << s->type;//Yes, surfType will be duplicated. So it is easier to parse file when reading.
            if(s->type == SphereSurfType) f << *static_cast<SphereSurf*>(s);
            if(s->type == QuadSurfType) f << *static_cast<QuadSurf*>(s);
            if(s->type == CylinderSurfType) f << *static_cast<CylinderSurf*>(s);
            if(s->type == TorusSurfType) f << *static_cast<TorusSurf*>(s);
        }
        MessageBox(0, "Saved scene", "Shapecraft", 0);
    }
};
