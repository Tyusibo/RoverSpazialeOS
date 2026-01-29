/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board2_types.h
 *
 * Code generated for Simulink model 'Board2'.
 *
 * Model version                  : 8.32
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Thu Jan 29 18:00:25 2026
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

#ifndef DEFINED_TYPEDEF_FOR_ENUM_CycleResult_
#define DEFINED_TYPEDEF_FOR_ENUM_CycleResult_

typedef enum {
  CYCLE_OK_CLEAN = 0,                  /* Default value */
  CYCLE_OK_DIRTY,
  CYCLE_FAIL
} ENUM_CycleResult;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ENUM_LinkStatus_
#define DEFINED_TYPEDEF_FOR_ENUM_LinkStatus_

typedef enum {
  LINK_OK = 0,                         /* Default value */
  LINK_DEGRADED,
  LINK_LOST
} ENUM_LinkStatus;

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
