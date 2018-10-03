#include "Entities.h"
#include "Graphics.h"
#include "ModuleWorld.h"

Body::Body(double dX, double dY, bool bBall) {
	bRendMX = bRendPX = bRendMY = bRendPY = false;
	dDen = 0.0005;

	cEdge.setZero();

	if(!bBall) {
		iNumVerts = 3;

		cVerts.resize(8,iNumVerts); cProps.resize(5,iNumVerts); cColos.resize(2,iNumVerts);
		cRVert.resize(2,iNumVerts);

		double dR = 100.0;
		cVerts(0, 0) = dX; cVerts(1,0) = dY - sqrt((3./4.)*dR*dR);
		cVerts(0,1) = dX - (0.5)*dR; cVerts(1,1) = dY;
		cVerts(0,2) = dX + (0.5)*dR; cVerts(1,2) = dY;
		cRVert.row(0) = cVerts.row(0); cRVert.row(1) = cVerts.row(1);

		cProps.row(3).setConstant(dR);

		cVerts.row(2) = cVerts.row(3) = cVerts.row(4) = cVerts.row(5) = cVerts.row(6) = cVerts.row(7).setZero();
		
		for(int i = 0; i < iNumVerts; i++) { 
			cProps(0, i) = 25.0; cProps(1,i) = 10; cProps(2,i) = 5e2; cProps(4,i) = 5e-8;
			cColos(0,i) = 0xff; cColos(1,i) = 0x00FF00;
		}
	}
	else {
		iNumVerts = 1;
		cVerts.resize(8,iNumVerts); cProps.resize(5,iNumVerts); cColos.resize(2,iNumVerts);
		cRVert.resize(2,iNumVerts);

		cVerts(0, 0) = dX; cVerts(1,0) = dY;
		cRVert.row(0) = cVerts.row(0); cRVert.row(1) = cVerts.row(1);

		cProps.row(3).setConstant(0);

		cVerts.row(2) = cVerts.row(3) = cVerts.row(4) = cVerts.row(5) = cVerts.row(6) = cVerts.row(7).setZero();
		cVerts(3,0) = -400;
		
		for(int i = 0; i < iNumVerts; i++) { 
			cProps(0, i) = 37.5; cProps(1,i) = 15; cProps(2,i) = 5e2; cProps(4,i) = 0;
			cColos(0,i) = 0x0000FF; cColos(1,i) = 0x00FF00;
		}
	}

	std::vector<double> cTemp;
	for(int i = 0; i < iNumVerts; i++) {
		cTemp.resize(8);
		cTemp[0] = 0; cTemp[1] = 1e3; cTemp[2] = cProps(0,i); cTemp[3] = cProps(1,i);
		cTemp[4] = 500; cTemp[5] = cTemp[6] = cTemp[7] = 1;

		cVReceptors.push_back(new Receptor(cTemp));		
	}


	iC = 0xff0000;

	iMinX = (int)cVerts.row(0).minCoeff(); iMaxX = (int)cVerts.row(0).maxCoeff();
	iMinY = (int)cVerts.row(1).minCoeff(); iMaxY = (int)cVerts.row(0).maxCoeff();
	
	iMinR = (int)cProps.row(1).minCoeff(); iMaxR = (int)cProps.row(1).maxCoeff();
};

Body::Body(double dX, double dY, int iSeed) { //used make random body
	dDen = 0.0005;
	if(iSeed < 0) iSeed = 0;

	//srand(iSeed);

	int n = 3 + rand() % 6; //num Verts;

	iNumVerts = n;
	
	double k = 6.28 / double(n); // quadrant size, for preventing vertex overlap
	double ang = 0; double mag0, mag1, mag2, mag3, mag4;
	double vx, vy, VL;
	cVerts.resize(8, n); cProps.resize(5,n); cColos.resize(2, n);
	cRVert.resize(2, n);
	cVerts.row(6) = (cVerts.row(7)).setZero();
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

		cColos(0,i) = CoM * int ( cProps(2, i) + (cProps(1,i) + cProps(0,i)) + 0xFF0000);
		cColos(1,i) = CoM * int ( cProps(2, i) + (cProps(1,i) + cProps(0,i)) + 0x0AFFA0);
	}
	cProps.row(4).setConstant(1e1);
}

Body::~Body() {};

void Body::Loop() {
	bRendPX = bRendPY = bRendMX = bRendMY = false;

	//if at world edge, render both sides.
	if(iMinX - iMaxR <= 0) bRendPX = true;
	if(iMinY - iMaxR <= 0) bRendPY = true;
	if(iMaxX + iMaxR >= World::Instance()->iXSize) bRendMX = true;
	if(iMaxY + iMaxR >= World::Instance()->iYSize) bRendMY = true;
}

void Body::Render(SDL_Surface* psTarg) {
	//Should seam checks be with render checks or collision checks?? Both seems wasteful.

	bRendPX = bRendPY = bRendMX = bRendMY = false;

	//if at world edge, render both sides.
	if(iMinX - iMaxR <= 0) bRendPX = true;
	if(iMinY - iMaxR <= 0) bRendPY = true;
	if(iMaxX + iMaxR >= World::Instance()->iXSize) bRendMX = true;
	if(iMaxY + iMaxR >= World::Instance()->iYSize) bRendMY = true;
	
	/*
	if(bRendPX) DrawCircle(psTarg, 50, 600, 500, 0xFF0000); //for bug finding purposes. i.e. to see if booleans are updating correctly
	if(bRendMX) DrawCircle(psTarg, 50, 400, 500, 0x00FF00);
	if(bRendPY) DrawCircle(psTarg, 50, 500, 400, 0xFF0000);
	if(bRendMY) DrawCircle(psTarg, 50, 500, 600, 0x00FF00);
	*/
	
	DrawNgon(psTarg, cVerts.row(0), cVerts.row(1), iC, 1);
	if(bRendPX) DrawNgon(psTarg, (cRVert.row(0) = cVerts.row(0).array() + World::Instance()->iXSize), cVerts.row(1), iC, 1);
	if(bRendMX) DrawNgon(psTarg, (cRVert.row(0) = cVerts.row(0).array() - World::Instance()->iXSize), cVerts.row(1), iC, 1);

	if(bRendPY) DrawNgon(psTarg, cVerts.row(0), (cRVert.row(1) = cVerts.row(1).array() + World::Instance()->iYSize), iC, 1);
	if(bRendMY) DrawNgon(psTarg, cVerts.row(0), (cRVert.row(1) = cVerts.row(1).array() - World::Instance()->iYSize), iC, 1);

	if(bRendPX && bRendPY) DrawNgon(psTarg, cRVert.row(0), cRVert.row(1), iC, 1);
	if(bRendPX && bRendMY) DrawNgon(psTarg, cRVert.row(0), cRVert.row(1), iC, 1);
	if(bRendMX && bRendPY) DrawNgon(psTarg, cRVert.row(0), cRVert.row(1), iC, 1);
	if(bRendMX && bRendMY) DrawNgon(psTarg, cRVert.row(0), cRVert.row(1), iC, 1);
	
	for(int i = 0; i < iNumVerts; i++) {
		DrawCircle(psTarg, cProps(1, i), cVerts(0,i), cVerts(1, i), cColos(0,i));

		if(bRendPX) DrawCircle(psTarg, cProps(1, i), cVerts(0,i) + World::Instance()->iXSize, cVerts(1, i), cColos(0,i));
		if(bRendMX) DrawCircle(psTarg, cProps(1, i), cVerts(0,i) - World::Instance()->iXSize, cVerts(1, i), cColos(0,i));

		if(bRendPY) DrawCircle(psTarg, cProps(1, i), cVerts(0,i), cVerts(1, i) + World::Instance()->iYSize, cColos(0,i));
		if(bRendMY) DrawCircle(psTarg, cProps(1, i), cVerts(0,i), cVerts(1, i) - World::Instance()->iYSize, cColos(0,i));

		if(bRendPX && bRendPY) DrawCircle(psTarg, cProps(1, i), cVerts(0,i) + World::Instance()->iXSize, cVerts(1, i) + World::Instance()->iYSize, cColos(0,i));
		if(bRendPX && bRendMY) DrawCircle(psTarg, cProps(1, i), cVerts(0,i) + World::Instance()->iXSize, cVerts(1, i) - World::Instance()->iYSize, cColos(0,i));
		if(bRendMX && bRendPY) DrawCircle(psTarg, cProps(1, i), cVerts(0,i) - World::Instance()->iXSize, cVerts(1, i) + World::Instance()->iYSize, cColos(0,i));
		if(bRendMX && bRendMY) DrawCircle(psTarg, cProps(1, i), cVerts(0,i) - World::Instance()->iXSize, cVerts(1, i) - World::Instance()->iYSize, cColos(0,i));
	}

		double dWid = 2;
		Eigen::Vector3d cN, cDif, cZ;
		
	for(int i = 0; i < iNumVerts-1; i++) {
		/*
		cZ << 0,0,1;
		cDif << cVerts(0,i+1) - cVerts(0,i), cVerts(1,i+1) - cVerts(1,i), 0;
		cN = cDif.cross(cZ);
		cN = cN/cN.norm();
		cEdge << -dWid*cN(0)+cVerts(0,i), -dWid*cN(0)+cVerts(0,i+1), dWid*cN(0)+cVerts(0,i+1), dWid*cN(0)+cVerts(0,i),
			-dWid*cN(1)+cVerts(1,i), -dWid*cN(1)+cVerts(1,i+1), dWid*cN(1)+cVerts(1,i+1), dWid*cN(1)+cVerts(1,i);
		
		DrawNgon(psTarg, cEdge.row(0), cEdge.row(1), cColos(1,i), 0);
		
		if(bRendPX) DrawNgon(psTarg, (cEdge.row(0).array() + World::Instance()->iXSize), cEdge.row(1), cColos(1,i), 0);
		if(bRendMX) DrawNgon(psTarg, (cEdge.row(0).array() - World::Instance()->iXSize), cEdge.row(1), cColos(1,i), 0);

		if(bRendPY) DrawNgon(psTarg, cEdge.row(0), (cEdge.row(1).array() + World::Instance()->iYSize), cColos(1,i), 0);
		if(bRendMY) DrawNgon(psTarg, cEdge.row(0), (cEdge.row(1).array() - World::Instance()->iYSize), cColos(1,i), 0);

		if(bRendPX && bRendPY) DrawNgon(psTarg, (cEdge.row(0).array() + World::Instance()->iXSize), (cEdge.row(1).array() + World::Instance()->iYSize), cColos(1,i), 0);
		if(bRendPX && bRendMY) DrawNgon(psTarg, (cEdge.row(0).array() + World::Instance()->iXSize), (cEdge.row(1).array() - World::Instance()->iYSize), cColos(1,i), 0);
		if(bRendMX && bRendPY) DrawNgon(psTarg, (cEdge.row(0).array() - World::Instance()->iXSize), (cEdge.row(1).array() + World::Instance()->iYSize), cColos(1,i), 0);
		if(bRendMX && bRendMY) DrawNgon(psTarg, (cEdge.row(0).array() - World::Instance()->iXSize), (cEdge.row(1).array() - World::Instance()->iYSize), cColos(1,i), 0);
		*/
		
		DrawLine(psTarg, cVerts(0, i), cVerts(1, i), cVerts(0, i+1), cVerts(1, i+1), cColos(1,i));

		if(bRendPX) DrawLine(psTarg, cVerts(0, i) + World::Instance()->iXSize, cVerts(1, i), cVerts(0, i+1) + World::Instance()->iXSize, cVerts(1, i+1), cColos(1,i));
		if(bRendMX) DrawLine(psTarg, cVerts(0, i) - World::Instance()->iXSize, cVerts(1, i), cVerts(0, i+1) - World::Instance()->iXSize, cVerts(1, i+1), cColos(1,i));

		if(bRendPY) DrawLine(psTarg, cVerts(0, i), cVerts(1, i) + World::Instance()->iYSize, cVerts(0, i+1), cVerts(1, i+1) + World::Instance()->iYSize, cColos(1,i));
		if(bRendMY) DrawLine(psTarg, cVerts(0, i), cVerts(1, i) - World::Instance()->iYSize, cVerts(0, i+1), cVerts(1, i+1) - World::Instance()->iYSize, cColos(1,i));

		if(bRendPX && bRendPY) DrawLine(psTarg, cVerts(0, i) + World::Instance()->iXSize, cVerts(1, i) + World::Instance()->iYSize, cVerts(0, i+1) + World::Instance()->iXSize, cVerts(1, i+1) + World::Instance()->iYSize, cColos(1,i));
		if(bRendPX && bRendMY) DrawLine(psTarg, cVerts(0, i) + World::Instance()->iXSize, cVerts(1, i) - World::Instance()->iYSize, cVerts(0, i+1) + World::Instance()->iXSize, cVerts(1, i+1) - World::Instance()->iYSize, cColos(1,i));
		if(bRendMX && bRendPY) DrawLine(psTarg, cVerts(0, i) - World::Instance()->iXSize, cVerts(1, i) + World::Instance()->iYSize, cVerts(0, i+1) - World::Instance()->iXSize, cVerts(1, i+1) + World::Instance()->iYSize, cColos(1,i));
		if(bRendMX && bRendMY) DrawLine(psTarg, cVerts(0, i) - World::Instance()->iXSize, cVerts(1, i) - World::Instance()->iYSize, cVerts(0, i+1) - World::Instance()->iXSize, cVerts(1, i+1) - World::Instance()->iYSize, cColos(1,i));
	}
	/*
	int i = iNumVerts-1;
	cDif << cVerts(0,0) - cVerts(0,iNumVerts-1), cVerts(1,0) - cVerts(1,iNumVerts-1), 0;
	cN = cDif.cross(cZ);
	cN = cN/cN.norm();
	cEdge << -dWid*cN(0)+cVerts(0,iNumVerts-1), -dWid*cN(0)+cVerts(0,0), dWid*cN(0)+cVerts(0,0), dWid*cN(0)+cVerts(0,iNumVerts-1),
		-dWid*cN(1)+cVerts(1,iNumVerts-1), -dWid*cN(1)+cVerts(1,0), dWid*cN(1)+cVerts(1,0), dWid*cN(1)+cVerts(1,iNumVerts-1);
		
	DrawNgon(psTarg, cEdge.row(0), cEdge.row(1), cColos(1,0), 0);

	if(bRendPX) DrawNgon(psTarg, (cEdge.row(0).array() + World::Instance()->iXSize), cEdge.row(1), cColos(1,i), 0);
	if(bRendMX) DrawNgon(psTarg, (cEdge.row(0).array() - World::Instance()->iXSize), cEdge.row(1), cColos(1,i), 0);

	if(bRendPY) DrawNgon(psTarg, cEdge.row(0), (cEdge.row(1).array() + World::Instance()->iYSize), cColos(1,i), 0);
	if(bRendMY) DrawNgon(psTarg, cEdge.row(0), (cEdge.row(1).array() - World::Instance()->iYSize), cColos(1,i), 0);

	if(bRendPX && bRendPY) DrawNgon(psTarg, (cEdge.row(0).array() + World::Instance()->iXSize), (cEdge.row(1).array() + World::Instance()->iYSize), cColos(1,i), 0);
	if(bRendPX && bRendMY) DrawNgon(psTarg, (cEdge.row(0).array() + World::Instance()->iXSize), (cEdge.row(1).array() - World::Instance()->iYSize), cColos(1,i), 0);
	if(bRendMX && bRendPY) DrawNgon(psTarg, (cEdge.row(0).array() - World::Instance()->iXSize), (cEdge.row(1).array() + World::Instance()->iYSize), cColos(1,i), 0);
	if(bRendMX && bRendMY) DrawNgon(psTarg, (cEdge.row(0).array() - World::Instance()->iXSize), (cEdge.row(1).array() - World::Instance()->iYSize), cColos(1,i), 0);
	*/
	
	DrawLine(psTarg, cVerts(0, iNumVerts-1), cVerts(1, iNumVerts-1), cVerts(0, 0), cVerts(1, 0), cColos(1,iNumVerts-1));

	if(bRendPX) DrawLine(psTarg, cVerts(0, iNumVerts-1) + World::Instance()->iXSize, cVerts(1, iNumVerts-1), cVerts(0, 0) + World::Instance()->iXSize, cVerts(1, 0), cColos(1,iNumVerts-1));
	if(bRendMX) DrawLine(psTarg, cVerts(0, iNumVerts-1) - World::Instance()->iXSize, cVerts(1, iNumVerts-1), cVerts(0, 0) - World::Instance()->iXSize, cVerts(1, 0), cColos(1,iNumVerts-1));

	if(bRendPY) DrawLine(psTarg, cVerts(0, iNumVerts-1), cVerts(1, iNumVerts-1) + World::Instance()->iYSize, cVerts(0, 0), cVerts(1, 0) + World::Instance()->iYSize, cColos(1,iNumVerts-1));
	if(bRendMY) DrawLine(psTarg, cVerts(0, iNumVerts-1), cVerts(1, iNumVerts-1) - World::Instance()->iYSize, cVerts(0, 0), cVerts(1, 0) - World::Instance()->iYSize, cColos(1,iNumVerts-1));

	if(bRendPX && bRendPY) DrawLine(psTarg, cVerts(0, iNumVerts-1) + World::Instance()->iXSize, cVerts(1, iNumVerts-1) + World::Instance()->iYSize, cVerts(0, 0) + World::Instance()->iXSize, cVerts(1, 0) + World::Instance()->iYSize, cColos(1,iNumVerts-1));
	if(bRendPX && bRendMY) DrawLine(psTarg, cVerts(0, iNumVerts-1) + World::Instance()->iXSize, cVerts(1, iNumVerts-1) - World::Instance()->iYSize, cVerts(0, 0) + World::Instance()->iXSize, cVerts(1, 0) - World::Instance()->iYSize, cColos(1,iNumVerts-1));
	if(bRendMX && bRendPY) DrawLine(psTarg, cVerts(0, iNumVerts-1) - World::Instance()->iXSize, cVerts(1, iNumVerts-1) + World::Instance()->iYSize, cVerts(0, 0) - World::Instance()->iXSize, cVerts(1, 0) + World::Instance()->iYSize, cColos(1,iNumVerts-1));
	if(bRendMX && bRendMY) DrawLine(psTarg, cVerts(0, iNumVerts-1) - World::Instance()->iXSize, cVerts(1, iNumVerts-1) - World::Instance()->iYSize, cVerts(0, 0) - World::Instance()->iXSize, cVerts(1, 0) - World::Instance()->iYSize, cColos(1,iNumVerts-1));
	
}