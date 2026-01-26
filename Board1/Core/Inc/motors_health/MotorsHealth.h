/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MotorsHealth.h
 *
 * Code generated for Simulink model 'MotorsHealth'.
 *
 * Model version                  : 18.11
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Mon Jan 26 15:42:50 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. RAM efficiency
 *    2. Execution efficiency
 * Validation result: Not run
 */

#ifndef MotorsHealth_h_
#define MotorsHealth_h_
#ifndef MotorsHealth_COMMON_INCLUDES_
#define MotorsHealth_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#endif                                 /* MotorsHealth_COMMON_INCLUDES_ */

#include "decision.h"
#include "sensors.h"

/* Forward declaration for rtModel */
typedef struct tag_RTM_MotorsHealth_T RT_MODEL_MotorsHealth_T;

/* user code (top of header file) */
#include "redLeds_functions.h"
#include "decision.h"
#ifndef DEFINED_TYPEDEF_FOR_ENUM_PidStatus_
#define DEFINED_TYPEDEF_FOR_ENUM_PidStatus_

typedef enum {
  TARGET_REACHED = 0,                  /* Default value */
  ERROR_TARGET_NOT_REACHED
} ENUM_PidStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ENUM_MotorStatus_
#define DEFINED_TYPEDEF_FOR_ENUM_MotorStatus_

typedef enum {
  OK = 0,                              /* Default value */
  NOT_OK
} ENUM_MotorStatus;

#endif

/* Block signals and states (default storage) for model 'MotorsHealth' */
typedef struct {
  real32_T setPoint_leftAxis_prev;     /* '<Root>/Motor_Handler' */
  real32_T setPoint_leftAxis_start;    /* '<Root>/Motor_Handler' */
  real32_T setPoint_rightAxis_prev;    /* '<Root>/Motor_Handler' */
  real32_T setPoint_rightAxis_start;   /* '<Root>/Motor_Handler' */
  ENUM_PidStatus LF_resultCheck;       /* '<Root>/Motor_Handler' */
  ENUM_PidStatus LR_resultCheck;       /* '<Root>/Motor_Handler' */
  ENUM_PidStatus RF_resultCheck;       /* '<Root>/Motor_Handler' */
  ENUM_PidStatus RR_resultCheck;       /* '<Root>/Motor_Handler' */
  ENUM_MotorStatus LF_HealthMotor;     /* '<Root>/Motor_Handler' */
  ENUM_MotorStatus LR_HealthMotor;     /* '<Root>/Motor_Handler' */
  ENUM_MotorStatus RF_HealthMotor;     /* '<Root>/Motor_Handler' */
  ENUM_MotorStatus RR_HealthMotor;     /* '<Root>/Motor_Handler' */
  struct {
    uint_T is_LF_Motor:2;              /* '<Root>/Motor_Handler' */
    uint_T is_RF_Motor:2;              /* '<Root>/Motor_Handler' */
    uint_T is_RR_Motor:2;              /* '<Root>/Motor_Handler' */
    uint_T is_LR_Motor:2;              /* '<Root>/Motor_Handler' */
    uint_T doneDoubleBufferReInit:1;   /* '<Root>/Motor_Handler' */
  } bitsForTID1;

  uint8_T LF_samples;                  /* '<Root>/Motor_Handler' */
  uint8_T LR_samples;                  /* '<Root>/Motor_Handler' */
  uint8_T RF_samples;                  /* '<Root>/Motor_Handler' */
  uint8_T RR_samples;                  /* '<Root>/Motor_Handler' */
} DW_MotorsHealth_f_T;

/* Real-time Model Data Structure */
struct tag_RTM_MotorsHealth_T {
  const char_T **errorStatus;
};

typedef struct {
  RT_MODEL_MotorsHealth_T rtm;
} MdlrefDW_MotorsHealth_T;

extern MdlrefDW_MotorsHealth_T MotorsHealth_MdlrefDW;

/* Block states (default storage) */
extern DW_MotorsHealth_f_T MotorsHealth_DW;
extern void MotorsHealth_Init(void);
extern void MotorsHealth_checkMotorHealth(const BUS_SetPoint *rtu_setPoint,
  const BUS_Speed *rtu_speed, ENUM_MotorStatus *rty_motorsHealth);

/* Model reference registration function */
extern void MotorsHealth_initialize(const char_T **rt_errorStatus);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'MotorsHealth'
 * '<S1>'   : 'MotorsHealth/Function-Call Subsystem'
 * '<S2>'   : 'MotorsHealth/Motor_Handler'
 * '<S3>'   : 'MotorsHealth/Function-Call Subsystem/MATLAB Function'
 */
#endif                                 /* MotorsHealth_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
