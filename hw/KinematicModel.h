#ifndef _KINEMATICMODEL_H_
#define _KINEMATICMODEL_H_
#include"config.h"

extern double vel_weel[4];
extern double pos_weel[4];
void Kinematic_Analysis(float Vx,float Vy,float Vw);
void Kinematic_Analysis_Pos(float x,float Vx,float Vy,float Vw);

#endif //_KINEMATICMODEL_H_