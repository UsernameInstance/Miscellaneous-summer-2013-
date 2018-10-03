#include "Graphics.h"

void FillSurf(SDL_Surface* Targ, int Color) { //32bpp
	if(Targ == NULL) return;
	if(Color < 0) Color = 0;
	else if(Color > 0xFFFFFFFF) Color = 0xFFFFFFFF;
	
	if(SDL_MUSTLOCK(Targ)) if(SDL_LockSurface(Targ) < 0) return;

	int i, j, yofs, ofs;
	yofs = 0;

	for (i = 0; i < Targ->h; i++) {
		for(j = 0, ofs = yofs; j < Targ->w; j++, ofs++) ((unsigned int*)Targ->pixels)[ofs] = Color;
		yofs += Targ->pitch / 4;
	} 

	if(SDL_MUSTLOCK(Targ)) SDL_UnlockSurface(Targ);
}

void DrawCircle(SDL_Surface* psTarg, double dR, double dX, double dY, int iC) { //32 bpp, (iX, iY) center of circle, iR is radius
	if(dR < 0 || psTarg == NULL) return; //undefined
	//check if offscreen
	if((dX + dR < 0) || (dX - dR > psTarg->w) || (dY - dR > psTarg->h) || (dY + dR < 0)) return; 

	if(iC < 0) iC = 0;
	else if(iC > 0xFFFFFFFF) iC = 0xFFFFFFFF;
	
	if(SDL_MUSTLOCK(psTarg)) if(SDL_LockSurface(psTarg) < 0) return;
	
	double dW = 0; //width given current height j relative to surface psTarg origin
	int iYofs = 0, iOfs = 0, iXOfs = 0;

	for(int j = int(dY - dR); j <= dY + dR; j++) {
		dW = 2*sqrt(pow(dR, 2) - pow(double(j) - dY, 2));
		iYofs = j * psTarg->pitch/4;
		//clipping
		//vertical
		if(j >= 0 && j < psTarg->h) { //first line of array is indexed at 0, so last line (horizontal) is 1 minus height of surface
			//horizontal
			iXOfs = int(dX - dW/2);
			if(iXOfs < 0) dW += iXOfs, iXOfs = 0; //left
			if(iXOfs + dW > psTarg->w) dW = (psTarg->w - iXOfs); //right

			for(int i = 0; i < dW; i++) {
				iOfs = iYofs + iXOfs;
				((unsigned int*)psTarg->pixels)[iOfs + i] = iC;
			}
		}
	}

	if(SDL_MUSTLOCK(psTarg)) SDL_UnlockSurface(psTarg);
}

void DrawLine(SDL_Surface* psTarg, double dX1, double dY1, double dX2, double dY2, int iC) {
	if(psTarg == NULL) return;

	if(iC < 0) iC = 0; 
	else if(iC > 0xFFFFFFFF) iC = 0xFFFFFFFF;

	if((dX1 < 0) && (dX2 < 0) || (dX1 >= psTarg->w) && (dX2 >= psTarg->w) || (dY1 < 0 && dY2 < 0) || (dY1 >= psTarg->h && (dY2 >= psTarg->h))) return;

	if(SDL_MUSTLOCK(psTarg)) if(SDL_LockSurface(psTarg) < 0) return;

	if(dX1 == dX2 && dY1 == dY2) { //point
		if((0 <= dX1 && dX1 < psTarg->w) && (0 <= dY1 && dY1 < psTarg->h)) {
			((unsigned int*)psTarg->pixels)[int(dY1) * (psTarg->pitch / 4) + int(dX1)] = iC;
		}
	}
	else if(dX1 == dX2) { //vertical
		if(0 <= dX1 && dX1 < psTarg->w) {
			int iMin, iMax;

			if(dY1 < dY2) iMin = int(dY1), iMax = int(dY2);
			else iMin = int(dY2), iMax = int(dY1);

			if(iMax >= 0 && iMin <= psTarg->h) {
				if(iMin < 0) iMin = 0;
				if(iMax > psTarg->h) iMax = psTarg->h;
				for(int i = iMin; i < iMax; i++) ((unsigned int*)psTarg->pixels)[i*psTarg->pitch/4 + int(dX1)] = iC;
			}
		}
	}
	else if(dY1 == dY2) {//horizontal
		if(0 <= dY1 && dY1 < psTarg->h) {
			int iMin, iMax;
			int iYof = int(dY1) * (psTarg->pitch/4);

			if(dX1 < dX2) iMin = int(dX1), iMax = int(dX2);
			else iMin = int(dX2), iMax = int(dX1);

			if(iMax >= 0 && iMin <= psTarg->w) {
				if(iMin < 0) iMin = 0;
				if(iMax > psTarg->w) iMax = psTarg->w;
				for(int i = iMin; i < iMax; i++) ((unsigned int*)psTarg->pixels)[iYof + i] = iC;
			}
		}
	}
	else { //oblique y1 != y2 && x1 != x2
		int iXMin, iXMax, iYMin, iYMax;

		if(dX1 < dX2) iXMin = int(dX1), iXMax = int(dX2);
		else iXMin = int(dX2), iXMax = int(dX1);
		
		if(dY1 < dY2) iYMin = int(dY1), iYMax = int(dY2);
		else iYMin = int(dY2), iYMax = int(dY1); 
	
		//endpoints must be of the form ( (iXMin, iYMin) and (iXMax, iYMax) ) or ( (iXMin, iYMax) and (iXMax, iYMin) )
		//meh.. just calculate y values using x values or vice versa
		double dSl = (dY2 - dY1)/(dX2 - dX1); //slope, pos implies xmin, ymin & xmax,ymax, neg implies alternative (above)
		
		if(iXMax >= 0 && iXMin < psTarg->w && iYMax >= 0 && iYMin < psTarg->h) {
			double j, i, b; //b is y-intercept
			if(dSl < 0) b = double(iYMax) - dSl * double(iXMin);
			else b = double(iYMin) - dSl * double(iXMin);
			//these are input bounds NOT output bounds?
			if(iXMin < 0) iXMin = 0;
			if(iYMin < 0) iYMin = 0;
			if(iXMax >= psTarg->w) iXMax = psTarg->w;
			if(iYMax >= psTarg->h) iYMax = psTarg->h;			

			int iOfs;
			//if abs (slope)  is greater than 1 than there is blank space between pixels since pixels are skipped
			//solution: use larger domain. since inverse slope is then less than 1
			if(abs(dSl) < 1) {
				for(i = iXMin; i < iXMax; i++) {
					j = dSl * i + b; 
					if(j >= 0 && j < psTarg->h) { //check corners
						iOfs = int(j) * (psTarg->pitch / 4) + int(i);
						((unsigned int*)psTarg->pixels)[iOfs] = iC;
					}
				}
			}
			else {
				for(j = iYMin; j < iYMax; j++) {
					i = (j/dSl) - (b/dSl);
					if(i >= 0 && i < psTarg->w) {
						iOfs = int(j) * (psTarg->pitch / 4) + int(i);
						((unsigned int*)psTarg->pixels)[iOfs] = iC;
					}
				}
			}
		}
	}
	
	if(SDL_MUSTLOCK(psTarg)) SDL_UnlockSurface(psTarg);
}

void DrawRectangle(SDL_Surface* psTarg, double dX, double dY, double dXLen, double dYLen, double dAng, int iC) {//dX, dY are center coordinates. dAng is angle in radians relative to left handed cartesian coordinate system with origin at dX, dY up to line parallel to x axis when dAng = 0
	if(psTarg == NULL) return;

	if(iC < 0) iC = 0; 
	else if(iC > 0xFFFFFFFF) iC = 0xFFFFFFFF;

	if(SDL_MUSTLOCK(psTarg)) if(SDL_LockSurface(psTarg) < 0) return;
	double dAx, dAy, dXc1, dYc1, dXc2, dYc2;
	dAx = (dXLen/2.0)*(cos(dAng)) + dX;
	dAy = (dXLen/2.0)*(sin(dAng)) + dY;
	dXc1 = dAx + (dYLen/2.0)*(sin(dAng));
	dYc1 = dAy - (dYLen/2.0)*(cos(dAng));
	dXc2 = dAx - (dYLen/2.0)*(sin(dAng));
	dYc2 = dAy + (dYLen/2.0)*(cos(dAng));

	//Lx(t) = dXc1 + (dXc2 - dXc1)t, Ly(t) = dYc1 + (dYc2 - dYc1)t, 0 <= t <= 1
	//Mx(t) = Lx(t) + (dXLen*cos(dAng + PI))t, My(t) = Ly(t) + (dXLen*sin(dAng + PI))t 
	//use step-size h = 1/(sqrt(pow(dXc1 - dXc2, 2) + pow(dYc1 - dYc2, 2))) or maybe half that
	//set angle dependent extra lines parameter to prevent missing pixels when diagonal

	double h = 1.0/(sqrt(pow(dXc2 - dXc1, 2) + pow(dYc2 - dYc1, 2)));
	double t = 0; double Lx, Ly, Mx, My;
	while(t <= 1) {
		Lx = dXc1 + (dXc2 - dXc1)*t; Ly = dYc1 + (dYc2 - dYc1)*t;
		Mx = Lx - dXLen*cos(dAng); My = Ly - dXLen*sin(dAng);
		DrawLine(psTarg, Lx, Ly, Mx, My, iC);
		if(t != 1) DrawLine(psTarg, Lx - 1, Ly, Mx - 1, My, iC);
		t += h;
	}

	if(SDL_MUSTLOCK(psTarg)) SDL_UnlockSurface(psTarg);
}