#ifndef MODULEWORLD_H
#define MODULEWORLD_H

#include "EventModule.h"

#include "Entities.h"
#include <vector>


static std::vector<Body*, Eigen::aligned_allocator<Eigen::Vector2d>> s_cBodyList;

class World: public Module {
private:
	World();
	static World* s_pcWorld;
	static bool s_bInstance;

public:
	static World* Instance();	
	~World();

	void Activate();

	void Loop();
	void Render();

	void Deactivate();
};

#endif