#ifndef EVENTAPP_H
#define EVENTAPP_H

#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <SDL.h>

#include "EventAppState.h"

//Primarily functions as the application's Hub.  Routes Events and other data to the active/correct module.
class App: public Event {
private:
	static bool s_bInstance;
	App(int nWidth = 1280, int nHeight = 960);
	int nAppState;

public:
	static App* s_pcApp;
	static void vMakeInstance();
	~App();

	static AppState* s_pcAppState;

	int nWidth, nHeight;
	bool bRunning;

	SDL_Surface* psSurfDisplay;

	int nOnRun();
	bool bOnInit();

	void vOnEvent(SDL_Event* ptEvent);
	void vOnLoop();
	void vOnRender();

	void vSetAppState(int nAppStateID);
	AppState* pcGetAppState();

	void vOnCleanup();
	void vOnExit();
	void vOnResize(int nW, int nH);

	enum {
		APP_STATE_NONE,
		APP_STATE_MENU_MAIN,
		APP_STATE_MENU_OPTIONS,
		APP_STATE_MENU_LOAD,
		APP_STATE_MENU_PAUSE,
		APP_STATE_MENU_WORLD_SETTINGS,
		APP_STATE_WORLD
	};
};

#endif