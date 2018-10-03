#include "Button.h"

Button::Button(int nTypeID, int nX, int nY, int nWidth, int nHeight) {
	this->nTypeID = nTypeID;
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;

	psSurfButton = NULL;

	bClicked = false;
}

Button::Button(int nTypeID, int nX, int nY, int nWidth, int nHeight, char* pchFile) {
	this->nTypeID = nTypeID;
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;

	psSurfButton = Surface::psOnLoad(pchFile);

	bClicked = false;
}

Button::~Button() {
	if(psSurfButton) SDL_FreeSurface(psSurfButton);
}

bool Button::bOnClick() {
	if(!bClicked) bClicked = true;
	else if(bClicked) bClicked = false;
	
	return bClicked;
}