/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Boards_Health_private.h
 *
 * Code generated for Simulink model 'Boards_Health'.
 *
 * Model version                  : 6.21
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Sun Jan 25 18:42:11 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Boards_Health_private_h_
#define Boards_Health_private_h_
#include "rtwtypes.h"
#include "Boards_Health_types.h"

/* Block states (default storage) for model 'Boards_Health' */
typedef struct {
  uint8_T count_updateB1;              /* '<Root>/Boards_health' */
  uint8_T count_errorB1;               /* '<Root>/Boards_health' */
  uint8_T count_errorB2;               /* '<Root>/Boards_health' */
  uint8_T count_updateB2;              /* '<Root>/Boards_health' */
  uint8_T is_B1;                       /* '<Root>/Boards_health' */
  uint8_T is_B2;                       /* '<Root>/Boards_health' */
} DW_Boards_Health_f_T;

/* Real-time Model Data Structure */
struct tag_RTM_Boards_Health_T {
  const char_T **errorStatus;
};

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

typedef struct {
  RT_MODEL_Boards_Health_T rtm;
} MdlrefDW_Boards_Health_T;

extern MdlrefDW_Boards_Health_T Boards_Health_MdlrefDW;

/* Block states (default storage) */
extern DW_Boards_Health_f_T Boards_Health_DW;

#endif                                 /* Boards_Health_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
