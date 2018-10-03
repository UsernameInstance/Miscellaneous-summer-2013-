#include "Surface.h"

Surface::Surface() {
}

SDL_Surface* Surface::psOnLoad(char* pchFile) {
	SDL_Surface* psSurfTemp = NULL;
	SDL_Surface* psSurfReturn = NULL;

	psSurfTemp = IMG_Load(pchFile);
	if(psSurfTemp == NULL) return NULL;

	psSurfReturn = SDL_DisplayFormatAlpha(psSurfTemp);

	SDL_FreeSurface(psSurfTemp);

	return psSurfReturn;
}

bool Surface::bOnDraw(SDL_Surface* psSurfDest, SDL_Surface* psSurfSrc, int nX, int nY) {
	if(psSurfDest == NULL || psSurfSrc == NULL) return false;

	SDL_Rect sRectDest;

	sRectDest.x = nX;
	sRectDest.y = nY;

	SDL_BlitSurface(psSurfSrc, NULL, psSurfDest, &sRectDest);

	return true;
}

bool Surface::bOnDraw(SDL_Surface* psSurfDest, SDL_Surface* psSurfSrc, int nX, int nY, int nX2, int nY2, int nW, int nH) {
	if(psSurfDest == NULL || psSurfSrc == NULL) return false;

	SDL_Rect sRectDest;

	sRectDest.x = nX;
	sRectDest.y = nY;

	SDL_Rect sRectSrc;

	sRectSrc.x = nX2;
	sRectSrc.y = nY2;
	sRectSrc.w = nW;
	sRectSrc.h = nH;

	SDL_BlitSurface(psSurfSrc, &sRectSrc, psSurfDest, &sRectDest);

	return true;
}