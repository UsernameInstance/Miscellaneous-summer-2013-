#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>

class Surface {
public:
	Surface();

	static SDL_Surface* psOnLoad(char* pchFile);

	static bool bOnDraw(SDL_Surface* psSurfDest, SDL_Surface* psSurfSrc, int nX, int nY);

	static bool bOnDraw(SDL_Surface* psSurfDest, SDL_Surface* psSurfSrc, int nX, int nY, int nX2, int nY2, int nW, int nH);


};


#endif