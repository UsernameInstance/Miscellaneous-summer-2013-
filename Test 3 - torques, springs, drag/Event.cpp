#include "Event.h"

Event::Event() {};
Event::~Event() {};

void Event::OnEvent(SDL_Event* psEvent) {
	switch(psEvent->type) {
		case SDL_KEYDOWN:
			OnKeyDown(psEvent->key.keysym.sym, psEvent->key.keysym.mod, psEvent->key.keysym.unicode);
			break;
		case SDL_KEYUP:
			OnKeyUp(psEvent->key.keysym.sym, psEvent->key.keysym.mod, psEvent->key.keysym.unicode);
			break;
		case SDL_QUIT:
			OnExit();
			break;
	}
}

void Event::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {};

void Event::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {};

void Event::OnExit() {};