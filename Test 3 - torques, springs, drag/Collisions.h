#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "ModuleWorld.h"
#include "Graphics.h"
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

	int iXSize, iYSize;
	double dLTime; //Time of last Loop() call, milliseconds
	double dDTime; //Time difference between current loop call and last.
	double dTPUC; //Time per update cycle. Used in conjunction with iUCPS. Used here as fixed time interval for calculations;

	double dRBSC; //Rigid Body Spring Constant
	double dRBDC; //Rigid Body Damping Constant
	double dSRBSC; //square root of dRBSC;
	double dSRT; //ratio dSRBSC/dDtime

	bool bM1X, bM2X, bP1X, bP2X, bM1Y, bM2Y, bP1Y, bP2Y; //Used to check if shifted collision checks are needed.
	double dS1X, dS2X, dS1Y, dS2Y; //used in shifted collision checks.
	bool bING; //InNgon
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Eigen::Vector2d cCollCen; //vector between vertex centers in collision.
	Eigen::Vector2d cCollOve; //overlap vector in collision handling
	Eigen::Vector2d cCollShi; //Shift vector in collision handling.
	Eigen::Vector2d cDifVe; //measures difference between vertices. used in spring displ calcs.
	Eigen::Vector2d cDisVe; //measures spring displacement between connected vertices of entity;
	Eigen::Vector2d cEqVe; //Equilibrium vertex vector used in spring displ calcs.
	
	Eigen::Vector2d cRelV; //Relative velocities
	Eigen::Vector2d cP; //Point vec used for containment checks.
	void Loop();

	//experimenting with drag force
	Eigen::Vector3d cZ, cN, cA, cB, cC, cVA, cVB, cVC, cF, cFB, cFA, cT;
	double dK, dKA, dKB, dMA, dMB, dMC, dU0, dU1;

};

#endif

/*
Static variables are local to the compilation unit. A compilation unit is basically a .cpp file with the contents of the .h file inserted in place of each #include directive.

Now, in a compilation unit you can't have two global variables with the same name. This is what's happening in your case: main.cpp includes file1.h and file.h, and each of the two headers defines its own Var1.

In other words if I have two cpp files. file1.cpp and file2.cpp, and I havea  a header file. file3.h. And I declare a static variable in file3.h, say:
static int s_iI;

Then if I include file3.h in each of the cpp files. Then each cpp file will get their own unique static s_iI variable (same name, different variable per file).
*/