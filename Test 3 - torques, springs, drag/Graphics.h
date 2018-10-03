#ifndef GRAPHICS_H
#define GRAPHICS_H
//for custom graphics functions
#include "SDL.h"
#include <cmath>
#include <Eigen>

//all assume 32 bpp
void FillSurf(SDL_Surface* Targ, int Color = 0);

void DrawCircle(SDL_Surface* psTarg, double dR, double dX, double dY, int iC = 0);

//void DrawTriangle(SDL_Surface* psTarg, Eigen::Vector2d cV1, Eigen::Vector2d cV2, Eigen::Vector2d cV3, int iC = 0);

//void DrawLine(SDL_Surface* psTarg, const Eigen::Vector2d & cV1, const Eigen::Vector2d & cV2, int iC); 

void DrawLine(SDL_Surface* psTarg, double dX1, double dY1, double dX2, double dY2, int iC);

void DrawRectangle(SDL_Surface* psTarg, double dX, double dY, double dXLen, double dYLen, double dAng = 0, int iC = 0);

//void DrawNgon(SDL_Surface* psTarg, const Eigen::RowVectorXd& cVertX, const Eigen::RowVectorXd& cVertY, int iC, int iBlend = 0);

void DrawNgon(SDL_Surface* psTarg,const Eigen::RowVectorXd& cVertX, const Eigen::RowVectorXd& cVertY, int iC, int iB = 0);

unsigned int Blend_Avg(unsigned int uiSource, unsigned int uiTarget);

unsigned int blend_mul(unsigned int source, unsigned int target);

unsigned int blend_add(unsigned int source, unsigned int target);

double ZCP(const Eigen::Vector2d& cV0, const Eigen::Vector2d& cV1, const Eigen::Vector2d& cV2);

bool IsInNgon(const Eigen::RowVectorXd& cVertX, const Eigen::RowVectorXd& cVertY, const Eigen::Vector2d& cP);

#endif