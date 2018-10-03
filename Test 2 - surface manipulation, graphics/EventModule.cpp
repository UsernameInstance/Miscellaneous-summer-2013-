#include "EventModule.h"
#include "App.h"

Module::Module() { bActive = false; }
Module::~Module() {}

void Module::HandleEvents(SDL_Event* psEvent) {
	while(SDL_PollEvent(psEvent)) OnEvent(psEvent);
}

void Module::OnExit() { App::Instance()->SetState(App::STATE_NONE); }