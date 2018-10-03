#include "Entities.h"
#include "Graphics.h"

Body::Body(double dX, double dY, int iSeed) { //used make random body
	if(iSeed < 0) iSeed = 0;

	srand(iSeed);

	int n = 3 + rand() % 6; //num Verts;

	iNumVerts = n;
	
	double k = 6.28 / double(n); // quadrant size, for preventing vertex overlap
	double ang = 0; double mag0, mag1, mag2, mag3, mag4;
	double vx, vy, VL;
	cVerts.resize(6, n); cProps.resize(3,n); cColos.resize(1, n);

	for(int i = 2; i < 5; i++) {
		for(int j = 0; j < n; j++) cVerts(i, 0); //set vel + accel = 0
	}

	double IL = 20; double LM = 10; double IMa = 10; int MA = 11;
	double IV = 3; double Ela = 1000; int CoM = 6;
	for(int i = 0; i < n; i++) {
		ang = k*i + ( double(rand() % 101)/100.0 )*k;
		cVerts(0, i) = (IL + pow(-1.0, rand() % 2) * LM * double(rand() % 101)/100.0) * cos(ang) + dX; //x Val relative disp
		cVerts(1, i) = (IL + pow(-1.0, rand() % 2) * LM * double(rand() % 101)/100.0) * sin(ang) + dY; //y Val relative disp
		vx = cVerts(0,i);
		vy = cVerts(1,i);

		mag0 = sqrt(pow(cVerts(0,i), 2) + pow(cVerts(1,i), 2));
		mag1 = abs(vx * cos(i*k) + vy * sin(i*k))  * sqrt(pow(cos(i*k) - vx, 2) + pow(sin(i*k) - vy, 2));
		mag2 = abs(vx * cos((i+1)*k) + vy * sin((i+1)*k))  * sqrt(pow(cos((i+1)*k) - vx, 2) + pow(sin((i+1)*k) - vy, 2));

		cProps(0, i) = IMa + rand() % MA;

		if(i == 0) {
			VL = mag0;
			if(VL > mag1) VL = mag1;
			if(VL > mag2) VL = mag2;

		}
		else if(0 < i && i < n - 1) {
			mag3 = sqrt(pow(cVerts(i-1, 0) - vx, 2) + pow(cVerts(i-1, 1) - vy, 2));
			VL = mag0;
			if(VL > mag1) VL = mag1;
			if(VL > mag2) VL = mag2;
			if(VL > mag3) VL = mag3;
		}
		else {
			mag3 = sqrt(pow(cVerts(0, i-1) - vx, 2) + pow(cVerts(1, i-1) - vy, 2));
			mag4 = sqrt(pow(cVerts(0, 0) - vx, 2) + pow(cVerts(1, 0) - vy, 2));
			VL = mag0;
			if(VL > mag1) VL = mag1;
			if(VL > mag2) VL = mag2;
			if(VL > mag3) VL = mag3;
			if(VL > mag4) VL = mag4;
		} //NOT EFFICIENT
		
		cProps(1, i) = IV + double(rand() % 101)/100.0;
		if(cProps(1,i) > VL) cProps(1,i) = VL;

		cProps(2, i) = double(rand() % 1001) / Ela;

		cColos(i) = CoM * int ( cProps(2, i) + (cProps(1,i) + cProps(0,i)) + 0xFF0000);
	}
	
}

Body::~Body() {};

void Body::Render(SDL_Surface* psTarg) {
	for(int i = 0; i < iNumVerts; i++) {
		DrawCircle(psTarg, cProps(1, i), cVerts(0,i), cVerts(1, i), cColos(i));
	}
	for(int i = 0; i < iNumVerts-1; i++) {
		DrawLine(psTarg, cVerts(0, i), cVerts(1, i), cVerts(0, i+1), cVerts(1, i+1), (cColos(i) + cColos(i+1))/2);
	}
	DrawLine(psTarg, cVerts(0, iNumVerts-1), cVerts(1, iNumVerts-1), cVerts(0, 0), cVerts(1, 0), (cColos(iNumVerts-1) + cColos(0))/2);
}