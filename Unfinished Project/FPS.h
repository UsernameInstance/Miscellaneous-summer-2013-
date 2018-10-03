#ifndef FPS_H
#define FPS_H

#include <SDL.h>

class FPS {
private:
	static bool s_bInstance;
	FPS();

	int nFrameTime, nCycleTime; //milliseconds

	float fHertz;

	int nFPS, nFrameCounter;

public:
	static FPS* s_pcFPS;
	static void vMakeInstance();
	~FPS();

	void vOnLoop();

	int nGetFPS();

	float fGetHertz();

};

#endif