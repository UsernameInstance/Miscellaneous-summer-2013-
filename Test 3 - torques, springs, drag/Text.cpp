#include "Text.h"
#include "App.h"

Text* Text::s_pcText = NULL;
bool Text::s_bInstance = false;

Text* Text::Instance() {
	if(!s_bInstance) {
		s_pcText = new Text();
		s_bInstance = true;
	}
	return s_pcText;
}

Text::Text() {
	SDL_Surface* temp = SDL_LoadBMP("../Debug/gfx/font.bmp");
	psFont = SDL_ConvertSurface(temp, App::Instance()->psSurfDisp->format, SDL_SWSURFACE);
	SDL_FreeSurface(temp);
}

Text::~Text() {
	SDL_FreeSurface(psFont);
	psFont = NULL;

	s_pcText = NULL;
	s_bInstance = false;
}

void Text::RenderChar(SDL_Surface* psTarg, int iX, int iY, int iCh) {
	if(psTarg == NULL) return;
	if(iCh == 32) return;

	iCh -=33; //font.bmp does not have the first 33 characters.

	if(SDL_MUSTLOCK(psTarg)) if(SDL_LockSurface(psTarg) < 0) return;

	int i, j;

	for(i = 0; i < psFont->w; i++) {
		int targofs = iX + (iY + i)*psTarg->pitch/4;
		int chofs = (i + iCh*psFont->w)*(psFont->pitch/4);

		for(j = 0; j < psFont->w; j++) {
			((unsigned int*)psTarg->pixels)[targofs] = ((unsigned int*)psFont->pixels)[chofs];
			targofs++;
			chofs++;
		}
	}


	if(SDL_MUSTLOCK(psTarg)) SDL_UnlockSurface(psTarg);
}

void Text::RenderString(SDL_Surface* psTarg, int iX, int iY, const char* pchS) {
	while(*pchS != 0) {
		RenderChar(psTarg, iX, iY, *pchS);
		pchS++;
		iX += psFont->w;
	}
}
