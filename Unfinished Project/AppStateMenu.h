#ifndef APPSTATEMENU_H
#define APPSTATEMENU_H

#include <SDL.h>
#include <vector>
#include "EventAppState.h"
#include "Button.h"

class Menu: public AppState {
public:
	Menu(int nX = 0, int nY = 0, int nWidth = 640, int nHeight = 480); //OnInit handles graphics.
	~Menu();

	int nX, nY; //coordinates relative to display.  In pixels.
	int nMX, nMY; //Last recorded mouse coordinates.
	int nDispWidth, nDispHeight; //App's Display Width & Height

	int nWidth, nHeight;

	SDL_Surface* psSurfBackground;

	bool bRunning;

	std::vector<Button*> cVecButtonList;

	void vAddButton(int nTypeID, int nX, int nY, int nWidth, int nHeight, char* pchFile);

	//functions should be overwritten in derived classes.
	virtual void vOnInit(int nDispWidth, int nDispHeight);
	virtual void vOnEvent(SDL_Event* ptEvent);
	virtual void vOnLButtonDown(int nMX, int nMY);
	virtual void vOnLButtonUp(int nMX, int nMY);

	virtual void vOnMouseMove(int nMX, int nMY, int nRelX, int nRelY, bool bLeft, bool bRight, bool bMiddle);

	virtual void vOnResize(int nW, int nH) = 0;

	virtual void vOnActivate() = 0;

	virtual void vOnDeactivate() = 0;

	virtual void vOnLoop() = 0;

	virtual void vOnRender(SDL_Surface* psSurfDisplay);

	//use enum to define button TypeID's here.

};

#endif