#include"pid.h"
#include"sys.h"
#include"stdlib.h"

// #define DEBUG_PRINTF(format, ...) do{printf(format,##__VA_ARGS__);}while (0)
#define _sign(_num) ((_num) < 0 ? -1 : 1)
#define _abs(_num) ((_num) < 0 ? -(_num) : (_num))

Arr_pStruct_Inc_PID create_IncPIDStructure(int CtrlArr_n){
    Arr_pStruct_Inc_PID Arr_pPID_slef = NULL;
    Arr_pPID_slef = (Arr_pStruct_Inc_PID)malloc(CtrlArr_n * sizeof(pStruct_Inc_PID));
    for(int i = 0; i < CtrlArr_n; i++){
        Arr_pPID_slef[i] = (pStruct_Inc_PID)malloc(sizeof(Struct_Inc_PID));
        ((pStruct_PID)Arr_pPID_slef[i])->Par_KP = 
        ((pStruct_PID)Arr_pPID_slef[i])->Par_KI = 
        ((pStruct_PID)Arr_pPID_slef[i])->Par_KD = 0;
        ((pStruct_PID)Arr_pPID_slef[i])->setPar = setPar_PID;
       
        Arr_pPID_slef[i]->Bias           = 
        Arr_pPID_slef[i]->Last_bias      =  
        Arr_pPID_slef[i]->Last_last_bias = 0;
        Arr_pPID_slef[i]->CtrllVar       = 0;
        Arr_pPID_slef[i]->run    = run_Incremental_PID;
    }
    return Arr_pPID_slef;
}

Arr_pStruct_Pos_PID create_PosPIDStructure(int CtrlArr_n){
    Arr_pStruct_Pos_PID Arr_pPID_slef;
    Arr_pPID_slef  =  (Arr_pStruct_Pos_PID)malloc(CtrlArr_n*sizeof(pStruct_Pos_PID));
    for(int i = 0; i < CtrlArr_n; i++){
        Arr_pPID_slef[i] = (pStruct_Pos_PID)malloc(sizeof(Struct_Pos_PID));
        ((pStruct_PID)Arr_pPID_slef[i])->Par_KP = 
        ((pStruct_PID)Arr_pPID_slef[i])->Par_KI = 
        ((pStruct_PID)Arr_pPID_slef[i])->Par_KD = 0;
        ((pStruct_PID)Arr_pPID_slef[i])->setPar = setPar_PID;
    
        Arr_pPID_slef [i]->Error    = 
        Arr_pPID_slef [i]->Integral = 
        Arr_pPID_slef [i]->dError   = 0; 
        Arr_pPID_slef [i]->CtrllVar = 0;
        Arr_pPID_slef [i]->run      = run_Positional_PID;
    }
    return Arr_pPID_slef ;
}

void setPar_PID(void* pPID_slef, PID_ParType _iP, PID_ParType _iI, PID_ParType _iD){ 
    ((pStruct_PID)pPID_slef)->Par_KP = _iP;
    ((pStruct_PID)pPID_slef)->Par_KI = _iI;
    ((pStruct_PID)pPID_slef)->Par_KD = _iD;
    
}


PID_ReVarType run_Incremental_PID(void* _pPID_slef, PID_InVarType ActualValue, PID_InVarType TargetValue){
    pStruct_Inc_PID pPID_slef = (pStruct_Inc_PID)_pPID_slef; 
	pPID_slef->Bias = TargetValue - ActualValue;                //计算偏差

    if(_abs(pPID_slef->Bias) < 2) 
        pPID_slef->CtrllVar += 0;

	// else if(_abs(Bias) > 30) 
    //     CtrllVar = _sign(Bias) * 900;

    else{
        
        PID_InVarType Integ_Var  = pPID_slef->Bias;
        PID_InVarType Delta_Var  = pPID_slef->Bias - pPID_slef->Last_bias;
        PID_InVarType Delta_dVar = (pPID_slef->Bias - pPID_slef->Last_bias) - 
                                   (pPID_slef->Last_bias - pPID_slef->Last_last_bias);

        pPID_slef->CtrllVar += 
        (PID_ReVarType)(
           ((pStruct_PID)pPID_slef)->Par_KP * Delta_Var +  //偏差越大速度越大
           ((pStruct_PID)pPID_slef)->Par_KI * Integ_Var +  //减小静态误差
           ((pStruct_PID)pPID_slef)->Par_KD * Delta_dVar   //限制速度
        );   //增量式PID控制器
    }

	if(_abs(pPID_slef->CtrllVar) >= 1000) 
        pPID_slef->CtrllVar = _sign(pPID_slef->CtrllVar) * 990;

	pPID_slef->Last_bias      = pPID_slef->Bias;	      //保存上一次偏差 
    pPID_slef->Last_last_bias = pPID_slef->Last_bias;     //保存上上一次偏差 
	return pPID_slef->CtrllVar;                           //增量输出
}


PID_ReVarType run_Positional_PID(void* _pPID_slef, PID_InVarType ActualValue, PID_InVarType TargetValue){
	pStruct_Pos_PID pPID_slef = (pStruct_Pos_PID) _pPID_slef;
    int index = 1;
    pPID_slef->Error  = TargetValue - ActualValue;                //计算偏差
    pPID_slef->dError = pPID_slef->Error - pPID_slef->Last_Error;

    // if(_abs(pPID_slef->Error) < 4) 
    //     return pPID_slef->CtrllVar = 0;

    //积分分离
    if(_abs(pPID_slef->Error) >= 65)
        index = 0;
    else
        index = 1,
        pPID_slef->Integral += pPID_slef->Error;
    
    //积分限幅
    if(_abs(pPID_slef->Integral) >= 500)
        pPID_slef->Integral = _sign(pPID_slef->Integral)*499;

    pPID_slef->CtrllVar = 
    (PID_ReVarType)(
        ((pStruct_PID)pPID_slef)->Par_KP * pPID_slef->Error            +  //偏差越大速度越大
        ((pStruct_PID)pPID_slef)->Par_KI * pPID_slef->Integral * index +  //保存上上一次偏差 
        ((pStruct_PID)pPID_slef)->Par_KD * pPID_slef->dError              //增量输出
    );   //位置式PID控制器

    //输出限幅
    if(_abs(pPID_slef->CtrllVar) >= 1000) 
        pPID_slef->CtrllVar = _sign(pPID_slef->CtrllVar) * 999;
    

    pPID_slef->Last_Error = pPID_slef->Error;
	return pPID_slef->CtrllVar;                    //输出
}