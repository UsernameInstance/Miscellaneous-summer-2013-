#include "FPS.h"

bool FPS::s_bInstance = false;
FPS* FPS::s_pcFPS = NULL;

void FPS::vMakeInstance() {
	if(!s_bInstance) {
		s_pcFPS = new FPS();
		s_bInstance = true;
	}
}

FPS::FPS() {
	nFrameTime = 0;
	nCycleTime = 0;

	fHertz = 0;

	nFPS = 0;
	nFrameCounter = 0;
}

void FPS::vOnLoop() {
	if(nFrameTime + 1000 < (int)SDL_GetTicks()) {
		nFrameTime = SDL_GetTicks();

		nFPS = nFrameCounter;

		nFrameCounter = 0;
	}

	fHertz = float(1000.0)/float(SDL_GetTicks() - nCycleTime); //cycles per second

	nCycleTime = SDL_GetTicks();

	nFrameCounter++;
}

int FPS::nGetFPS() { return nFPS; }

float FPS::fGetHertz() { return fHertz; }