#ifndef MENUSETTING_H
#define MENUSETTINGS_H

#include "AppStateMenu.h"

class Settings: public Menu {
private:
	static bool s_bInstance;
	Settings();
	static Settings* s_pcMenuSettings;

public:
	static void vMakeInstance();
	static Settings* s_pcGetInstance();
	~Settings();

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
		BUTTON_TYPE_START,
		BUTTON_TYPE_BACK
	};
};

#endif