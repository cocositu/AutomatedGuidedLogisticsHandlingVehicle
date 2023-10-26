#ifndef _PID_H_
#define _PID_H_
#include"config.h"

// variable 变量
typedef float PID_ParType;
typedef double PID_ReVarType;
typedef double PID_InVarType;

struct _Structure_PIDCtrl{
    PID_ParType Par_KP;
    PID_ParType Par_KI;
    PID_ParType Par_KD;

    void (*setPar)(void*, PID_ParType, PID_ParType, PID_ParType); 
    //PID_ReVarType (*run)(void*, int, PID_InVarType, PID_InVarType);
};

struct _Structure_IncPIDCtrl{
    struct _Structure_PIDCtrl super;

    PID_InVarType Bias;
    PID_InVarType Last_bias;
    PID_InVarType Last_last_bias;

    // PID_InVarType Integ_Umax;
    // PID_InVarType Out_Umax;

    PID_ReVarType delta_CtrllVar;
    PID_ReVarType CtrllVar;

    PID_ReVarType (*run)(void*, PID_InVarType, PID_InVarType);
};

struct _Structure_PosPIDCtrl{
    struct _Structure_PIDCtrl super;
    
    PID_InVarType Error;
    PID_InVarType Last_Error;

    PID_InVarType Integral;
    PID_InVarType dError;

    // PID_InVarType Integ_Umax;
    // PID_InVarType Out_Umax;

    PID_ReVarType CtrllVar;

    PID_ReVarType (*run)(void*, PID_InVarType, PID_InVarType);
};

typedef struct _Structure_PIDCtrl     Struct_PID;
typedef struct _Structure_PIDCtrl*    pStruct_PID;
typedef struct _Structure_PIDCtrl**   ppStruct_PID;
typedef ppStruct_PID                  Arr_pStruct_PID;

typedef struct _Structure_IncPIDCtrl   Struct_Inc_PID;
typedef struct _Structure_IncPIDCtrl*  pStruct_Inc_PID;
typedef struct _Structure_IncPIDCtrl** ppStruct_Inc_PID;
typedef ppStruct_Inc_PID               Arr_pStruct_Inc_PID;

typedef struct _Structure_PosPIDCtrl   Struct_Pos_PID;
typedef struct _Structure_PosPIDCtrl*  pStruct_Pos_PID;
typedef struct _Structure_PosPIDCtrl** ppStruct_Pos_PID;
typedef ppStruct_Pos_PID               Arr_pStruct_Pos_PID;

void setPar_PID(void* pPID_slef, PID_ParType _iP, PID_ParType _iI, PID_ParType _iD);

Arr_pStruct_Inc_PID create_IncPIDStructure(int CtrlArr_n);
PID_ReVarType run_Incremental_PID(void* _pPID_slef, PID_InVarType Encoder, PID_InVarType Target);

Arr_pStruct_Pos_PID create_PosPIDStructure(int CtrlArr_n);
PID_ReVarType run_Positional_PID(void* _pPID_slef, PID_InVarType ActualValue, PID_InVarType TargetValue);

#endif //_PID_H_
