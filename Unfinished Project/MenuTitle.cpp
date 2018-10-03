#include "MenuTitle.h"
#include <iostream>
#include <assert.h>
#include "EventApp.h"

bool Title::s_bInstance = false;
Title* Title::s_pcMenuTitle = NULL;

void Title::vMakeInstance() {
	if(!s_bInstance) {
		s_pcMenuTitle = new Title();
		s_bInstance = true;
	}
}

Title::Title() {
	//OnInit Handles this
}

Title::~Title() {
	//parent class free's surfaces and deletes buttons
	s_bInstance = false;
	bRunning = false;
}

/*
void Title::vOnInit(int nX, int nY, int nWidth, int nHeight, char* pchFile) {
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;

	try {
	psSurfBackground = NULL;
	psSurfBackground = Surface::psOnLoad("../Debug/gfx/Menus/MenuTitleBG.png");
	if(!psSurfBackground) throw "Failed to load MenuTitleBG.png";
	}
	catch(char* pchException) {
		std::cerr << pchException;
		assert(!pchException);
	}
	catch(...) {
		std::cerr << "Caught an exception of unknown type in Title::vOnInit(int nX, int nY, int nWidth, int nHeight, char* pchFile)";
		assert(0);
	}


	//add buttons

	bRunning = false;
}
*/

void Title::vOnInit(int nDispWidth, int nDispHeight) {
	//Image and button loading done in OnActivate() for performance
	this->nDispWidth = nDispWidth;
	this->nDispHeight = nDispHeight;

	bRunning = false;
}

void Title::vOnEvent(SDL_Event* ptEvent) {
	Event::vOnEvent(ptEvent);
}

void Title::vOnLButtonUp(int nMX, int nMY) {
	if(cVecButtonList.size() != 0) {
		for(int i = 0; i < (int) cVecButtonList.size(); i++) {
			if(nMX >= (cVecButtonList[i]->nX) && nMX <= (cVecButtonList[i]->nX + cVecButtonList[i]->nWidth) 
			&& (nMY >= cVecButtonList[i]->nY) && nMY <= (cVecButtonList[i]->nY + cVecButtonList[i]->nHeight)) {
				if(!cVecButtonList[i]->bOnClick()) {
					switch (cVecButtonList[i]->nTypeID) {
					case BUTTON_TYPE_NEW:
						//Do Stuff
						return;
					case BUTTON_TYPE_LOAD:
						return;
					case BUTTON_TYPE_OPTIONS:
						return;
					case BUTTON_TYPE_EXIT:
						App::s_pcApp->vOnExit();
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

void Title::vOnRender(SDL_Surface* psSurfDisplay) {
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

void Title::vOnActivate() {
	try {
	psSurfBackground = Surface::psOnLoad("../Debug/gfx/Menus/MenuTitleBG.png");
	if(!psSurfBackground) throw "Failed to load MenuTitleBG.png";
	}
	catch(char* pchException) {
		std::cerr << pchException;
		assert(!pchException);
	}
	catch(...) {
		std::cerr << "Caught an exception of unknown type in Title::vOnInit(int nX, int nY, int nWidth, int nHeight, char* pchFile)";
		assert(0);
	}

	nWidth = psSurfBackground->w;
	nHeight = psSurfBackground->h;

	nX = nDispWidth / 2 - nWidth / 2;
	nY = nDispHeight / 2 - nHeight / 2;

	vAddButton(BUTTON_TYPE_NEW, nX + nWidth/2 - 80, int(nY + nHeight*(1.0/2.0) - 60 - 30), 160, 60, "../Debug/gfx/Menus/ButtonNew.png");
	vAddButton(BUTTON_TYPE_LOAD, nX + nWidth/2 - 80, int(nY + nHeight*(1.0/2.0) - 30), 160, 60, "../Debug/gfx/Menus/ButtonLoad.png");
	vAddButton(BUTTON_TYPE_EXIT, nX + nWidth/2 - 80, int(nY + nHeight*(1.0/2.0) + 60 - 30), 160, 60, "../Debug/gfx/Menus/ButtonExit.png");
	
	bRunning = true;
}

void Title::vOnDeactivate() {
	//free surfaces to increase performance
	if(psSurfBackground) SDL_FreeSurface(psSurfBackground);

	for(int i = 0; i < (int) cVecButtonList.size(); i++) delete cVecButtonList[i];
	cVecButtonList.clear();

	//delete the entire module from the App if you really want to save memory.
	bRunning = false;
}

void Title::vOnLoop() {
}

Title* Title::s_pcGetInstance() {
	return s_pcMenuTitle;
}

void Title::vOnResize(int nW, int nH) {
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
	cVecButtonList[2]->nY = int(nY + nHeight*(1.0/2.0) + 60 - 30);
}
