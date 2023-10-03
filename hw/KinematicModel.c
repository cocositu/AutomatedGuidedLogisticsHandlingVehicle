#include "KinematicModel.h"
#include "sys.h"
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
	vel_weel[0]  = +Vx+Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[1]  = +Vx-Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[2]  = +Vx-Vy+Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[3]  = +Vx+Vy+Vw*(a_PARAMETER+b_PARAMETER);
}


void Kinematic_Analysis_Pos(float x,
							float Vx,float Vy,float Vw)
{
	vel_weel[0]  = +Vx+Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[1]  = +Vx-Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[2]  = +Vx-Vy+Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[3]  = +Vx+Vy+Vw*(a_PARAMETER+b_PARAMETER);

	pos_weel[0] = x * _sign_f(vel_weel[0]);
	pos_weel[1] = x * _sign_f(vel_weel[1]);
	pos_weel[2] = x * _sign_f(vel_weel[2]);
	pos_weel[3] = x * _sign_f(vel_weel[3]);
}

