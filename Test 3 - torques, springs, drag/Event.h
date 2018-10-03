#ifndef EVENT_H
#define EVENT_H

#include "SDL.h"

class Event {
public:
	Event();
	virtual ~Event();

	virtual void OnEvent(SDL_Event* psEvent);
	
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	virtual void OnExit();

};

#endif