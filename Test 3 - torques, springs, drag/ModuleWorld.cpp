#include "App.h"
#include "Graphics.h"
#include "Collisions.h"
//look up: Global Variable & the extern keyword, STL containers with eigen objects and/or objects containing eigen objects.
std::vector<Body*, Eigen::aligned_allocator<Eigen::Vector2d>> g_cBodyList;

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
	//for(int i = 0; i < 6; i++) g_cBodyList.push_back(new Body(200*(i%3) + 100, 200 * (i/3) + 100, rand())); 
	//g_cBodyList.push_back(new Body(320, 240, SDL_GetTicks()));

	iXSize = 1280; iYSize = 720;

	dLTime = 0;
	dDTime = 0;
	iUCPS = 100;
	dTPUC = 1.0/double(iUCPS);
	dFDensity = 0.001025;

	g_cBodyList.push_back(new Body(270, 600, true));
	g_cBodyList.push_back(new Body(370, 900, true));
	g_cBodyList.push_back(new Body(320, 0, true));
	g_cBodyList.push_back(new Body(320,300));
	

	g_cBodyList.push_back(new Body(1010, 600, true));
	g_cBodyList.push_back(new Body(910, 900, true));
	g_cBodyList.push_back(new Body(960, 0, true));
	g_cBodyList.push_back(new Body(960,300));

	g_cBodyList.push_back(new Body(640, 100, true));
	g_cBodyList.push_back(new Body(640,600));
	
	
}

World::~World() { for(int i = 0; i < int(g_cBodyList.size()); i++) delete g_cBodyList[i]; }

void World::Activate() { bActive = true; dLTime = SDL_GetTicks(); }

void World::Loop() {
	dDTime = 1e-3 * double( SDL_GetTicks() - dLTime ); //Divided by 1000 to convert to seconds.

	if(dDTime < dTPUC) return;

	dLTime = SDL_GetTicks();

	//Handle Collisions:
	Collisions::Instance()->Loop();
	
	for(int i = 0; i < int(g_cBodyList.size()); i++) {
		g_cBodyList[i]->Loop();

		//Vertices: 
		//Update: Accel -> Vel -> Pos
		g_cBodyList[i]->cVerts.row(4) = ( (g_cBodyList[i]->cVerts.row(6)).cwiseQuotient(g_cBodyList[i]->cProps.row(0)) );
		g_cBodyList[i]->cVerts.row(5) = ( (g_cBodyList[i]->cVerts.row(7)).cwiseQuotient(g_cBodyList[i]->cProps.row(0)) );

		g_cBodyList[i]->cVerts.row(2) += dTPUC * g_cBodyList[i]->cVerts.row(4);
		g_cBodyList[i]->cVerts.row(3) += dTPUC * g_cBodyList[i]->cVerts.row(5);

		g_cBodyList[i]->cVerts.row(0) +=  dTPUC * g_cBodyList[i]->cVerts.row(2);
		g_cBodyList[i]->cVerts.row(1) +=  dTPUC * g_cBodyList[i]->cVerts.row(3);
		
		//Clear: Forces & Accel
		g_cBodyList[i]->cVerts.row(4) = g_cBodyList[i]->cVerts.row(5) = g_cBodyList[i]->cVerts.row(6) = g_cBodyList[i]->cVerts.row(7).setZero();

		//Handle any necessary shifts.
		if(g_cBodyList[i]->iMinX > iXSize) g_cBodyList[i]->cVerts.row(0) = g_cBodyList[i]->cVerts.row(0).array() - iXSize;
		if(g_cBodyList[i]->iMinY > iYSize) g_cBodyList[i]->cVerts.row(1) = g_cBodyList[i]->cVerts.row(1).array() - iYSize;
		if(g_cBodyList[i]->iMaxX < 0) g_cBodyList[i]->cVerts.row(0) = g_cBodyList[i]->cVerts.row(0).array() + iXSize;
		if(g_cBodyList[i]->iMaxY < 0) g_cBodyList[i]->cVerts.row(1) = g_cBodyList[i]->cVerts.row(1).array() + iYSize;

		//Update extremums
		g_cBodyList[i]->iMinX = (int)g_cBodyList[i]->cVerts.row(0).minCoeff(); g_cBodyList[i]->iMaxX = (int)g_cBodyList[i]->cVerts.row(0).maxCoeff();
		g_cBodyList[i]->iMinY = (int)g_cBodyList[i]->cVerts.row(1).minCoeff(); g_cBodyList[i]->iMaxY = (int)g_cBodyList[i]->cVerts.row(1).maxCoeff();
	}
	
	//update brain
	
}

void World::Render() {
	SDL_Surface* psScreen = App::Instance()->psSurfDisp;
	FillSurf(psScreen, 0xFFFFFF);
	for(int i = 0; i < int(g_cBodyList.size()); i++) g_cBodyList[i]->Render(psScreen);
}

void World::Deactivate() { bActive = false; dLTime = 0;}

