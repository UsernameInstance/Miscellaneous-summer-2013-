#include "App.h"
#include "ModulePause.h"
#include "ModuleWorld.h"

App* App::s_pcApp = NULL;
bool App::s_bInstance = false;

App* App::Instance(int W, int H) {
	if(!s_bInstance) {
		s_pcApp = new App(W, H);
		s_bInstance = true;
	}
	return s_pcApp;
}
App::App(int W, int H) {
	if(W < 1 || W > 1920) W = 640;
	if(H < 1 || H > 1080) H = 480;
	iW = W; iH = H;
	bInit = false;
	bRunning = false;
	pcModuleActive = NULL;
}

App::~App() {
	s_bInstance = false;
}

void App::Init() {
	if(!bInit) {
		atexit(SDL_Quit);

		if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
			exit(1);
		}

		psSurfDisp = NULL;
		psSurfDisp = SDL_SetVideoMode(iW, iH, 32, SDL_SWSURFACE);
		if(psSurfDisp == NULL) {
			fprintf(stderr, "Unable to set %dx%d video: %s\n", iW, iH, SDL_GetError());
			exit(1);
		}

		SetState(STATE_WORLD);
	}
	else fprintf(stderr, "App already initialized.\n");
}

void App::Run() { 
	if(pcModuleActive) bRunning = true;
	int iLTime = SDL_GetTicks();
	while(bRunning) {
		pcModuleActive->HandleEvents(&sEvent);
		if(pcModuleActive) {
			pcModuleActive->Loop();
			if(int(SDL_GetTicks()) > iLTime + 17) iLTime = SDL_GetTicks(), pcModuleActive->Render();
			SDL_UpdateRect(psSurfDisp, 0, 0, 0, 0);
		}
		else bRunning = false;	
	}

	Cleanup();
}

void App::Cleanup() {
	SDL_Quit();
}

void App::SetState(int iState) {
	if(pcModuleActive) pcModuleActive->Deactivate();

	switch(iState) {
		case STATE_NONE:
			pcModuleActive = NULL;
			break;
		case STATE_WORLD:
			pcModuleActive = World::Instance();
			break;
		case STATE_PAUSE:
			pcModuleActive = Pause::Instance();
			break;
	}

	if(pcModuleActive) pcModuleActive->Activate();
}

Module* App::GetState() { return pcModuleActive; }

