#ifndef EVENTMODULE_H
#define EVENTMODULE_H

#include "Event.h"

class Module: public Event { //app module/state etc...
public:
	Module();
	virtual ~Module();

	bool bActive;

	virtual void Activate() = 0;

	virtual void HandleEvents(SDL_Event* psEvent);
	virtual void Loop() = 0;
	virtual void Render() = 0;

	virtual void Deactivate() = 0;

	virtual void OnExit();
};


#endif