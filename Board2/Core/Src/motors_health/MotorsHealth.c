/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MotorsHealth.c
 *
 * Code generated for Simulink model 'MotorsHealth'.
 *
 * Model version                  : 20.1
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Wed Jan 28 15:38:55 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. RAM efficiency
 *    2. Execution efficiency
 * Validation result: Not run
 */

#include "MotorsHealth.h"
#include "decision.h"
#include "sensors.h"
#include "rtwtypes.h"
#include <math.h>

/* Named constants for Chart: '<Root>/MotorsHealth' */
#define MotorsHealth_IN_KeepMonitoring ((uint8_T)1U)
#define MotorsHealth_IN_Motor_NOT_OK   ((uint8_T)2U)
#define MotorsHealth_IN_Motor_OK       ((uint8_T)3U)
#define MotorsHealth_SETTLINGS_SAMPLES ((uint8_T)60U)
#define MotorsHealth_TOLERANCE         (15.0F)

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         (*((rtm)->errorStatus))
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    (*((rtm)->errorStatus) = (val))
#endif

#ifndef rtmGetErrorStatusPointer
#define rtmGetErrorStatusPointer(rtm)  (rtm)->errorStatus
#endif

#ifndef rtmSetErrorStatusPointer
#define rtmSetErrorStatusPointer(rtm, val) ((rtm)->errorStatus = (val))
#endif

MdlrefDW_MotorsHealth_T MotorsHealth_MdlrefDW;

/* Block states (default storage) */
DW_MotorsHealth_f_T MotorsHealth_DW;

/* Forward declaration for local functions */
static ENUM_PidStatus MotorsHealth_checkSSReached(real32_T motorSpeed, real32_T
  motorSetPoint, real32_T tolerance);

/* Function for Chart: '<Root>/MotorsHealth' */
static ENUM_PidStatus MotorsHealth_checkSSReached(real32_T motorSpeed, real32_T
  motorSetPoint, real32_T tolerance)
{
  ENUM_PidStatus result;
  if (fabsf(motorSetPoint - motorSpeed) <= tolerance) {
    result = TARGET_REACHED;
  } else {
    result = ERROR_TARGET_NOT_REACHED;
  }

  return result;
}

/* System initialize for referenced model: 'MotorsHealth' */
void MotorsHealth_Init(void)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/UpdateMotorsHealth' incorporates:
   *  Chart: '<Root>/MotorsHealth'
   */
  /* Chart: '<Root>/MotorsHealth' */
  MotorsHealth_DW.bitsForTID1.is_LF_Motor = MotorsHealth_IN_Motor_OK;
  MotorsHealth_DW.LF_samples = 1U;
  MotorsHealth_DW.bitsForTID1.is_RF_Motor = MotorsHealth_IN_Motor_OK;
  MotorsHealth_DW.RF_samples = 1U;
  MotorsHealth_DW.bitsForTID1.is_RR_Motor = MotorsHealth_IN_Motor_OK;
  MotorsHealth_DW.RR_samples = 1U;
  MotorsHealth_DW.bitsForTID1.is_LR_Motor = MotorsHealth_IN_Motor_OK;
  MotorsHealth_DW.LR_samples = 1U;

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/UpdateMotorsHealth' */
}

/* Output and update for referenced model: 'MotorsHealth' */
void MotorsHealth_UpdateMotorsHealth(const BUS_SetPoint *rtu_setPoint, const
  BUS_Speed *rtu_speed, ENUM_MotorsStatus *rty_motorsHealth)
{
  /* RootInportFunctionCallGenerator generated from: '<Root>/UpdateMotorsHealth' incorporates:
   *  Chart: '<Root>/MotorsHealth'
   */
  /* Chart: '<Root>/MotorsHealth' */
  MotorsHealth_DW.setPoint_leftAxis_prev =
    MotorsHealth_DW.setPoint_leftAxis_start;
  MotorsHealth_DW.setPoint_leftAxis_start = rtu_setPoint->leftAxis;
  MotorsHealth_DW.setPoint_rightAxis_prev =
    MotorsHealth_DW.setPoint_rightAxis_start;
  MotorsHealth_DW.setPoint_rightAxis_start = rtu_setPoint->rightAxis;
  if (!MotorsHealth_DW.bitsForTID1.doneDoubleBufferReInit) {
    MotorsHealth_DW.bitsForTID1.doneDoubleBufferReInit = true;
    MotorsHealth_DW.setPoint_leftAxis_prev = rtu_setPoint->leftAxis;
    MotorsHealth_DW.setPoint_leftAxis_start = rtu_setPoint->leftAxis;
    MotorsHealth_DW.setPoint_rightAxis_prev = rtu_setPoint->rightAxis;
    MotorsHealth_DW.setPoint_rightAxis_start = rtu_setPoint->rightAxis;
  }

  switch (MotorsHealth_DW.bitsForTID1.is_LF_Motor) {
   case MotorsHealth_IN_KeepMonitoring:
    if ((MotorsHealth_DW.LF_resultCheck == ERROR_TARGET_NOT_REACHED) ||
        (MotorsHealth_DW.setPoint_leftAxis_prev !=
         MotorsHealth_DW.setPoint_leftAxis_start)) {
      /* In caso di disturbo, si da la possibilit� al PID  di ritornare a regime */
      MotorsHealth_DW.bitsForTID1.is_LF_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.LF_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.LF_samples = 1U;
    } else {
      MotorsHealth_DW.LF_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor1, rtu_setPoint->leftAxis, MotorsHealth_TOLERANCE);
    }
    break;

   case MotorsHealth_IN_Motor_NOT_OK:
    MotorsHealth_DW.LF_HealthMotor = MOTOR_NOT_OK;
    break;

   default:
    /* case IN_Motor_OK: */
    if (MotorsHealth_DW.setPoint_leftAxis_prev !=
        MotorsHealth_DW.setPoint_leftAxis_start) {
      MotorsHealth_DW.bitsForTID1.is_LF_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.LF_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.LF_samples = 1U;
    } else if (MotorsHealth_DW.LF_samples >= MotorsHealth_SETTLINGS_SAMPLES) {
      MotorsHealth_DW.LF_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor1, rtu_setPoint->leftAxis, MotorsHealth_TOLERANCE);
      if (MotorsHealth_DW.LF_resultCheck == TARGET_REACHED) {
        MotorsHealth_DW.bitsForTID1.is_LF_Motor = MotorsHealth_IN_KeepMonitoring;
        MotorsHealth_DW.LF_HealthMotor = MOTOR_OK;
      } else {
        MotorsHealth_DW.bitsForTID1.is_LF_Motor = MotorsHealth_IN_Motor_NOT_OK;
        MotorsHealth_DW.LF_HealthMotor = MOTOR_NOT_OK;
      }
    } else {
      MotorsHealth_DW.LF_samples++;
    }
    break;
  }

  switch (MotorsHealth_DW.bitsForTID1.is_RF_Motor) {
   case MotorsHealth_IN_KeepMonitoring:
    if ((MotorsHealth_DW.RF_resultCheck == ERROR_TARGET_NOT_REACHED) ||
        (MotorsHealth_DW.setPoint_rightAxis_prev !=
         MotorsHealth_DW.setPoint_rightAxis_start)) {
      /* In caso di disturbo, si da la possibilit� al PID  di ritornare a regime */
      MotorsHealth_DW.bitsForTID1.is_RF_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.RF_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.RF_samples = 1U;
    } else {
      MotorsHealth_DW.RF_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor2, rtu_setPoint->rightAxis, MotorsHealth_TOLERANCE);
    }
    break;

   case MotorsHealth_IN_Motor_NOT_OK:
    MotorsHealth_DW.RF_HealthMotor = MOTOR_NOT_OK;
    break;

   default:
    /* case IN_Motor_OK: */
    if (MotorsHealth_DW.setPoint_rightAxis_prev !=
        MotorsHealth_DW.setPoint_rightAxis_start) {
      MotorsHealth_DW.bitsForTID1.is_RF_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.RF_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.RF_samples = 1U;
    } else if (MotorsHealth_DW.RF_samples >= MotorsHealth_SETTLINGS_SAMPLES) {
      MotorsHealth_DW.RF_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor2, rtu_setPoint->rightAxis, MotorsHealth_TOLERANCE);
      if (MotorsHealth_DW.RF_resultCheck == TARGET_REACHED) {
        MotorsHealth_DW.bitsForTID1.is_RF_Motor = MotorsHealth_IN_KeepMonitoring;
        MotorsHealth_DW.RF_HealthMotor = MOTOR_OK;
      } else {
        MotorsHealth_DW.bitsForTID1.is_RF_Motor = MotorsHealth_IN_Motor_NOT_OK;
        MotorsHealth_DW.RF_HealthMotor = MOTOR_NOT_OK;
      }
    } else {
      MotorsHealth_DW.RF_samples++;
    }
    break;
  }

  switch (MotorsHealth_DW.bitsForTID1.is_RR_Motor) {
   case MotorsHealth_IN_KeepMonitoring:
    if ((MotorsHealth_DW.RR_resultCheck == ERROR_TARGET_NOT_REACHED) ||
        (MotorsHealth_DW.setPoint_rightAxis_prev !=
         MotorsHealth_DW.setPoint_rightAxis_start)) {
      /* In caso di disturbo, si da la possibilit� al PID  di ritornare a regime */
      MotorsHealth_DW.bitsForTID1.is_RR_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.RR_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.RR_samples = 1U;
    } else {
      MotorsHealth_DW.RR_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor3, rtu_setPoint->rightAxis, MotorsHealth_TOLERANCE);
    }
    break;

   case MotorsHealth_IN_Motor_NOT_OK:
    MotorsHealth_DW.RR_HealthMotor = MOTOR_NOT_OK;
    break;

   default:
    /* case IN_Motor_OK: */
    if (MotorsHealth_DW.setPoint_rightAxis_prev !=
        MotorsHealth_DW.setPoint_rightAxis_start) {
      MotorsHealth_DW.bitsForTID1.is_RR_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.RR_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.RR_samples = 1U;
    } else if (MotorsHealth_DW.RR_samples >= MotorsHealth_SETTLINGS_SAMPLES) {
      MotorsHealth_DW.RR_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor3, rtu_setPoint->rightAxis, MotorsHealth_TOLERANCE);
      if (MotorsHealth_DW.RR_resultCheck == TARGET_REACHED) {
        MotorsHealth_DW.bitsForTID1.is_RR_Motor = MotorsHealth_IN_KeepMonitoring;
        MotorsHealth_DW.RR_HealthMotor = MOTOR_OK;
      } else {
        MotorsHealth_DW.bitsForTID1.is_RR_Motor = MotorsHealth_IN_Motor_NOT_OK;
        MotorsHealth_DW.RR_HealthMotor = MOTOR_NOT_OK;
      }
    } else {
      MotorsHealth_DW.RR_samples++;
    }
    break;
  }

  switch (MotorsHealth_DW.bitsForTID1.is_LR_Motor) {
   case MotorsHealth_IN_KeepMonitoring:
    if ((MotorsHealth_DW.LR_resultCheck == ERROR_TARGET_NOT_REACHED) ||
        (MotorsHealth_DW.setPoint_leftAxis_prev !=
         MotorsHealth_DW.setPoint_leftAxis_start)) {
      /* In caso di disturbo, si da la possibilit� al PID  di ritornare a regime */
      MotorsHealth_DW.bitsForTID1.is_LR_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.LR_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.LR_samples = 1U;
    } else {
      MotorsHealth_DW.LR_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor4, rtu_setPoint->leftAxis, MotorsHealth_TOLERANCE);
    }
    break;

   case MotorsHealth_IN_Motor_NOT_OK:
    MotorsHealth_DW.LR_HealthMotor = MOTOR_NOT_OK;
    break;

   default:
    /* case IN_Motor_OK: */
    if (MotorsHealth_DW.setPoint_leftAxis_prev !=
        MotorsHealth_DW.setPoint_leftAxis_start) {
      MotorsHealth_DW.bitsForTID1.is_LR_Motor = MotorsHealth_IN_Motor_OK;
      MotorsHealth_DW.LR_HealthMotor = MOTOR_OK;
      MotorsHealth_DW.LR_samples = 1U;
    } else if (MotorsHealth_DW.LR_samples >= MotorsHealth_SETTLINGS_SAMPLES) {
      MotorsHealth_DW.LR_resultCheck = MotorsHealth_checkSSReached
        (rtu_speed->motor4, rtu_setPoint->leftAxis, MotorsHealth_TOLERANCE);
      if (MotorsHealth_DW.LR_resultCheck == TARGET_REACHED) {
        MotorsHealth_DW.bitsForTID1.is_LR_Motor = MotorsHealth_IN_KeepMonitoring;
        MotorsHealth_DW.LR_HealthMotor = MOTOR_OK;
      } else {
        MotorsHealth_DW.bitsForTID1.is_LR_Motor = MotorsHealth_IN_Motor_NOT_OK;
        MotorsHealth_DW.LR_HealthMotor = MOTOR_NOT_OK;
      }
    } else {
      MotorsHealth_DW.LR_samples++;
    }
    break;
  }

  /* End of Chart: '<Root>/MotorsHealth' */

  /* RootInportFunctionCallGenerator generated from: '<Root>/UpdateMotorsHealth' incorporates:
   *  SubSystem: '<Root>/MotorsHealthFunction'
   */
  /* MATLAB Function: '<S2>/MATLAB Function' */
  if ((MotorsHealth_DW.LF_HealthMotor == MOTOR_OK) &&
      (MotorsHealth_DW.LR_HealthMotor == MOTOR_OK) &&
      (MotorsHealth_DW.RF_HealthMotor == MOTOR_OK) &&
      (MotorsHealth_DW.RR_HealthMotor == MOTOR_OK)) {
    *rty_motorsHealth = MOTORS_OK;
  } else {
    *rty_motorsHealth = MOTORS_NOT_OK;
  }

  /* End of MATLAB Function: '<S2>/MATLAB Function' */
  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/UpdateMotorsHealth' */
}

/* Model initialize function */
void MotorsHealth_initialize(const char_T **rt_errorStatus)
{
  RT_MODEL_MotorsHealth_T *const MotorsHealth_M = &(MotorsHealth_MdlrefDW.rtm);

  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatusPointer(MotorsHealth_M, rt_errorStatus);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
