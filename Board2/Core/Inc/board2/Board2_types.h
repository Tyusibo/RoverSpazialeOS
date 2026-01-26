/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board2_types.h
 *
 * Code generated for Simulink model 'Board2'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Mon Jan 26 17:51:08 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Board2_types_h_
#define Board2_types_h_
#include "sensors.h"
#include "decision.h"
#include "rtwtypes.h"
#include "states.h"
#ifndef DEFINED_TYPEDEF_FOR_ENUM_Error_
#define DEFINED_TYPEDEF_FOR_ENUM_Error_

typedef enum {
  NO_ERROR = 0,                        /* Default value */
  ERROR_PRESENT
} ENUM_Error;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ENUM_BoardStatus_
#define DEFINED_TYPEDEF_FOR_ENUM_BoardStatus_

typedef enum {
  BOARD_OK = 0,                        /* Default value */
  BOARD_ERROR,
  BOARD_FAULT
} ENUM_BoardStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ENUM_MotorsStatus_
#define DEFINED_TYPEDEF_FOR_ENUM_MotorsStatus_

typedef enum {
  MOTORS_OK = 0,                       /* Default value */
  MOTORS_NOT_OK
} ENUM_MotorsStatus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ENUM_TemperatureStatus_
#define DEFINED_TYPEDEF_FOR_ENUM_TemperatureStatus_

typedef enum {
  TEMPERATURE_OK = 0,                  /* Default value */
  TEMPERATURE_NOT_OK
} ENUM_TemperatureStatus;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_Board2_T RT_MODEL_Board2_T;

#endif                                 /* Board2_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
