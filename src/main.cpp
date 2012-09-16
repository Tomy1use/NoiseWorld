#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#include <twoWayList.h>
#include <Color.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Math/Plane.h>
#include <Math/MatrixConst.h>
#include <input.h>
#include <error.h>
#include <Text.h>
#include <Math/MathFunc.h>
#include <Camera/FollowCamera.h>
#include <Input_win32.h>
#include <Math/VectorConst.h>
#include <Math/VectorFunc.h>
#include <Math/VanDerCorputSequence.h>
#include <Math/PlaneFunc.h>
#include <Math/FrustumFunc.h>
#include <Math/MatrixFunc.h>
#include <Math/AxesFunc.h>
#include <Win32/Cursor.h>
#include <Win32/CursorInput.h>
#include <DrawThings.h>
#include <ColorFromNormal.h>
#include <MultiPointLine.h>
#include <Geometry/Icosahedron.h>
#include <Geometry/Sphere.h>
#include <WriteGif.h>
#include <Math/RandomNumber.h>
#include <GL/RenderContext.h>
#include <gl/gl.h>
#include <glFunc.h>
#include <Strip.h>
#include <DataFile.h>
#include <GL/ExtensionFunctions.h>
#include <glGeneralCylinder.h>
extern void glCrossPlanes(const Vector& P, float S);
#include <args.h>
#include <BoxSide.h>
#include <glCylinder.h>
extern void glFastBox(float L, float B, float K, float R, float T, float F);
extern void glFastBox(const Vector& Min, const Vector& Max);
extern void glTexBox(float L, float B, float K, float R, float T, float F);
extern void glTexBox(const Vector& Min, const Vector& Max);
#include <ZoomDragMatrix.h>
#include <glArc.h>
#include <glFastOcta.h>
void glSphereQuarter(const Vector& K, const Vector& A, const Vector& B, const Vector& C,  float radius, int depth);
#include <ListFunc.h>
#include <RayThroughCursor.h>
#include <Menu/MenuConf.h>
#include <Menu/MenuExec.h>
#include <AboutBox.h>
#include <FileEnvironment.h>
#include <IcosahedronRenderPiece.h>
#include <GL/ShaderFunctions.h>
extern void makeScreenshot(HWND window, const char* fileName);
extern GLuint subdivideIcosahedronDisplayList(int depth);
extern int createAtmosphereDisplayList();
extern void centerWindow(HWND window, int width, int height);

extern void CreateWorld();
extern void RenderWorld(const Matrix& cameraMatrix, float aspect, float fov, const Vector& vLightDir);
extern void RenderWorldGUI();
extern bool WorldMouseMove(float x, float y);
extern void WorldLeftMouseUp();
extern void WorldKey(int k);

#include <PerlinNoise.h>

static const float HumanSpeed = 1.2f;
static const float CarSpeed = 16.f;
static const float JetSpeed = 100;


struct View
{
	HWND window;
	gl::RenderContext renderContext;
	View(int width, int height)
		: window(createWindow(width, height))
		, renderContext(window)
	{
	}
	HWND createWindow(int width, int height)
	{
		const char* ClassName = "NoiseWorldClass";
		const char* Title = "Noise World";
		WNDCLASSEX c = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L, 
			GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
			ClassName, NULL };
		c.hCursor = LoadCursor(NULL, IDC_ARROW);
		RegisterClassEx(&c);
		return CreateWindow(ClassName, Title, 
			WS_OVERLAPPEDWINDOW, 0, 0, width, height,
			GetDesktopWindow(), NULL, c.hInstance, NULL);
	}
};


struct Game
{
	AboutBox aboutBox;
	MenuConf menuConf;
	MenuExec menuExec;
	FileEnvironment fileEnv;
	View view;
    HFONT font;
    Text textMsec;
    Text textCursor;
    int frameIndex;
    Matrix cameraMatrix;
	float cameraSpeed;
    CursorInput cursorInput;
    float fullTime;
    unsigned int bgColor;
    Vector sunDir, sunPos;
    bool isFogOn, isShadowOn, isShadowDrawn;
    float fov;
    ZoomDragMatrix zoomDragMatrix;
	GLint atmoDispList;
	GLuint oneLightProgram;
	GLuint oneLightSpecEnvProgram;
	GLuint oneLightShadowProgram;
	GLuint oneLightMoonProgram;
	GLuint addShineProgram;
	GLuint checkerProgram;
	GLuint subIcoDispList0;
	GLuint subIcoDispList1;
	GLuint subIcoDispList2;
	GLuint subIcoDispList3;
	bool showEdges;
	Vector pivot;
	int snapGridSize;
	GLuint moonTex;
    
	Game(int width, int height)
		: aboutBox("about.txt")
		, menuConf("menu.conf")
		, menuExec(&menuConf)
		, fileEnv("recent.txt")
        , view(width, height)
		, font(NULL)
        , frameIndex(0)
        , cameraMatrix(matrixLookAt(Vector(3, 5000, 30), ZeroVector))
		, cameraSpeed(CarSpeed)
        , fullTime(0)
        , bgColor(0xff00FFff)
        , sunDir(unit(Vector(-1, -4, -2)))
		, sunPos(-sunDir * 50)
        , isFogOn(false), isShadowOn(false), isShadowDrawn(false)
        , fov(Pi/3)
		, subIcoDispList0(subdivideIcosahedronDisplayList(0))
		, subIcoDispList1(subdivideIcosahedronDisplayList(1))
		, subIcoDispList2(subdivideIcosahedronDisplayList(2))
		, subIcoDispList3(subdivideIcosahedronDisplayList(3))
		, showEdges(true)
		, pivot(ZeroVector)
		, snapGridSize(128)
    {
		resolveExtensionFunctions();
        textMsec.setFont("Arial", 15);
        textMsec.setPoint(0, 0);
        textCursor.setFont("Arial", 13);
        centerWindow(view.window, width, height);
        ShowWindow(view.window, SW_NORMAL);
        printf("OpenGL version %s\n", glGetString(GL_VERSION));
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//oneLightProgram = createShader("Shaders/OneLightVertexShader.glsl", "Shaders/OneLightPixelShader.glsl", NULL);
		//oneLightSpecEnvProgram = createShader("Shaders/OneLight_Spec_Env_VertexShader.glsl", "Shaders/OneLight_Spec_Env_PixelShader.glsl", NULL);
		oneLightMoonProgram = createShader("Shaders/OneLightMoonVertexShader.glsl", "Shaders/OneLightMoonPixelShader.glsl", NULL);
		addShineProgram = createShader("Shaders/AdditiveShinyVertexShader.glsl", "Shaders/AdditiveShinyPixelShader.glsl", NULL);
		//checkerProgram = createShader("Shaders/CheckerVertexShader.glsl", "Shaders/CheckerPixelShader.glsl", NULL);
		atmoDispList = createAtmosphereDisplayList();
		CreateWorld();
		
		{
			PerlinNoise per;
			const int TexReso = 32;
			float* texMap = new float[TexReso*TexReso*TexReso];
			float minT=100, maxT=-100;
			for(int z=0; z<TexReso; z++){
				for(int y=0; y<TexReso; y++){
					for(int x=0; x<TexReso; x++){
						float f = float(rand()) / float(RAND_MAX);
						float X = float(x)/float(TexReso), Y = float(y)/float(TexReso), Z = float(z)/float(TexReso);
						float t = per(X*32, Y*32, Z*32) / 32 + per(X*15, Y*16, Z*16) / 16 + per(X*8, Y*8, Z*8) / 8 + per(X*4, Y*4, Z*4) / 4;
						texMap[z*TexReso*TexReso+y*TexReso+x] = t;
						if(t < minT) minT = t;
						if(maxT < t) maxT = t;
					}
				}
			}
			for(int i=0; i<TexReso*TexReso*TexReso; i++){
				texMap[i] = (texMap[i] - minT) / (maxT - minT);
			}
			glGenTextures(1, &moonTex);
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, moonTex);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, TexReso, TexReso, TexReso, 0, GL_LUMINANCE, GL_FLOAT, texMap);
			glDisable(GL_TEXTURE_3D);
			delete[] texMap;
		}
    }
	~Game()
    {
    }
	void update(float dt)
    {
		fullTime += dt;
        frameIndex ++;
        {
            //performance counter
            const int count = 33;
            static char text[33];
            static int msec[count];
            static int index = 0;
            msec[index] = int(dt * 1000);
            index = (index + 1) % count;
            int average = 0;
            for(int i=0; i<count; i++){
                average += msec[i];
            }
            average /= count;
            if(average){
                sprintf(text, "%d ms (%d fps)", average, 1000/average);
                textMsec.setText(text);
            }
        }
        static const float MaxDeltaT = 1.f / 30.f;
        dt = dt > MaxDeltaT ? MaxDeltaT : dt;
        cursorInput.beginFrame();
		bool guiHas = false;
        {
            MSG m;
            if(PeekMessage(&m, NULL, 0, 0, PM_REMOVE)){
                TranslateMessage(&m);
                DispatchMessage(&m);
                if(m.hwnd == view.window){
                    cursorInput.message(m);
					if(m.message == WM_LBUTTONUP){
						WorldLeftMouseUp();
					}
					if(m.message == WM_MOUSEMOVE){
						if(m.wParam & MK_LBUTTON){
							int x = GET_X_LPARAM(m.lParam);
							int y = GET_Y_LPARAM(m.lParam);
							RECT rect;
							GetClientRect(m.hwnd, &rect);
							guiHas = WorldMouseMove(float(x)/float(rect.right), float(y)/float(rect.bottom));
						}
					}
                    if(m.message == WM_RBUTTONUP){
                        if(cursorInput.x == cursorInput.rightPressX && cursorInput.y == cursorInput.rightPressY){
							win32::Cursor cursor;
							MenuExec::Result result = menuExec.execute(cursor.point.x, cursor.point.y, m.hwnd);
							printf("MenuExec::execute-> %s\n", (const char*)(result));
							if(result == "about") aboutBox.display(m.hwnd);
							if(result == "exit") PostMessage(m.hwnd, WM_CLOSE, 0, 0);
							if(result == "csHuman") cameraSpeed = HumanSpeed;
							if(result == "csCar") cameraSpeed = CarSpeed;
							if(result == "csJet") cameraSpeed = JetSpeed; 
							if(result == "camSaveAs"){ 
								if(fileEnv.saveFileDialog(m.hwnd)){
									DataFile camFile(fopen(fileEnv.saveFileName(), "w"));
									camFile << cameraMatrix;
								}
							}
							if(result == "toggleEdges") showEdges = !showEdges;
							if(result == "screenshot") makeScreenshot(m.hwnd, NULL);
                        }
                    }
                    Input::processMessage(m.hwnd, m.message, m.wParam, m.lParam);
                }
            }
        }
		if(! guiHas){
			cameraMatrix = zoomDragMatrix.update(view.window, fov, snapPlane(), pivot, cameraMatrix, cursorInput, dt);
		}
        cameraMatrix = moveFreeMatrix(cameraMatrix, dt, cameraSpeed, 2);
		if(Keys::pressed.r) WorldKey('r');
		if(Keys::pressed.f) WorldKey('f');
		if(Keys::pressed.t) WorldKey('t');
    }
    void drawPivot(const Vector& pivot, const Vector& camera)
    {
        float S = length(camera - pivot) / 100;
        const float K = .5f * S, A = 2.f * S, W = .2f * S;
        glColor3f(1,1,1); glFastBox(pivot-Vector(K), pivot+Vector(K));
        glColor3f(1,0,0); glFastBox(pivot-Vector(A,W,W), pivot+Vector(A,W,W));
        glColor3f(0,1,0); glFastBox(pivot-Vector(W,A,W), pivot+Vector(W,A,W));
        glColor3f(0,.5f,1); glFastBox(pivot-Vector(W,W,A), pivot+Vector(W,W,A));
    }
    Plane snapPlane()
    {
		return Plane(Vector(0,1,0), 0);
        Vector z = nearestAxis(cameraMatrix.axes.z);
        //Vector z = nearestAxis(cameraMatrix.origin - pivot);
        return planeFromPointNormal(pivot, z);
    }
    void drawSnapPlane(const Plane& plane, float alpha)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        Matrix m = matrixFromOriginAxisZ(pivot, plane.n);
        const float S = 15;
        glColor4f(1,1,1, alpha);
        glBegin(GL_QUADS);
            const float u[] = {-1, +1, +1, -1}, v[4] = {-1, -1, +1, +1};
            for(int i=0; i<4; i++){
                float U = u[i] * S, V = v[i] * S;
                glTexCoord2f(U, V);
                glVertex(m.out(Vector(U*snapGridSize/128, V*snapGridSize/128, 0)));
            }
        glEnd();
        
        glDisable(GL_BLEND);
    }
    void drawGL()
    {
        RECT clientRect;
        GetClientRect(view.window, &clientRect);
        
		glClearColor(0, 0, 0, 0);
        glColorMask(1, 1, 1, 1);
        glDepthMask(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glDisable(GL_TEXTURE_2D);
        glFrontFace(GL_CCW);
        if(1){
			setupViewerProjection(clientRect);
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrix(Matrix(cameraMatrix.axes, ZeroVector).in(UnitMatrix));
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glEnable(GL_CULL_FACE);
			if(1){//moon
				glCullFace(GL_BACK);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_3D, moonTex);
				glUseProgram(oneLightMoonProgram);
				setShaderVector(oneLightMoonProgram, "vLightDir", cameraMatrix.axes.in(sunDir));
				setShaderTexture(oneLightMoonProgram, "MoonMap", 0);
				glPushMatrix();
				glTranslate(Vector(-10, 3, 7));
					glColor3f(.3f, .3f, .3f);
					glCallList(subIcoDispList3);
				glPopMatrix();
				glDisable(GL_TEXTURE_3D);
			}
			if(1){//sun
				glCullFace(GL_BACK);
				glUseProgram(addShineProgram);
				glPushMatrix();
					glTranslate(-sunDir * 20);
					glColor3f(.3f, .3f, 0);
					glCallList(subIcoDispList2);
				glPopMatrix();
			}
			if(1){//atmosphere
				glUseProgram(0);
				glDisable(GL_FOG);
				glCullFace(GL_FRONT);
				glShadeModel(GL_SMOOTH);
				glPushMatrix();
					glScalef(5,5,5);
					glCallList(atmoDispList);
				glPopMatrix();
			}
			if(1){//high altitude clouds
				glUseProgram(0);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);
				extern GLuint cloudsTexture;
				glBindTexture(GL_TEXTURE_2D, cloudsTexture);
				glDisable(GL_FOG);
				glShadeModel(GL_SMOOTH);
				glColor4f(1,1,1,.5f);
				glPushMatrix();
					glScalef(55,3,55);
					//draw canopy
					const int ResoX = 9, ResoZ = 9;
					for(int j=0; j<ResoZ-1; j++){
						float v1 = float(j) / float(ResoZ-1);
						float v2 = float(j+1) / float(ResoZ-1);
						float z1 = lerp(-1.f, 1.f, v1);
						float z2 = lerp(-1.f, 1.f, v2);
						glBegin(GL_QUAD_STRIP);
							for(int i=0; i<ResoX; i++){
								float u = float(i) / float(ResoX-1);
								float x = lerp(-1.f, 1.f, u);
								float y1 = (1-x*x) * (1-z1*z1);
								float y2 = (1-x*x) * (1-z2*z2);
								glTexCoord2f(u, v1);
								glVertex3f(x, y1, z1);
								glTexCoord2f(u, v2);
								glVertex3f(x, y2, z2);
							}
						glEnd();
					}
				glPopMatrix();
			}
		}
		glUseProgram(0);
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		{
			setupViewerProjection(clientRect);
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrix(cameraMatrix.in(UnitMatrix));
		}
        
		drawPivot(pivot, cameraMatrix.origin);
        //drawSnapCursor();
		
		glUseProgram(0);
		glDisable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		RenderWorld(cameraMatrix, float(clientRect.right)/float(clientRect.bottom), fov, cameraMatrix.axes.in(sunDir));
		glDisable(GL_TEXTURE_2D);

		{
			//draw GUI
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			RenderWorldGUI();
		}
		
        glFlush();
		glFinish();
        
		HDC dc = GetDC(view.window);
        SwapBuffers(dc);
        ReleaseDC(view.window, dc);
    }
    void setupViewerProjection(const RECT& clientRect)
    {
        glViewport(0, 0, clientRect.right, clientRect.bottom);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float Near = 1.f, Far = 99999.f;
		float aspect = float(clientRect.right)/float(clientRect.bottom);
		float F = tanf(fov/2);
		float L = -F*Near, R = +F*Near;
		float B = -F*Near/aspect, T = +F*Near/aspect;
		glFrustum(L, R, B, T, Near, Far);
    }
	void getViewProjMatrix(const Matrix& cameraMatrix, float viewProjMatrix[16])
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glMultMatrix(cameraMatrix.in(UnitMatrix));
			glGetFloatv(GL_PROJECTION_MATRIX, viewProjMatrix);
		glPopMatrix();
	}
    void setupPickMatrix(const RECT& clientRect, const CursorInput& cursor)
    {
        //glViewport(0, 0, clientRect.right, clientRect.bottom);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float Near = .1f;
        float aspect = float(clientRect.right)/float(clientRect.bottom);
        float WTF = 1.15f;//WTF is this number???
        float x = (float(cursor.x)/float(clientRect.right)-.5f) * WTF;
        float y = (float(cursor.y)/float(clientRect.bottom)-.5f) * WTF;
        const float S = .001f;
        float L = (x - S) * Near, R = (x + S)*Near;
        float T = (-y+S)*Near/aspect, B = (-y-S)*Near/aspect;
        glFrustum(L, R, B, T, Near, 9999.f);
    }
    struct PickPass
    {
        static const int PickBufferSize = 256;
        unsigned int pickBuffer[PickBufferSize];
        void begin()
        {
            glSelectBuffer(PickBufferSize, pickBuffer);
            glRenderMode(GL_SELECT);
        }
        int end()
        {
            int hitCount = glRenderMode(GL_RENDER);
            return nearestPick(hitCount, pickBuffer);
        }
        unsigned int nearestPick(int pickCount, const unsigned int pickBuffer[])
        {
            if(pickCount <= 0) return 0;
            const unsigned int* pick = pickBuffer;
            unsigned int bestPick = pick[3], bestDepth = pick[1];
            for(int i=0; i<pickCount; i++){
                unsigned int nameCount = pick[0], minDepth = pick[1];//, maxDepth = pick[2];
                if(minDepth < bestDepth) bestPick = pick[3], bestDepth = minDepth;
                pick += 3 + nameCount;
            }
            return bestPick;
        }
    };
	void glRect2(float x1, float y1, float x2, float y2)
    {
        assert(x1 < x2 && y1 < y2);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(x1, y1, 0);
            glTexCoord2f(0, 1); glVertex3f(x1, y2, 0);
            glTexCoord2f(1, 1); glVertex3f(x2, y2, 0);
            glTexCoord2f(1, 0); glVertex3f(x2, y1, 0);
        glEnd();
    }
    bool isDone()
    {
        if(! IsWindow(view.window)) return true;
        return false;
    }
    #include <MoveFreeMatrix.h>
};


int main(int argc, char* argv[])
{
	extern void parseDataFile(const char* fileName);
	extern void parseTokenStream(const char* fileName);
	//parseDataFile("SampleDataFile.txt");
	//parseTokenStream("SampleDataFile.txt");
	//return 0;
	
	Args args(argc, argv);
    int w = 800, h = 600;
	srand(unsigned(time(0)));
    if(args.count > 3){
        w = atoi(argv[1]);
        h = atoi(argv[2]);
    }
    Game game(w, h);
	
	float oldTime = 0;
	float curTime = 0;
	while(true){
		srand((unsigned)time(NULL));
		curTime = clock() / (float)CLK_TCK;
		float dt = curTime - oldTime;
		oldTime = curTime;
		if(dt <= 0){
			Sleep(5);
			continue;
		}
		Keys::process();
		Mouse::process();
		game.update(dt);
        game.drawGL();
        if(game.isDone()){
            break;
        }
	}
	return 0;
}
