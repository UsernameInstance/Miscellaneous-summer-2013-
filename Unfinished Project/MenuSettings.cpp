#include "MenuSettings.h"
#include <iostream>
#include <assert.h>
#include "EventApp.h"


bool Settings::s_bInstance = false;
Settings* Settings::s_pcMenuSettings = NULL;

void Settings::vMakeInstance() {
	if(!s_bInstance) {
		s_pcMenuSettings = new Settings();
		s_bInstance = true;
	}
}

Settings::Settings() {
	//OnInit Handles this
}

Settings::~Settings() {
	//parent class free's surfaces and deletes buttons
	s_bInstance = false;
	bRunning = false;
}


void Settings::vOnInit(int nDispWidth, int nDispHeight) {
	//Image and button loading done in OnActivate() for performance
	this->nDispWidth = nDispWidth;
	this->nDispHeight = nDispHeight;

	bRunning = false;
}

void Settings::vOnEvent(SDL_Event* ptEvent) {
	Event::vOnEvent(ptEvent);
}

void Settings::vOnLButtonUp(int nMX, int nMY) {
	if(cVecButtonList.size() != 0) {
		for(int i = 0; i < (int) cVecButtonList.size(); i++) {
			if(nMX >= (cVecButtonList[i]->nX) && nMX <= (cVecButtonList[i]->nX + cVecButtonList[i]->nWidth) 
			&& (nMY >= cVecButtonList[i]->nY) && nMY <= (cVecButtonList[i]->nY + cVecButtonList[i]->nHeight)) {
				if(!cVecButtonList[i]->bOnClick()) {
					switch (cVecButtonList[i]->nTypeID) {
					case BUTTON_TYPE_START:
						//Do Stuff
						return;
					case BUTTON_TYPE_BACK:
						return;
					default:
						return;				
					}
				}
			}
			cVecButtonList[i]->bClicked = false;
		}
	}
}

void Settings::vOnRender(SDL_Surface* psSurfDisplay) {
	Surface::bOnDraw(psSurfDisplay, psSurfBackground, nX, nY);
	int nActiveButtonFrame = 0;
	for(int i = 0; i < (int) cVecButtonList.size(); i++) { //designed for a three frame button graphic (Up / Hover / Down)
		if(cVecButtonList[i]->bClicked) 
			nActiveButtonFrame = 2;
		else if(nMX >= (cVecButtonList[i]->nX) && nMX <= (cVecButtonList[i]->nX + cVecButtonList[i]->nWidth) && (nMY >= cVecButtonList[i]->nY) && nMY <= (cVecButtonList[i]->nY + cVecButtonList[i]->nHeight))
			nActiveButtonFrame = 1;
		else nActiveButtonFrame = 0;

		Surface::bOnDraw(psSurfDisplay, cVecButtonList[i]->psSurfButton, cVecButtonList[i]->nX, cVecButtonList[i]->nY, int( float(nActiveButtonFrame/3.0) * cVecButtonList[i]->psSurfButton->w ), 0, cVecButtonList[i]->nWidth, cVecButtonList[i]->nHeight);
	}
}

void Settings::vOnActivate() {
	try {
	psSurfBackground = Surface::psOnLoad("../Debug/gfx/Menus/MenuSettingsBG.png");
	if(!psSurfBackground) throw "Failed to load MenuSettingsBG.png";
	}
	catch(char* pchException) {
		std::cerr << pchException;
		assert(!pchException);
	}
	catch(...) {
		std::cerr << "Caught an exception of unknown type in Settings::vOnInit(int nX, int nY, int nWidth, int nHeight, char* pchFile)";
		assert(0);
	}

	nWidth = psSurfBackground->w;
	nHeight = psSurfBackground->h;

	nX = nDispWidth / 2 - nWidth / 2;
	nY = nDispHeight / 2 - nHeight / 2;

	vAddButton(BUTTON_TYPE_START, nX + nWidth/2 - 80, int(nY + nHeight*(1.0/2.0) - 60 - 30), 160, 60, "../Debug/gfx/Menus/ButtonNew.png");
	vAddButton(BUTTON_TYPE_BACK, nX + nWidth/2 - 80, int(nY + nHeight*(1.0/2.0) - 30), 160, 60, "../Debug/gfx/Menus/ButtonLoad.png");

	bRunning = true;
}

void Settings::vOnDeactivate() {
	//free surfaces to increase performance
	if(psSurfBackground) SDL_FreeSurface(psSurfBackground);

	for(int i = 0; i < (int) cVecButtonList.size(); i++) delete cVecButtonList[i];
	cVecButtonList.clear();

	//delete the entire module from the App if you really want to save memory.
	bRunning = false;
}

void Settings::vOnLoop() {
}

Settings* Settings::s_pcGetInstance() {
	return s_pcMenuSettings;
}

void Settings::vOnResize(int nW, int nH) {
	nDispWidth = nW;
	nDispHeight = nH;

	nWidth = psSurfBackground->w;
	nHeight = psSurfBackground->h;

	nX = nDispWidth / 2 - nWidth / 2;
	nY = nDispHeight / 2 - nHeight / 2;

	for(int i = 0; i < (int) cVecButtonList.size(); i++) {
		cVecButtonList[i]->nX = nX + nWidth/2 - 80;
	}

	//this is probably not the most efficient or intelligent way.
	cVecButtonList[0]->nY = int(nY + nHeight*(1.0/2.0) - 60 - 30);
	cVecButtonList[1]->nY = int(nY + nHeight*(1.0/2.0) - 30);
}
