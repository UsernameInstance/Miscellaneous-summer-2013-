#ifndef MENUTITLE_H
#define MENUTITLE_H

#include "AppStateMenu.h"

class Title: public Menu {
private:
	static bool s_bInstance;
	Title();
	static Title* s_pcMenuTitle;

public:
	static void vMakeInstance();
	static Title* s_pcGetInstance();
	~Title();
	
	//void vOnInit(int nX, int nY, int nWidth = 640, int nHeight = 480, char* pchFile = NULL);
	void vOnInit(int nDispWidth, int nDispHeight);

	void vOnEvent(SDL_Event* ptEvent);
	void vOnLButtonUp(int nMX, int nMY);

	void vOnActivate();
	void vOnDeactivate();
	void vOnLoop();
	void vOnRender(SDL_Surface* psSurfDisplay);

	void vOnResize(int nW, int nH);

	enum { //button TypeIDs go here
		BUTTON_TYPE_NONE,
		BUTTON_TYPE_NEW,
		BUTTON_TYPE_LOAD,
		BUTTON_TYPE_OPTIONS,
		BUTTON_TYPE_EXIT
	};
};


#endif