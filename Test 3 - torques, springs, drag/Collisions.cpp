#include "Collisions.h"
#include "Text.h"
#include "App.h"

Collisions* Collisions::s_pcCollisions = NULL;
bool Collisions::s_bInstance = false;

Collisions* Collisions::Instance() {
	if(!s_bInstance) {
		s_pcCollisions = new Collisions();
		s_bInstance = true;
	}
	return s_pcCollisions;
}

Collisions::Collisions() {
	dRBSC = 1e5;
	dRBDC = 1e-5;
	dSRBSC = sqrt(dRBSC);
	dTPUC = World::Instance()->dTPUC;
	dSRT = dSRBSC/dTPUC;
	bING = false;
	dLTime = 0;
	dDTime = 0;
	bM1X = bM2X = bP1X = bP2X = bM1Y = bM2Y = bP1Y = bP2Y = false;

	cP.setZero();
	cRelV = cCollShi = cCollOve = cCollCen = cDifVe = cDisVe = cEqVe.setZero();

}

Collisions::~Collisions() { s_bInstance = false; }

double QuadEq(double x, double c_2, double c_1, double c_0) { return (c_2*x*x + c_1*x + c_0); }
//these are bad. Integrate it yourself wimp.
double DragFI0(double u, double c_2, double c_1, double c_0) {
	double SRQ = sqrt(c_2*QuadEq(u, c_2, c_1, c_0));
	return ( (1./(8*c_2*sqrt(c_2))*(2*(2*c_2*u+c_1)*SRQ - (c_1*c_1 - 4*c_2*c_0)*log(abs(2*SRQ + 2*c_2*u+c_1)))) );
}
double DragFI1(double u, double c_2, double c_1, double c_0) {
	double SRQ = sqrt(c_2*QuadEq(u, c_2, c_1, c_0));
	return ( (1./(48*pow(c_2,2)*sqrt(c_2))) * ( 3*(c_1*c_1*c_1 - 4*c_2*c_1*c_0)*log(abs(2*SRQ + 2*c_2*u + c_1)) + 2*SRQ*(2*c_2*c_1*u + 8*c_2*(c_2*u*u + c_0) - 3*c_1*c_1) ) );
}
double DragFI2(double u, double c_2, double c_1, double c_0) {
	double SRQ = sqrt(c_2*QuadEq(u, c_2, c_1, c_0));
	return ( (1./(384*pow(c_2,3)*sqrt(c_2))) * 
		( 2*SRQ*( 24*c_2*c_2*u*(2*c_2*u*u + c_0) - 10*c_2*c_1*c_1*u + 4*c_2*c_1*(2*c_2*u*u - 13*c_0) + 15*pow(c_1,3) ) - 
		3*(16*pow(c_2*c_0,2) - 24*c_2*c_1*c_1*c_0 + 5*pow(c_1,4)) * log(abs(2*SRQ + 2*c_2*u + c_1)) ) );
}


void Collisions::Loop() {
	//Attempted a Hookes law derivation previously. It will not work for interactions involving multiple entities. See n-body problem.
	
	//dDTime = World::Instance()->dDTime; //1e-3 * double( SDL_GetTicks() - dLTime ); //Divided by 1000 to convert to seconds.
	//dLTime = World::Instance()->dLTime; //SDL_GetTicks();

	if(g_cBodyList.size() > 0) {
	
		for(int i = 0; i < int(g_cBodyList.size()); i++) {
			//Using render check bools will cause inaccurate behavior if render updates per seconds != physics updates per cycle
			bM1X = g_cBodyList[i]->bRendMX;
			bP1X = g_cBodyList[i]->bRendPX;
			bM1Y = g_cBodyList[i]->bRendMY;
			bP1Y = g_cBodyList[i]->bRendPY;

			for(int j = i; j < int(g_cBodyList.size()); j++) {

				bM2X = g_cBodyList[j]->bRendMX;
				bP2X = g_cBodyList[j]->bRendPX;
				bM2Y = g_cBodyList[j]->bRendMY;
				bP2Y = g_cBodyList[j]->bRendPY;

				if(!(j==i)) {
					for(int k = 0; k < int(g_cBodyList[i]->cVerts.cols()); k++) { 
						for(int l = 0; l < int(g_cBodyList[j]->cVerts.cols()); l++) { //later add a box check to see if any vertices are in range before running through these.
							for(int a = -1; a < 2; a++) {
								if((a == -1) && !bM1X) continue;
								if((a == 1) && !bP1X) continue;

								for(int b = -1; b < 2; b++) {
									if((b == -1) && !bM1Y) continue;
									if((b == 1) && !bP1Y) continue;

									for(int c = -1; c < 2; c++) {
										if((c == -1) && !bM2X) continue;
										if((c == 1) && !bP2X) continue;

										for(int d = -1; d < 2; d++) {
											if((d == -1) && !bM2Y) continue;
											if((d == 1) && !bP2Y) continue;
											
											cCollCen << (-(g_cBodyList[i]->cVerts(0,k) + a*World::Instance()->iXSize)
													+ g_cBodyList[j]->cVerts(0,l) + c*World::Instance()->iXSize), 
												(-(g_cBodyList[i]->cVerts(1,k) + b*World::Instance()->iYSize) 
													+ g_cBodyList[j]->cVerts(1,l) + d*World::Instance()->iYSize);
											
											if(cCollCen.norm() <= g_cBodyList[i]->cProps(1,k) + g_cBodyList[j]->cProps(1,l)) {
												cCollOve = cCollCen*(1./cCollCen.norm())*(cCollCen.norm() - g_cBodyList[j]->cProps(1,l));
												cCollShi = cCollCen*(1./cCollCen.norm())*(g_cBodyList[i]->cProps(1,k) - cCollOve.norm());

												cRelV << -g_cBodyList[j]->cVerts(2,l) + g_cBodyList[i]->cVerts(2,k), -g_cBodyList[j]->cVerts(3,l) + g_cBodyList[i]->cVerts(3,k);
												cRelV = (cRelV.dot(cCollCen)/pow(cCollCen.norm(),2))*cCollCen;

												g_cBodyList[i]->cVerts(6,k) += -cCollShi(0)*dRBSC - dRBDC*cRelV(0);
												g_cBodyList[i]->cVerts(7,k) += -cCollShi(1)*dRBSC - dRBDC*cRelV(1);
												
												g_cBodyList[j]->cVerts(6,l) += cCollShi(0)*dRBSC + dRBDC*cRelV(0);
												g_cBodyList[j]->cVerts(7,l) += cCollShi(1)*dRBSC + dRBDC*cRelV(1);
											}

										}
									}
								}
							}
							
						}
					}

				}
				else {
					//self check.
					if(int(g_cBodyList[j]->cVerts.cols()) > 1) {
						for(int h = 0; h < int(g_cBodyList[j]->cVerts.cols()-1); h++) {
							cDifVe << (g_cBodyList[j]->cVerts(0,h) - g_cBodyList[j]->cVerts(0,h+1)), (g_cBodyList[j]->cVerts(1,h) - g_cBodyList[j]->cVerts(1,h+1));

							if(cDifVe.norm() != g_cBodyList[j]->cProps(3,h)) {
								cEqVe = g_cBodyList[j]->cProps(3,h)/(cDifVe.norm()) * cDifVe;
								cDisVe = cDifVe - cEqVe;

								cRelV << g_cBodyList[j]->cVerts(2,h) - g_cBodyList[j]->cVerts(2,h+1), g_cBodyList[j]->cVerts(3,h) - g_cBodyList[j]->cVerts(3,h+1);
								cRelV = (cRelV.dot(cDisVe)/pow(cDisVe.norm(),2))*cDisVe;

								g_cBodyList[j]->cVerts(6,h) += -g_cBodyList[j]->cProps(2,h) * cDisVe(0) - g_cBodyList[j]->cProps(4,h)*cRelV(0);
								g_cBodyList[j]->cVerts(6,h+1) += +g_cBodyList[j]->cProps(2,h) * cDisVe(0) + g_cBodyList[j]->cProps(4,h+1)*cRelV(0);

								g_cBodyList[j]->cVerts(7,h) += -g_cBodyList[j]->cProps(2,h) * cDisVe(1) - g_cBodyList[j]->cProps(4,h)*cRelV(1);
								g_cBodyList[j]->cVerts(7,h+1) += +g_cBodyList[j]->cProps(2,h) * cDisVe(1) + g_cBodyList[j]->cProps(4,h+1)*cRelV(1);

							}
							
							//experimenting with drag force
							cA << g_cBodyList[j]->cVerts(0,h), g_cBodyList[j]->cVerts(1,h), 0;
							cB << g_cBodyList[j]->cVerts(0,h+1), g_cBodyList[j]->cVerts(1,h+1), 0;
							dU0 = g_cBodyList[j]->cProps(1,h)/(cB-cA).norm();
							dU1 = 1. - g_cBodyList[j]->cProps(1,h+1)/(cB-cA).norm();
							cVA << g_cBodyList[j]->cVerts(2,h), g_cBodyList[j]->cVerts(3,h), 0;
							cVB << g_cBodyList[j]->cVerts(2,h+1), g_cBodyList[j]->cVerts(3,h+1), 0;

							dK = ( World::Instance()->dFDensity * 1.05 /2.0 );
							dKA = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h);
							dKB = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h+1);

							double a2, a1, a0, BAdBA, BAdVBA, VBAdVA, BAdVA, c, det, chk1, chk2, chk3;
							BAdBA = (cB-cA).dot(cB-cA); BAdVBA = (cB-cA).dot(cVB-cVA); VBAdVA = (cVB-cVA).dot(cVA); BAdVA = (cB-cA).dot(cVA);
							a2 = BAdBA*( (cVB-cVA).dot(cVB-cVA) ) - pow(BAdVBA,2);
							a1 = 2*VBAdVA*BAdBA - 2*BAdVBA*BAdVA;
							a0 = BAdBA* (cVA.dot(cVA)) - BAdVA*BAdVA;
							det = a1*a1 - 4*a2*a0;
							c = a1/(2*a2);
							chk1 = 1./(384.*pow(a2,4));
							chk2 = 2*sqrt(a2*QuadEq(dU1, a2, a1, a0)) + 2*a2*dU1+a1;
							chk3 = 2*sqrt(a2*QuadEq(dU0, a2, a1, a0)) + 2*a2*dU0+a1;

							//Text::Instance()->tString = Text::Instance()->ToString<double>(cFA(0));
							//Text::Instance()->RenderString(App::Instance()->psSurfDisp, 700, 340, Text::Instance()->tString.c_str());
							
							if( !( (cVA.norm() == 0) && (cVB.norm() == 0) ) ) {
								if( (det != 0) ) {
									if( (chk1 != 0) && (chk2 != 0) && (chk3 != 0) ) {
										cF = -dK*(DragFI1(dU1, a2, a1, a0) - DragFI1(dU0, a2, a1, a0))*(cVB - cVA) - dK*(DragFI0(dU1, a2, a1, a0) - DragFI0(dU0, a2, a1, a0))*cVA;
										cFB = -dK*(DragFI2(dU1, a2, a1, a0) - DragFI2(dU0, a2, a1, a0))*(cVB - cVA) - dK*(DragFI1(dU1, a2, a1, a0) - DragFI1(dU0, a2, a1, a0))*cVA;
										cFA = cF - cFB;
									}
									else { cFA = cFB = cF.setZero(); }
								}
								else {
									if( dU1 < -c ) {
										cF = -dK*( -sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*(cVB-cVA) - sqrt(a2)*( (pow(dU1,2) - pow(dU0,2))/2. + c*(dU1-dU0) )*cVA );
										cFB = -dK*( -sqrt(a2)*( (pow(dU1,4)-pow(dU0,4))/4. + (c/3)*(pow(dU1,3) - pow(dU0,3)) )*(cVB-cVA) - sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*cVA );
										cFA = cF - cFB;
									}
									else if( dU0 >= -c ) {
										cF = -dK*( sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*(cVB-cVA) + sqrt(a2)*( (pow(dU1,2) - pow(dU0,2))/2. + c*(dU1-dU0) )*cVA );
										cFB = -dK*( sqrt(a2)*( (pow(dU1,4)-pow(dU0,4))/4. + (c/3)*(pow(dU1,3) - pow(dU0,3)) )*(cVB-cVA) + sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*cVA );
										cFA = cF - cFB;
									}
									else {
										cF = -dK*( sqrt(a2)*((pow(dU1,3) - pow(-c,3))/3. + (c/2)*(pow(dU1,2) - pow(-c,2)))*(cVB-cVA) + sqrt(a2)*( (pow(dU1,2) - pow(-c,2))/2. + c*(dU1+c) )*cVA )
											+ dK*( sqrt(a2)*((pow(-c,3) - pow(dU0,3))/3. + (c/2)*(pow(-c,2) - pow(dU0,2)))*(cVB-cVA) + sqrt(a2)*( (pow(-c,2) - pow(dU0,2))/2. + c*(-c-dU0) )*cVA );
										cFB = -dK*( sqrt(a2)*( (pow(dU1,4)-pow(-c,4))/4. + (c/3)*(pow(dU1,3) - pow(-c,3)) )*(cVB-cVA) + sqrt(a2)*((pow(dU1,3) - pow(-c,3))/3. + (c/2)*(pow(dU1,2) - pow(-c,2)))*cVA )
											+ dK*( sqrt(a2)*( (pow(-c,4)-pow(dU0,4))/4. + (c/3)*(pow(-c,3) - pow(dU0,3)) )*(cVB-cVA) + sqrt(a2)*((pow(-c,3) - pow(dU0,3))/3. + (c/2)*(pow(-c,2) - pow(dU0,2)))*cVA );
										cFA = cF - cFB;
									}
								}
								if(  !(cFB(0) != cFB(0)) && !(cFB(1) != cFB(1)) && !(cFA(0) != cFA(0)) && !(cFA(1) != cFA(1)) ) {
									g_cBodyList[j]->cVerts(6,h) += cFA(0);
									g_cBodyList[j]->cVerts(7,h) += cFA(1);

									g_cBodyList[j]->cVerts(6,h+1) += cFB(0);
									g_cBodyList[j]->cVerts(7,h+1) += cFB(1);
								}
							}

							g_cBodyList[j]->cVerts(6,h) += -dKA*cVA.norm()*cVA(0);
							g_cBodyList[j]->cVerts(7,h) += -dKA*cVA.norm()*cVA(1);

							g_cBodyList[j]->cVerts(6,h+1) += -dKB*cVB.norm()*cVB(0);
							g_cBodyList[j]->cVerts(7,h+1) += -dKB*cVB.norm()*cVB(1);

							/*
							cZ << 0,0,1;
							cA << g_cBodyList[j]->cVerts(0,h), g_cBodyList[j]->cVerts(1,h), 0;
							cB << g_cBodyList[j]->cVerts(0,h+1), g_cBodyList[j]->cVerts(1,h+1), 0;
							cC = (0.5)*(cB + cA);
							cN = (cB-cA).cross(cZ);
							cVA << g_cBodyList[j]->cVerts(2,h), g_cBodyList[j]->cVerts(3,h), 0;
							cVB << g_cBodyList[j]->cVerts(2,h+1), g_cBodyList[j]->cVerts(3,h+1), 0;
							cVC << (0.5)*(cVB + cVA);
							dMA = g_cBodyList[j]->cProps(0,h);
							dMB = g_cBodyList[j]->cProps(0,h+1);
							dMC = dMA + dMB;
							dU0 = g_cBodyList[j]->cProps(1,h)/(cB-cA).norm();
							dU1 = 1. - g_cBodyList[j]->cProps(1,h+1)/(cB-cA).norm();
							
							dK = ( World::Instance()->dFDensity * 1.05 /2.0 );
							cF = (dK/pow((cB-cA).norm(),2)) * ( .33333333333*pow((cVB-cVA).dot(cN),2)*(pow(dU1,3)-pow(dU0,3)) + (cVB-cVA).dot(cN)*(pow(dU1,2)-pow(dU0,2)) + pow((cVA.dot(cN)),2)*(dU1-dU0) ) * cN;
							
							dKA = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h) ;
							dKB = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h+1);

							if( cVA.dot(cN) > 0) {
								g_cBodyList[j]->cVerts(6,h) += -0.5*cF(0);
								g_cBodyList[j]->cVerts(7,h) += -0.5*cF(1); 						
							}
							else{
								g_cBodyList[j]->cVerts(6,h) += 0.5*cF(0);
								g_cBodyList[j]->cVerts(7,h) += 0.5*cF(1); 
							}

							if(cVB.dot(cN) > 0) {
								g_cBodyList[j]->cVerts(6,h+1) += -0.5*cF(0);
								g_cBodyList[j]->cVerts(7,h+1) += -0.5*cF(1);
							}
							else {
								g_cBodyList[j]->cVerts(6,h+1) += 0.5*cF(0);
								g_cBodyList[j]->cVerts(7,h+1) += 0.5*cF(1);
							}
							
							g_cBodyList[j]->cVerts(6,h) += -dKA*cVA.norm()*cVA(0);
							g_cBodyList[j]->cVerts(7,h) += -dKA*cVA.norm()*cVA(1);

							g_cBodyList[j]->cVerts(6,h+1) += -dKB*cVB.norm()*cVB(0);
							g_cBodyList[j]->cVerts(7,h+1) += -dKB*cVB.norm()*cVB(1);
							*/
						}
						cDifVe << (g_cBodyList[j]->cVerts(0,int(g_cBodyList[j]->cVerts.cols()-1)) - g_cBodyList[j]->cVerts(0,0)), 
									(g_cBodyList[j]->cVerts(1,int(g_cBodyList[j]->cVerts.cols()-1)) - g_cBodyList[j]->cVerts(1,0));

						if(cDifVe.norm() != g_cBodyList[j]->cProps(3,int(g_cBodyList[j]->cVerts.cols()-1))) {
							cEqVe = g_cBodyList[j]->cProps(3,int(g_cBodyList[j]->cVerts.cols()-1))/(cDifVe.norm()) * cDifVe;
							cDisVe = cDifVe - cEqVe;
							cRelV << g_cBodyList[j]->cVerts(2,int(g_cBodyList[j]->cVerts.cols()-1)) - g_cBodyList[j]->cVerts(2,0), g_cBodyList[j]->cVerts(3,int(g_cBodyList[j]->cVerts.cols()-1)) - g_cBodyList[j]->cVerts(3,0);
							cRelV = (cRelV.dot(cDisVe)/pow(cDisVe.norm(),2))*cDisVe;

							g_cBodyList[j]->cVerts(6,int(g_cBodyList[j]->cVerts.cols()-1)) += -g_cBodyList[j]->cProps(2,int(g_cBodyList[j]->cVerts.cols()-1)) * cDisVe(0) - g_cBodyList[j]->cProps(4,int(g_cBodyList[j]->cVerts.cols()-1))*cRelV(0);
							g_cBodyList[j]->cVerts(6,0) += g_cBodyList[j]->cProps(2,int(g_cBodyList[j]->cVerts.cols()-1)) * cDisVe(0) + g_cBodyList[j]->cProps(4,int(g_cBodyList[j]->cVerts.cols()-1))*cRelV(0);

							g_cBodyList[j]->cVerts(7,int(g_cBodyList[j]->cVerts.cols()-1)) += -g_cBodyList[i]->cProps(2,int(g_cBodyList[j]->cVerts.cols()-1)) * cDisVe(1) - g_cBodyList[j]->cProps(4,int(g_cBodyList[j]->cVerts.cols()-1))*cRelV(1);
							g_cBodyList[j]->cVerts(7,0) += g_cBodyList[j]->cProps(2,int(g_cBodyList[j]->cVerts.cols()-1)) * cDisVe(1) + g_cBodyList[j]->cProps(4,int(g_cBodyList[j]->cVerts.cols()-1))*cRelV(1);
							
							//experimenting with drag force
							int h = int(g_cBodyList[j]->cVerts.cols()-1);
							int h_2 = 0;
							cA << g_cBodyList[j]->cVerts(0,h), g_cBodyList[j]->cVerts(1,h), 0;
							cB << g_cBodyList[j]->cVerts(0,h_2), g_cBodyList[j]->cVerts(1,h_2), 0;
							dU0 = g_cBodyList[j]->cProps(1,h)/(cB-cA).norm();
							dU1 = 1. - g_cBodyList[j]->cProps(1,h_2)/( (cB-cA).norm() );
							cVA << g_cBodyList[j]->cVerts(2,h), g_cBodyList[j]->cVerts(3,h), 0;
							cVB << g_cBodyList[j]->cVerts(2,h_2), g_cBodyList[j]->cVerts(3,h_2), 0;

							dK = ( World::Instance()->dFDensity * 1.05 /2.0 );
							dKA = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h) ;
							dKB = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h_2);

							double a2, a1, a0, BAdBA, BAdVBA, VBAdVA, BAdVA, det, c, chk1,chk2,chk3;
							BAdBA = (cB-cA).dot(cB-cA); BAdVBA = (cB-cA).dot(cVB-cVA); VBAdVA = (cVB-cVA).dot(cVA); BAdVA = (cB-cA).dot(cVA);
							a2 = BAdBA*( (cVB-cVA).dot(cVB-cVA) ) - pow(BAdVBA,2);
							a1 = 2*VBAdVA*BAdBA - 2*BAdVBA*BAdVA;
							a0 = BAdBA*cVA.dot(cVA) - BAdVA*BAdVA;
							det = a1*a1 - 4*a2*a0;
							c = a1/(2*a2);
							chk1 = 1./(384.*pow(a2,4));
							chk2 = 2*sqrt(a2*QuadEq(dU1, a2, a1, a0)) + 2*a2*dU1+a1;
							chk3 = 2*sqrt(a2*QuadEq(dU0, a2, a1, a0)) + 2*a2*dU0+a1;

							if( !( (cVA.norm() == 0) && (cVB.norm() == 0) ) ) {
								if( (det != 0) ) {
									if( (chk1 != 0) && (chk2 != 0) && (chk3 != 0) ) {
										cF = -dK*(DragFI1(dU1, a2, a1, a0) - DragFI1(dU0, a2, a1, a0))*(cVB - cVA) - dK*(DragFI0(dU1, a2, a1, a0) - DragFI0(dU0, a2, a1, a0))*cVA;
										cFB = -dK*(DragFI2(dU1, a2, a1, a0) - DragFI2(dU0, a2, a1, a0))*(cVB - cVA) - dK*(DragFI1(dU1, a2, a1, a0) - DragFI1(dU0, a2, a1, a0))*cVA;
										cFA = cF - cFB;
									}
									else { cFA = cFB = cF.setZero(); }
								}
								else {
									if( dU1 < -c ) {
										cF = -dK*( -sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*(cVB-cVA) - sqrt(a2)*( (pow(dU1,2) - pow(dU0,2))/2. + c*(dU1-dU0) )*cVA );
										cFB = -dK*( -sqrt(a2)*( (pow(dU1,4)-pow(dU0,4))/4. + (c/3)*(pow(dU1,3) - pow(dU0,3)) )*(cVB-cVA) - sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*cVA );
										cFA = cF - cFB;
									}
									else if( dU0 >= -c ) {
										cF = -dK*( sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*(cVB-cVA) + sqrt(a2)*( (pow(dU1,2) - pow(dU0,2))/2. + c*(dU1-dU0) )*cVA );
										cFB = -dK*( sqrt(a2)*( (pow(dU1,4)-pow(dU0,4))/4. + (c/3)*(pow(dU1,3) - pow(dU0,3)) )*(cVB-cVA) + sqrt(a2)*((pow(dU1,3) - pow(dU0,3))/3. + (c/2)*(pow(dU1,2) - pow(dU0,2)))*cVA );
										cFA = cF - cFB;
									}
									else {
										cF = -dK*( sqrt(a2)*((pow(dU1,3) - pow(-c,3))/3. + (c/2)*(pow(dU1,2) - pow(-c,2)))*(cVB-cVA) + sqrt(a2)*( (pow(dU1,2) - pow(-c,2))/2. + c*(dU1+c) )*cVA )
											+ dK*( sqrt(a2)*((pow(-c,3) - pow(dU0,3))/3. + (c/2)*(pow(-c,2) - pow(dU0,2)))*(cVB-cVA) + sqrt(a2)*( (pow(-c,2) - pow(dU0,2))/2. + c*(-c-dU0) )*cVA );
										cFB = -dK*( sqrt(a2)*( (pow(dU1,4)-pow(-c,4))/4. + (c/3)*(pow(dU1,3) - pow(-c,3)) )*(cVB-cVA) + sqrt(a2)*((pow(dU1,3) - pow(-c,3))/3. + (c/2)*(pow(dU1,2) - pow(-c,2)))*cVA )
											+ dK*( sqrt(a2)*( (pow(-c,4)-pow(dU0,4))/4. + (c/3)*(pow(-c,3) - pow(dU0,3)) )*(cVB-cVA) + sqrt(a2)*((pow(-c,3) - pow(dU0,3))/3. + (c/2)*(pow(-c,2) - pow(dU0,2)))*cVA );
										cFA = cF - cFB;
									}
								}
								if(  !(cFB(0) != cFB(0)) && !(cFB(1) != cFB(1)) && !(cFA(0) != cFA(0)) && !(cFA(1) != cFA(1)) ) {
									g_cBodyList[j]->cVerts(6,h) += cFA(0);
									g_cBodyList[j]->cVerts(7,h) += cFA(1);

									g_cBodyList[j]->cVerts(6,h_2) += cFB(0);
									g_cBodyList[j]->cVerts(7,h_2) += cFB(1);
								}
							}

							g_cBodyList[j]->cVerts(6,h) += -dKA*cVA.norm()*cVA(0);
							g_cBodyList[j]->cVerts(7,h) += -dKA*cVA.norm()*cVA(1);

							g_cBodyList[j]->cVerts(6,h_2) += -dKB*cVB.norm()*cVB(0);
							g_cBodyList[j]->cVerts(7,h_2) += -dKB*cVB.norm()*cVB(1);

							/*
							int h = int(g_cBodyList[j]->cVerts.cols()-1);
							int h_2 = 0;

							cZ << 0,0,1;
							cA << g_cBodyList[j]->cVerts(0,h), g_cBodyList[j]->cVerts(1,h), 0;
							cB << g_cBodyList[j]->cVerts(0,h_2), g_cBodyList[j]->cVerts(1,h_2), 0;
							cC = (0.5)*(cB + cA);
							cN = (cB-cA).cross(cZ);
							cVA << g_cBodyList[j]->cVerts(2,h), g_cBodyList[j]->cVerts(3,h), 0;
							cVB << g_cBodyList[j]->cVerts(2,h_2), g_cBodyList[j]->cVerts(3,h_2), 0;
							cVC << (0.5)*(cVB + cVA);
							dMA = g_cBodyList[j]->cProps(0,h);
							dMB = g_cBodyList[j]->cProps(0,h_2);
							dMC = dMA + dMB;
							dU0 = g_cBodyList[j]->cProps(1,h)/(cB-cA).norm();
							dU1 = 1. - g_cBodyList[j]->cProps(1,h_2)/( (cB-cA).norm() );
							dK = ( World::Instance()->dFDensity * 1.05 /2.0 );
							cF = (dK/pow((cB-cA).norm(),2)) * ( .33333333333*pow((cVB-cVA).dot(cN),2)*(pow(dU1,3)-pow(dU0,3)) + (cVB-cVA).dot(cN)*(pow(dU1,2)-pow(dU0,2)) + pow((cVA.dot(cN)),2)*(dU1-dU0) ) * cN;
							dKA = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h) ;
							dKB = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,h_2);

							cP << cN(0), cN(1);
							cP = 0.001*cP/cP.norm();
							bING = IsInNgon(g_cBodyList[j]->cVerts.row(0), g_cBodyList[j]->cVerts.row(1), cP);

							if( cVA.dot(cN) > 0) {
								if(bING) {
									g_cBodyList[j]->cVerts(6,h) += -0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(0);
									g_cBodyList[j]->cVerts(7,h) += -0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(1); 
								}
								else {
									g_cBodyList[j]->cVerts(6,h) += -0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(0);
									g_cBodyList[j]->cVerts(7,h) += -0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(1); 
								}

							}
							else{
								if(bING) {
									g_cBodyList[j]->cVerts(6,h) += 0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(0);
									g_cBodyList[j]->cVerts(7,h) += 0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(1); 
								}
								else{
									g_cBodyList[j]->cVerts(6,h) += 0.5*cF(0);
									g_cBodyList[j]->cVerts(7,h) += 0.5*cF(1); 
								}
							}

							if(cVB.dot(cN) > 0) {
								if(bING) {
									g_cBodyList[j]->cVerts(6,h_2) += -0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(0);
									g_cBodyList[j]->cVerts(7,h_2) += -0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(1);
								}
								else {
									g_cBodyList[j]->cVerts(6,h_2) += -0.5*cF(0);
									g_cBodyList[j]->cVerts(7,h_2) += -0.5*cF(1);
								}
							}
							else {
								if(bING) {
									g_cBodyList[j]->cVerts(6,h_2) += 0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(0);
									g_cBodyList[j]->cVerts(7,h_2) += 0.5*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cF(1);
								}
								else {
									g_cBodyList[j]->cVerts(6,h_2) += 0.5*cF(0);
									g_cBodyList[j]->cVerts(7,h_2) += 0.5*cF(1);
								}
							}

							cP << cVA(0), cVA(1);
							cP = 0.001*cP/cP.norm();
							bING = IsInNgon(g_cBodyList[j]->cVerts.row(0), g_cBodyList[j]->cVerts.row(1), cP);

							if(bING) {
								g_cBodyList[j]->cVerts(6,h) += -dKA*cVA.norm()*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cVA(0);
								g_cBodyList[j]->cVerts(7,h) += -dKA*cVA.norm()*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cVA(1);
							}
							else {
								g_cBodyList[j]->cVerts(6,h) += -dKA*cVA.norm()*cVA(0);
								g_cBodyList[j]->cVerts(7,h) += -dKA*cVA.norm()*cVA(1);
							}

							cP << cVB(0), cVB(1);
							cP = 0.001*cP/cP.norm();
							bING = IsInNgon(g_cBodyList[j]->cVerts.row(0), g_cBodyList[j]->cVerts.row(1), cP);

							if(bING) {
								g_cBodyList[j]->cVerts(6,h_2) += -dKB*cVB.norm()*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cVB(0);
								g_cBodyList[j]->cVerts(7,h_2) += -dKB*cVB.norm()*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cVB(1);
							}
							else {
								g_cBodyList[j]->cVerts(6,h_2) += -dKB*cVB.norm()*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cVB(0);
								g_cBodyList[j]->cVerts(7,h_2) += -dKB*cVB.norm()*(g_cBodyList[j]->dDen/World::Instance()->dFDensity)*cVB(1);
							}
							*/
						}
					}
					else if(int(g_cBodyList[j]->cVerts.cols()) == 1) {
						cVA << g_cBodyList[j]->cVerts(2,0), g_cBodyList[j]->cVerts(3,0), 0;
						dKA = (0.47)*World::Instance()->dFDensity * g_cBodyList[j]->cProps(1,0) ;
						g_cBodyList[j]->cVerts(6,0) += -dKA*cVA.norm()*cVA(0);
						g_cBodyList[j]->cVerts(7,0) += -dKA*cVA.norm()*cVA(1);
						
					}
				}
			}
		}
	}
	cCollCen = cCollOve = cCollShi.setZero();

}

