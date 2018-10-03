#include "EventApp.h"
//Include derived class AppState header files here
#include "MenuTitle.h"

AppState* App::s_pcAppState = NULL;

bool App::s_bInstance = false;
App* App::s_pcApp = NULL;

void App::vMakeInstance() {
	if(!s_bInstance) {
		s_pcApp = new App();
		s_bInstance = true;
	}
}

App::App(int nWidth, int nHeight) {
	psSurfDisplay = NULL;
	bRunning = true;
	this->nWidth = nWidth;
	this->nHeight = nHeight;
}

App::~App() {
	s_bInstance = false;
}

bool App::bOnInit() {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;

	if((psSurfDisplay = SDL_SetVideoMode(nWidth, nHeight, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE)) == NULL) return false;

	SDL_WM_SetCaption("Project 1 v 1", NULL);

	SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

	//make instances of other singletons
	Title::vMakeInstance();
	Title::s_pcGetInstance()->vOnInit(nWidth, nHeight);
	vSetAppState(APP_STATE_MENU_MAIN);

	return true;
}

int App::nOnRun() {
	if(!bOnInit()) throw "Application Initialization Failure.";

	SDL_Event tEvent;

	while(bRunning) {
		while(SDL_PollEvent(&tEvent)) {
			vOnEvent(&tEvent);
		}

		vOnLoop();
		vOnRender();
	}

	vOnCleanup();

	return 0;
}


void App::vOnEvent(SDL_Event* ptEvent) {
	if(ptEvent->type == SDL_QUIT || ptEvent->type == SDL_VIDEORESIZE) Event::vOnEvent(ptEvent); //Only used in order to detect if "x" has been used to shut window/program.

	//Direct ptEvent to the correct program module (based on the App's state).
	if(s_pcAppState) s_pcAppState->vOnEvent(ptEvent);
}

void App::vOnLoop() {
	s_pcAppState->vOnLoop();
	//Call vOnLoop() of the appropriate module based on the App's state.
}


void App::vOnRender() {
	SDL_FillRect(psSurfDisplay, NULL, 0); //temporary
	s_pcAppState->vOnRender(psSurfDisplay);
	SDL_Flip(psSurfDisplay);
}

void App::vOnExit() {
	bRunning = false;
}

void App::vOnCleanup() {
	SDL_FreeSurface(psSurfDisplay);

	SDL_Quit();
}

void App::vSetAppState(int nAppStateID) {
	if(s_pcAppState) s_pcAppState->vOnDeactivate();

	switch (nAppStateID) {
		case APP_STATE_MENU_MAIN:
			s_pcAppState = Title::s_pcGetInstance();
			break;
		case APP_STATE_MENU_OPTIONS:
			break;
			//etc...
	}

	if(s_pcAppState) s_pcAppState->vOnActivate();
	nAppState = nAppStateID;
}


AppState* App::pcGetAppState() { 
	return s_pcAppState; //needed???
}

void App::vOnResize(int nW, int nH) {
		if((psSurfDisplay = SDL_SetVideoMode(nW, nH, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE)) == NULL) {
			std::cerr << "Window resize failure.";
			assert(0);
		}
		else {
			nWidth = nW;
			nHeight = nH;

			Title::s_pcGetInstance()->vOnResize(nWidth, nHeight);
		}
}