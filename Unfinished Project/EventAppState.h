#ifndef EVENTAPPSTATE_H
#define EVENTAPPSTATE_H

#include "Event.h"

//template for module interfaces with EventApp.  i.e. create a derived AppState class for each module and have the module inherit from the class.

class AppState: public Event {
public:
	AppState();

	virtual void vOnActivate() = 0;

	virtual void vOnDeactivate() = 0;

	virtual void vOnLoop() = 0;

	virtual void vOnRender(SDL_Surface* psSurfDisplay) = 0;


};


#endif