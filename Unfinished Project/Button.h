#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include "Surface.h"

//Basically a simple container only used by the Menu class.  Might make it a nested class.

class Button {
public:
	Button(int nTypeID, int nX, int nY, int nWidth, int nHeight);

	Button(int nTypeID, int nX, int nY, int nWidth, int nHeight, char* pchFile);

	~Button();

	int nX, nY; //Coordinates relative to containing menu.

	int nTypeID; //Defined by containing menu.

	int nWidth, nHeight;

	bool bClicked; //clicked -> graphic down/pressed

	SDL_Surface* psSurfButton;

	bool bOnClick(); //toggles bClicked.  Then returns new bClicked value.

};

#endif