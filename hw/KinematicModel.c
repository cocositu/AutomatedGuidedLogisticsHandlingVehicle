#include "KinematicModel.h"
#include "arm_math.h"
#include "math.h"
#include "sys.h"
#include"stdio.h"
#include"string.h"
#include"CarTaskConfig.h"
double vel_weel[4];
double pos_weel[4];

#define PI                   (3.1415926535f)
#define a_PARAMETER          (22.0f  / (49.644921f))               
#define b_PARAMETER          (26.0f  / (49.644921f))   

// #define a_PARAMETER          (22.2f  / (48))               
// #define b_PARAMETER          (26.0f  / (48))   1222/1.5126
 
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


void Kinematic_Analysis_Pos(float Vx, float Vy, float Vw, float dx, float dy, float dw){
	
	vel_weel[0]  = +Vx+Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[1]  = +Vx-Vy-Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[2]  = +Vx-Vy+Vw*(a_PARAMETER+b_PARAMETER);
	vel_weel[3]  = +Vx+Vy+Vw*(a_PARAMETER+b_PARAMETER);

	pos_weel[0] = +dx+dy-dw*(a_PARAMETER+b_PARAMETER);
	pos_weel[1] = +dx-dy-dw*(a_PARAMETER+b_PARAMETER);
	pos_weel[2] = +dx-dy+dw*(a_PARAMETER+b_PARAMETER);
	pos_weel[3] = +dx+dy+dw*(a_PARAMETER+b_PARAMETER);
}