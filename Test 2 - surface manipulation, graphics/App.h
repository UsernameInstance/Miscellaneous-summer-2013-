#ifndef APP_H
#define APP_H

#include <stdlib.h>
#include "EventModule.h"

//serve as hub
class App {
private:
	App(int W = 640, int H = 480);
	
	static App* s_pcApp;
	static bool s_bInstance;
	
	SDL_Event sEvent;
	
	int iW, iH;
	bool bInit;
	bool bRunning;
	Module* pcModuleActive;

public:
	static App* Instance(int W = 640, int H = 480);
	~App();

	SDL_Surface* psSurfDisp;
	
	void Init();
	void Run();

	void Cleanup();

	void SetState(int iState);
	Module* GetState();

	enum state {
		STATE_NONE,
		STATE_WORLD,
		STATE_PAUSE
	};
};

#endif