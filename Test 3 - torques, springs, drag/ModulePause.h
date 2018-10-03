#ifndef MODULEPAUSE_H
#define MODULEPAUSE_H

#include "EventModule.h"

class Pause: public Module {
private:
	Pause();
	static Pause* s_pcPause;
	static bool s_bInstance;

public:
	static Pause* Instance();	
	~Pause();

	void Activate();

	void Loop();
	void Render();

	void Deactivate();
};

#endif