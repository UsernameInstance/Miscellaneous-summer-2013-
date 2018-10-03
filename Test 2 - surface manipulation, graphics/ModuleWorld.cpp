#include "ModuleWorld.h"
#include "App.h"
#include "Graphics.h"

World* World::s_pcWorld = NULL;
bool World::s_bInstance = false;

World* World::Instance() {
	if(!s_bInstance) {
		s_pcWorld = new World();
		s_bInstance = true;
	}
	return s_pcWorld;
}

World::World() { 
	srand(SDL_GetTicks());
	//for(int i = 0; i < 6; i++) s_cBodyList.push_back(new Body(200*(i%3) + 100, 200 * (i/3) + 100, rand())); 
	s_cBodyList.push_back(new Body(320, 240, SDL_GetTicks()));
}

World::~World() { for(int i = 0; i < int(s_cBodyList.size()); i++) delete s_cBodyList[i]; }

void World::Activate() { bActive = true; }

void World::Loop() {
//for(int i = 0; i < int(s_cBodyList.size()); i++) 
	//for(int j = 0; j < s_cBodyList[i]->iNumVerts; j++) s_cBodyList[i]->cVerts(j, 0) += -sin(double(SDL_GetTicks()/1000))/(0.9 + rand()%101), s_cBodyList[i]->cVerts(j, 1) += cos(double(SDL_GetTicks()/1000))/(0.9 + rand()%101);

}

void World::Render() {
	SDL_Surface* psScreen = App::Instance()->psSurfDisp;
	FillSurf(psScreen, 0xFFFFFF);
	for(int i = 0; i < int(s_cBodyList.size()); i++) s_cBodyList[i]->Render(psScreen);
}

void World::Deactivate() { bActive = false; }

