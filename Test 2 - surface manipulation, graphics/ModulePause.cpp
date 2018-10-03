#include "ModulePause.h"

Pause* Pause::s_pcPause = NULL;
bool Pause::s_bInstance = false;

Pause* Pause::Instance() {
	if(!s_bInstance) {
		s_pcPause = new Pause();
		s_bInstance = true;
	}
	return s_pcPause;
}

Pause::Pause() {}
Pause::~Pause() {}

void Pause::Activate() { bActive = true; }

void Pause::Loop() {}
void Pause::Render() {}

void Pause::Deactivate() { bActive = false; }