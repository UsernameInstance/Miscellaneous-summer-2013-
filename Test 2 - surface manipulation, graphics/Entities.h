#ifndef ENTITIES_H
#define ENTITIES_H
#include "SDL.h"
#include <Eigen>
#include <StdVector>
//drag force = (1/2)*fluid densiy*(velocity*velocity)*(drag coefficient)*(reference area)
//drag coefficient is 0.47 for sphere, 0.82 for long cylinder, 1.15 for short cylinder, 1.05 for cube, 0.5 for cone, 0.42 for half sphere, 0.8 for angled cube
class Vertex {
public:
	Vertex();
	~Vertex();

	//use surface for performance
	double dX, dY, dVX, dVY, dAX, dAY; //pos, vel, accel
	double dM, dR, dA, dK; //mass, radius, area, spring constant


};

class Body {
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Body(double dX, double dY, int iSeed);
	//Body(Eigen::Vector2d cRO, Eigen::Matrix<double, Eigen::Dynamic, 2> cVerts, Eigen::Matrix<double, Eigen::Dynamic, 2> cProps);
	~Body();

	int iNumVerts;

	Eigen::Vector2d cCM; //Center of mass relative to display surface.

	Eigen::Matrix<double, 6, Eigen::Dynamic> cVerts; //cVerts are relative to display origin (i.e. top left of window) ~ relative Origin, row0 ~ x, row1 ~ y, row2 ~ velocity x, row3 ~ velocity y, row4 ~ accel x, row5 ~ accel y;
	Eigen::Matrix<double, 3, Eigen::Dynamic> cProps; //Non-motion base properties of vertices. row1 ~ Mass, row2 ~ Volume/area, row3 ~ Elasticity, aka spring constant, avged for edges;
	Eigen::Matrix<int, 1, Eigen::Dynamic> cColos; //colors of verticies and edges. first set of cols = verts, 2nd set edges

	void Init(int iSeed);
	void Render(SDL_Surface* psTarg);
};

class Brain { //neural network or genetic algorithm??
public:
	Brain(int iNeurons);
	~Brain();
};

class Agent {
public:
	Agent(double dX, double dY);
	~Agent();

	Body cBody;
	Brain cBrain;

	void Loop();
};


#endif