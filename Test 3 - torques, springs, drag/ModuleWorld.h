#ifndef MODULEWORLD_H
#define MODULEWORLD_H

#include "EventModule.h"
#include "Entities.h"
#include <vector>

extern std::vector<Body*, Eigen::aligned_allocator<Eigen::Vector2d>> g_cBodyList;

class World: public Module {
private:
	World();
	static World* s_pcWorld;
	static bool s_bInstance;

public:
	static World* Instance();	
	~World();

	double dFDensity; //Fluid density
	double dLTime; //Time of last Loop() call, milliseconds
	double dDTime; //Time difference between current loop call and last.

	int iUCPS; //Update cycles per second. Used to prevent inconsistent behavior.
	double dTPUC; //Time per update cycle. Used in conjunction with iUCPS;

	int iXSize, iYSize; //width and height of world

	void Activate();

	void Loop();
	void Render();

	void Deactivate();
};

#endif