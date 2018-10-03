#include "AppStateMenu.h"

Menu::Menu(int nX, int nY, int nWidth, int nHeight) {
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;
	bRunning = false;
	
	psSurfBackground = NULL;
};

Menu::~Menu() {
	if(psSurfBackground) SDL_FreeSurface(psSurfBackground);

	for(int i = 0; i < (int) cVecButtonList.size(); i++) delete cVecButtonList[i];

	cVecButtonList.clear();
}

void Menu::vAddButton(int nTypeID, int nX, int nY, int nWidth, int nHeight, char* pchFile) {
	Button* pcButtonTemp = new Button(nTypeID, nX, nY, nWidth, nHeight);
	pcButtonTemp->psSurfButton = Surface::psOnLoad(pchFile);

	cVecButtonList.push_back(pcButtonTemp);
}

void Menu::vOnInit(int nDispWidth, int nDispHeight) {
	//Set Menu Graphics
	//Add Buttons
}

/*
void Menu::vOnInit(int nX, int nY, int nWidth, int nHeight, char* pchFile) { //pure virtual
	//Set Menu Background Image
	//Add Buttons
}
*/

void Menu::vOnEvent(SDL_Event* ptEvent) { //pure virtual
}

void Menu::vOnLButtonDown(int nMX, int nMY) {
	if(cVecButtonList.size() != 0) {
		for(int i = 0; i < (int) cVecButtonList.size(); i++) {
			if(nMX >= (cVecButtonList[i]->nX) && nMX <= (cVecButtonList[i]->nX + cVecButtonList[i]->nWidth) 
			&& (nMY >= cVecButtonList[i]->nY) && nMY <= (cVecButtonList[i]->nY + cVecButtonList[i]->nHeight)) if(!cVecButtonList[i]->bOnClick()) break;
		}
	}
}


void Menu::vOnLButtonUp(int nMX, int nMY) {
	if(cVecButtonList.size() != 0) {
		for(int i = 0; i < (int) cVecButtonList.size(); i++) {
			if(nMX >= (cVecButtonList[i]->nX) && nMX <= (cVecButtonList[i]->nX + cVecButtonList[i]->nWidth) 
			&& (nMY >= cVecButtonList[i]->nY) && nMY <= (cVecButtonList[i]->nY + cVecButtonList[i]->nHeight)) {
				if(!cVecButtonList[i]->bOnClick()) {
					switch (cVecButtonList[i]->nTypeID) {
						case 0 /*Your button type here*/:
							return;
						 //button function goes here
						default:
							return;
					}
					break;
				}
			}
			cVecButtonList[i]->bClicked = false;
		}
	}
}


void Menu::vOnRender(SDL_Surface* psSurfDisplay) {
	Surface::bOnDraw(psSurfDisplay, psSurfBackground, nX, nY);
	for(int i = 0; i < (int) cVecButtonList.size(); i++) { //designed for a two frame button graphic (Up / Down)
		if(cVecButtonList[i]->bClicked) Surface::bOnDraw(psSurfDisplay, cVecButtonList[i]->psSurfButton, cVecButtonList[i]->nX, cVecButtonList[i]->nY, cVecButtonList[i]->psSurfButton->w, 0, cVecButtonList[i]->nWidth, cVecButtonList[i]->nHeight);
		else Surface::bOnDraw(psSurfDisplay, cVecButtonList[i]->psSurfButton, cVecButtonList[i]->nX, cVecButtonList[i]->nY, 0, 0, cVecButtonList[i]->nWidth, cVecButtonList[i]->nHeight);
	}
}

void Menu::vOnMouseMove(int nMX, int nMY, int nRelX, int nRelY, bool bLeft, bool bRight, bool bMiddle) {
	this->nMX = nMX;
	this->nMY = nMY;
}


