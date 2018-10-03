#include "Collisions.h"

Collisions* Collisions::s_pcCollisions = 0;
bool Collisions::s_bInstance = false;

Collisions* Collisions::Instance() {
	if(!s_bInstance) {
		s_pcCollisions = new Collisions();
		s_bInstance = true;
	}
	return s_pcCollisions;
}

Collisions::Collisions() {}
Collisions::~Collisions() { s_bInstance = false; }

void Collisions::Loop() {}