#ifndef COLLISIONS_H
#define COLLISIONS_H

//singleton
//detects collisions and alters parameters of entities in accordance

class Collisions { 
private:
	Collisions();

	static Collisions* s_pcCollisions;
	static bool s_bInstance;

public:
	static Collisions* Instance();
	~Collisions();

	void Loop();

};

#endif