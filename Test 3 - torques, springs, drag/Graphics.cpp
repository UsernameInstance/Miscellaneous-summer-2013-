#include "Graphics.h"
#include <vector>

unsigned int Blend_Avg(unsigned int uiSource, unsigned int uiTarget) {
	//32 bits per pixel
  unsigned int sourceb = (uiSource >>  0) & 0xff;
  unsigned int sourceg = (uiSource >>  8) & 0xff;
  unsigned int sourcer = (uiSource >> 16) & 0xff;
  unsigned int targetb = (uiTarget >>  0) & 0xff;
  unsigned int targetg = (uiTarget >>  8) & 0xff;
  unsigned int targetr = (uiTarget >> 16) & 0xff;

  targetr = (sourcer + targetr) / 2;
  targetg = (sourceg + targetg) / 2;
  targetb = (sourceb + targetb) / 2;

  return (targetb <<  0) |
         (targetg <<  8) |
         (targetr << 16);
}

unsigned int blend_mul(unsigned int source, unsigned int target)
{
  unsigned int sourcer = (source >>  0) & 0xff;
  unsigned int sourceg = (source >>  8) & 0xff;
  unsigned int sourceb = (source >> 16) & 0xff;
  unsigned int targetr = (target >>  0) & 0xff;
  unsigned int targetg = (target >>  8) & 0xff;
  unsigned int targetb = (target >> 16) & 0xff;

  targetr = (sourcer * targetr) >> 8;
  targetg = (sourceg * targetg) >> 8;
  targetb = (sourceb * targetb) >> 8;

  return (targetr <<  0) |
         (targetg <<  8) |
         (targetb << 16);
}

unsigned int blend_add(unsigned int source, unsigned int target)
{
  unsigned int sourcer = (source >>  0) & 0xff;
  unsigned int sourceg = (source >>  8) & 0xff;
  unsigned int sourceb = (source >> 16) & 0xff;
  unsigned int targetr = (target >>  0) & 0xff;
  unsigned int targetg = (target >>  8) & 0xff;
  unsigned int targetb = (target >> 16) & 0xff;

  targetr += sourcer;
  targetg += sourceg;
  targetb += sourceb;

  if (targetr > 0xff) targetr = 0xff;
  if (targetg > 0xff) targetg = 0xff;
  if (targetb > 0xff) targetb = 0xff;

  return (targetr <<  0) |
         (targetg <<  8) |
         (targetb << 16);
}

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
//void DrawLine(SDL_Surface* psTarg, const Eigen::Vector2d & cV1, const Eigen::Vector2d & cV2, int iC) {
/*
void DrawLine(SDL_Surface* psTarg, double dX1, double dY1, double dX2, double dY2, int iC) {
	if(psTarg == 0) return;

	if(iC < 0) iC = 0; 
	else if(iC > 0xFFFFFFFF) iC = 0xFFFFFFFF;

	double dIX, dSX, dIY, dSY;
	if(dX1 > dX2) dSX = dX1, dIX = dX2;
	else dSX = dX2, dIX = dX1;

	if(dY1 > dY2) dSY = dY1, dIY = dY2;
	else dSY = dY2, dIY = dY1;

	if( (dSX < 0) || (dIX > psTarg->w) || (dSY < 0) || (dIY > psTarg->h) ) return;

	if(SDL_MUSTLOCK(psTarg)) if(SDL_LockSurface(psTarg) < 0) return;

	double dDIX, dDIY, dDSX, dDSY;
	dDIX = dIX; dDSX = dSX;
	dDIY = dIY; dDSY = dSY;
		
	if(dDIX < 0) dDIX = 0;
	if(dDSX > psTarg->w) dDSX = psTarg->w-1;
	if(dDIY < 0) dDIY = 0;
	if(dDSY > psTarg->h) dDSY = psTarg->h-1;

	int iLIX, iLSX, iLIY, iLSY;
	iLIX = int(dDIX); iLSX = int(dDSX);
	iLIY = int(dDIY); iLSY = int(dDSY);

	int iYoff;// = psTarg->pitch/4;
	int iXoff;

	if(iLIY == iLSY) {
		iYoff = iLIY*psTarg->pitch/4;
		for(int x=iLIX; x<=iLSX; x++) *((unsigned int*)(psTarg->pixels) + iYoff + x) = iC;
	}
	else {

		for(int y=iLIY; y<=dDSY; y++) {
			iYoff = y*psTarg->pitch/4;
			iXoff = int( (dX2 - dX1)*(y - dY1)/(dY2 - dY1) + dX1 );
			 *((unsigned int*)(psTarg->pixels) + iYoff + iXoff) = iC;
		}
	
	}
	


	if(SDL_MUSTLOCK(psTarg)) SDL_UnlockSurface(psTarg);

}*/

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

double ZCP(const Eigen::Vector2d& cV0, const Eigen::Vector2d& cV1, const Eigen::Vector2d& cV2) {
	//Return z-component of cross product of cV1 - cV0 and cV2 - cV0
	return ( (cV1(0)-cV0(0))*(cV2(1)-cV0(1))-(cV2(0)-cV0(0))*(cV1(1)-cV0(1)) );
}

bool IsInNgon(const Eigen::RowVectorXd& cVertX, const Eigen::RowVectorXd& cVertY, const Eigen::Vector2d& cP) {
	//if on boundary, returns true
	int iW, iN; //iW = winding number, iN = number of vertices

	double dZcp;

	iW = 0; iN = cVertX.size();

	Eigen::Vector2d cTVec0, cTVec1;

	for(int k = 0; k < iN; k++) {
		cTVec0 << cVertX(k), cVertY(k);
		cTVec1 << cVertX((k+1)%iN), cVertY((k+1)%iN);
				
		if( !( cTVec0(1) == cTVec1(1) ) ) {
			dZcp = ZCP(cTVec0, cTVec1, cP);
			if(dZcp == 0) return true;

			if( (cTVec0(1) <= cP(1)) && (cP(1) <= cTVec1(1)) ) {
				if(dZcp < 0) --iW;
			}
			else if( (cTVec0(1) >= cP(1)) && (cP(1) >= cTVec1(1) ) ) {
				if(dZcp > 0) ++iW;
			}	
		}
	}

	if(iW != 0) return true;
	else return false;
}

/* Runs like crap because of the obscene number of containment checks.
void DrawNgon(SDL_Surface* psTarg, const Eigen::RowVectorXd& cVertX, const Eigen::RowVectorXd& cVertY, int iC, int iBlend) {
	if((psTarg == NULL) || (cVertX.size() < 3) || (cVertY.size() < 3) || (cVertX.size() != cVertY.size())) return;

	if(iC < 0) iC = 0; 
	else if(iC > 0xFFFFFFFF) iC = 0xFFFFFFFF;

	double dIX, dSX, dIY, dSY; //infs and sups of legal n-gon.

	dIX = cVertX.minCoeff(); dSX = cVertX.maxCoeff();
	dIY = cVertY.minCoeff(); dSY = cVertY.maxCoeff();

	if( (dSX < 0) || (dIX >= psTarg->w) || (dSY < 0) || (dIY >= psTarg->h) ) return; //check if on screen

	if(iBlend < 0) iBlend = 0;
	else if(iBlend > 3) iBlend = 3;

	if(SDL_MUSTLOCK(psTarg)) if(SDL_LockSurface(psTarg) < 0) return;
	//set correct drawing range

	double dDIX, dDSX, dDIY, dDSY; //infs and sups of legal drawing range.
	int iLIX, iLSX; //loop drawing inf and sup X;
	dDIX = dIX; dDSX = dSX;
	dDIY = dIY; dDSY = dSY;
		
	if(dDIX < 0) dDIX = 0;
	if(dDSX > psTarg->w) dDSX = psTarg->w;
	if(dDIY < 0) dDIY = 0;
	if(dDSY > psTarg->h) dDSY = psTarg->h;
	
	int iN, iYOff; //iN = number of vertices

	iN = cVertX.size();

	Eigen::Vector2d cP; //Point used in polygon containment check.

	std::vector<double> cXin;
	
	cXin.push_back(dIX);

	for(int y = int(dDIY); y < int(dDSY); y++) {
		for(int i = 0, i_2 = 0; i < iN; i++) {
			if(i<iN-1) i_2 = i+1;
			else i_2 = 0;
			if( ((cVertY(i) < y) && (y <= cVertY(i_2))) || ((cVertY(i) > y) && (y >= cVertY(i_2))) ){
				if(cVertY(i) != cVertY(i_2)) cXin.push_back( (cVertX(i_2) - cVertX(i))*((y - cVertY(i))/(cVertY(i_2) - cVertY(i))) + cVertX(i) ); 
			}
		}
		cXin.push_back(dSX); std::sort(cXin.begin(), cXin.end());
		cP(1) = y;
		for(int k = 0; k < int(cXin.size())-1; k++) { 
			cP(0) = (cXin[k] + cXin[k+1])/2.0;
			if( !(cXin[k] > dDSX || cXin[k+1] < dDIX) ) {
				if(IsInNgon(cVertX, cVertY, cP)) {
					if(cXin[k] < dDIX) iLIX = int(dDIX);
					else iLIX = int(cXin[k]);
					if(cXin[k+1] > dDSX) iLSX = int(dDSX);
					else iLSX = int(cXin[k+1]);

					iYOff = y * (psTarg->pitch / 4);
					
					switch(iBlend) {
					case 0:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = iC;
						break;
					case 1:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = Blend_Avg(iC, *( (unsigned int*)(psTarg->pixels) + iYOff + x));
						break;
					case 2:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = blend_mul(iC, *( (unsigned int*)(psTarg->pixels) + iYOff + x));
						break;
					case 3:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = blend_add(iC, *( (unsigned int*)(psTarg->pixels) + iYOff + x));
						break;
					}
					
					//DrawLine(psTarg, cXin[k], y, cXin[k+1], y, iC); }
				}
			}
		}
		cXin.clear();
		
	}

	if(SDL_MUSTLOCK(psTarg)) SDL_UnlockSurface(psTarg);
}
*/
struct sort_pred {
	bool operator()(const std::pair<double, int> &left, const std::pair<double, int> &right) {
		return left.first < right.first;
	}
};
//2.5x better
void DrawNgon(SDL_Surface* psTarg,const Eigen::RowVectorXd& cVertX, const Eigen::RowVectorXd& cVertY, int iC, int iB) {
	if((psTarg == NULL) || (cVertX.size() < 3) || (cVertY.size() < 3) || (cVertX.size() != cVertY.size())) return;

	if(iC < 0) iC = 0; 
	else if(iC > 0xFFFFFFFF) iC = 0xFFFFFFFF;

	double dIX, dSX, dIY, dSY; //infs and sups of legal n-gon.

	dIX = cVertX.minCoeff(); dSX = cVertX.maxCoeff();
	dIY = cVertY.minCoeff(); dSY = cVertY.maxCoeff();

	if( (dSX < 0) || (dIX > psTarg->w) || (dSY < 0) || (dIY > psTarg->h) ) return; //check if on screen

	if(SDL_MUSTLOCK(psTarg)) if(SDL_LockSurface(psTarg) < 0) return;

	if(iB < 0) iB = 0;
	else if(iB > 3) iB = 3;
	
	//set correct drawing range
	bool bIn = false; 
	double dDIX, dDSX, dDIY, dDSY; //infs and sups of legal drawing range.
	int iLIX, iLSX; //loop drawing inf and sup X;
	int iLIY, iLSY;
	dDIX = dIX; dDSX = dSX;
	dDIY = dIY; dDSY = dSY;
		
	if(dDIX < 0) dDIX = 0;
	if(dDSX > psTarg->w) dDSX = psTarg->w;
	if(dDIY < 0) dDIY = 0;
	if(dDSY > psTarg->h) dDSY = psTarg->h;
	
	int iN, iYOff, iInd, iInd2, iInd3, iInd4; //iN = number of vertices

	iN = cVertX.size();

	Eigen::Vector2d cP; //Point used in polygon containment check.
	Eigen::Matrix<double,2,4> cZVert; //Zone vertices. Col a = vertex a. Left to right top to bottom.
	cZVert.setZero();

	std::vector<std::pair<double,int>> cXin; //first element = intersection, 2nd element = original index wrt cVert
	std::vector<double> cYv, cYp;

	for(int i=0; i < cVertY.size(); i++) cYv.push_back(cVertY(i));
	std::sort(cYv.begin(), cYv.end()); cYv.erase(std::unique(cYv.begin(), cYv.end()), cYv.end());

	for(int i=0; i<int(cYv.size())-1; i++) cYp.push_back((cYv[i]+cYv[i+1])/2.0);
	
	for(int i=0, i_2=0; i<int(cYp.size()); i++) {
		i_2 = i+1;

		for(int j = 0, j_2 = 0; j < iN; j++) {
			if(j<iN-1) j_2 = j+1;
			else j_2 = 0;
			if( ((cVertY(j) < cYp[i]) && (cYp[i] < cVertY(j_2))) ||  ((cVertY(j) > cYp[i]) && (cYp[i] > cVertY(j_2))) ){
				cXin.push_back(std::make_pair((cVertX(j_2) - cVertX(j))*(cYp[i] - cVertY(j))/(cVertY(j_2) - cVertY(j)) + cVertX(j), j)); 
			}
		}
		std::sort(cXin.begin(), cXin.end(), sort_pred());
		

		if(cXin.size() > 0) cP << cXin[0].first+0.001, cYp[i];
		else cP << 0.001, cYp[i];
		if(IsInNgon(cVertX, cVertY, cP)) bIn = true;
		else bIn = false;
		
		
		for(int k=0; k<int(cXin.size())-1; k++) {
			if(bIn) {
				bIn = false;
				
				iInd = (cXin[k].second);
				if(cVertY(iInd) < cYp[i]) {
					iInd2 = iInd;
					iInd = (iInd+1)%iN;
				}
				else iInd2= (iInd+1)%iN;

				iInd3 = cXin[k+1].second;
				if(cVertY(iInd3) < cYp[i]) {
					iInd4 = iInd3;
					iInd3 = (iInd3+1)%iN;
				}
				else iInd4 = (iInd3+1)%iN;

				cZVert(0,0) = (cVertX(iInd2) - cVertX(iInd))*(cYv[i] - cVertY(iInd))/(cVertY(iInd2) - cVertY(iInd)) + cVertX(iInd);
				cZVert(1,0) = cYv[i];
				cZVert(0,1) = (cVertX(iInd4) - cVertX(iInd3))*(cYv[i] - cVertY(iInd3))/(cVertY(iInd4) - cVertY(iInd3)) + cVertX(iInd3);
				cZVert(1,1) = cYv[i];
				cZVert(0,2) = (cVertX(iInd2) - cVertX(iInd))*(cYv[i_2] - cVertY(iInd))/(cVertY(iInd2) - cVertY(iInd)) + cVertX(iInd);
				cZVert(1,2) = cYv[i_2];
				cZVert(0,3) = (cVertX(iInd4) - cVertX(iInd3))*(cYv[i_2] - cVertY(iInd3))/(cVertY(iInd4) - cVertY(iInd3)) + cVertX(iInd3);
				cZVert(1,3) = cYv[i_2];
				
				
				iLIY = int(cYv[i]); iLSY = int(cYv[i_2]);
				if(iLIY < dDIY) iLIY = int(dDIY);
				if(iLSY > dDSY) iLSY = int(dDSY);
				
				for(int y=iLIY; y<iLSY; y++) {
					iLIX = int( (cZVert(0,2)-cZVert(0,0))*(double(y) - cZVert(1,0))/(cZVert(1,2)-cZVert(1,0)) + cZVert(0,0) );
					iLSX = int( (cZVert(0,3)-cZVert(0,1))*(double(y) - cZVert(1,1))/(cZVert(1,3)-cZVert(1,1)) + cZVert(0,1) );

					if(iLIX < dDIX) iLIX = int(dDIX);
					if(iLSX > dDSX) iLSX = int(dDSX);

					iYOff = y * (psTarg->pitch / 4);
					
					switch(iB) {
					case 0:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = iC;
						break;
					case 1:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = Blend_Avg(iC, *( (unsigned int*)(psTarg->pixels) + iYOff + x));
						break;
					case 2:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = blend_mul(iC, *( (unsigned int*)(psTarg->pixels) + iYOff + x));
						break;
					case 3:
						for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = blend_add(iC, *( (unsigned int*)(psTarg->pixels) + iYOff + x));
						break;
					}
					//for(int x = iLIX; x < iLSX; x++) *( (unsigned int*)(psTarg->pixels) + iYOff + x) = iC;
					
				}
				

			}
			else bIn = true;
		}
		cXin.clear();
		
	}
	
	if(SDL_MUSTLOCK(psTarg)) SDL_UnlockSurface(psTarg);
}