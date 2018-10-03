#ifndef ENTITIES_H
#define ENTITIES_H
#include "SDL.h"
#include <Eigen>

#include "Organ.h"
//drag force = (1/2)*fluid densiy*(velocity*velocity)*(drag coefficient)*(reference area)
//drag coefficient is 0.47 for sphere, 0.82 for long cylinder, 1.15 for short cylinder, 1.05 for cube, 0.5 for cone, 0.42 for half sphere, 0.8 for angled cube
class Body {
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Body(double dX, double dY, int iSeed);
	Body(double dX, double dY, bool bBall = false); //testing purposes 
	//Body(Eigen::Vector2d cRO, Eigen::Matrix<double, Eigen::Dynamic, 2> cVerts, Eigen::Matrix<double, Eigen::Dynamic, 2> cProps);
	~Body();

	int iC; //interior color
	int iNumVerts, iMinX, iMaxX, iMinY, iMaxY, iMinR, iMaxR; //min and max coordinate components of vertex set. Minimum & Maximum Vertex Radius;
	bool bRendMX, bRendPX, bRendMY, bRendPY; //for rendering. rend minusx, plusx, minusy, plusy. for use if ent is at world seam.
	double dDen; //Internal Fluid Density
	//Eigen::Vector2d cP1, cP2; //used for endpoints for line segments drawn in render.

	Eigen::Vector2d cCM; //Center of mass relative to display surface.
	Eigen::Matrix<double, 2, Eigen::Dynamic> cRVert; //Used to hold shifted vertices for rendering purposes.
	Eigen::Matrix<double, 8, Eigen::Dynamic> cVerts; //cVerts are relative to display origin (i.e. top left of window) ~ relative Origin, row0 ~ x, row1 ~ y, row2 ~ velocity x, row3 ~ velocity y, row4 ~ accel x, row5 ~ accel y, row6 ~ force x, row7 ~ force y;
	Eigen::Matrix<double, 5, Eigen::Dynamic> cProps; //Non-motion base properties of vertices. row0 ~ Mass, row1 ~ Radius, row2 ~ spring constant, row 3 ~ spring equilibrium length, row 4 ~ Damping Coefficient;
	Eigen::Matrix<int, 2, Eigen::Dynamic> cColos; //colors of verticies and edges. row0 verts, row1, edges, colj vertj and edge between vertj and vertj+1 mod(iNumVerts), i.e. last one is between vertiNumVerts-1 and vert0;
	Eigen::Matrix<double, 2, 4> cEdge; //used in rendering.

	std::vector<Receptor*> cVReceptors;
	std::vector<Receptor*> cEReceptors;

	void Init(int iSeed);
	void Render(SDL_Surface* psTarg);
	void Loop(); //update shift booleans. To decouple from physics loop.
	void UpdateReceptors();
};


class Agent {
public:
	Agent(double dX, double dY);
	~Agent();

	void Loop();
};

#endif