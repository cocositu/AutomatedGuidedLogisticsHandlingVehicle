#include "KinematicModel.h"
#include "arm_math.h"
#include "math.h"
#include "sys.h"
#include"stdio.h"
double vel_weel[4];
double pos_weel[4];

#define PI                   (3.14159f)
#define a_PARAMETER          (22.2f  / (6*PI))               
#define b_PARAMETER          (26.0f  / (6*PI))      
/**************************************************************************
函数功能：小车运动数学模型
入口参数：X Y Z 三轴速度或者位置
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float Vx,float Vy,float Vw)
{
	vel_weel[0]  = +Vx+Vy-Vw*(a_PARAMETER+b_PARAMETER);//LF
	vel_weel[1]  = +Vx-Vy-Vw*(a_PARAMETER+b_PARAMETER);//LR
	vel_weel[2]  = +Vx-Vy+Vw*(a_PARAMETER+b_PARAMETER);//RF
	vel_weel[3]  = +Vx+Vy+Vw*(a_PARAMETER+b_PARAMETER);//RR
}


void Kinematic_Analysis_Pos(float Vx, float Vy, float Vw, float x, float xw){
	double dx =0,dy =0,dw =0; 
	double tmp;
	tmp = sqrt(Vx*Vx + Vy*Vy);
	dx = x * Vx/tmp;
	dy = x * Vy/tmp;
	dw = xw *_sign_f(Vw);
	vel_weel[0]  = +Vx+Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[1]  = +Vx-Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[2]  = +Vx-Vy+Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[3]  = +Vx+Vy+Vw*(a_PARAMETER+b_PARAMETER);

	pos_weel[0] = +dx+dy-dw*(a_PARAMETER+b_PARAMETER);
	pos_weel[1] = +dx-dy-dw*(a_PARAMETER+b_PARAMETER);
	pos_weel[2] = +dx-dy+dw*(a_PARAMETER+b_PARAMETER);
	pos_weel[3] = +dx+dy+dw*(a_PARAMETER+b_PARAMETER);
}

