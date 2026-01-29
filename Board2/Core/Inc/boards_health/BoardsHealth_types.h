/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: BoardsHealth_types.h
 *
 * Code generated for Simulink model 'BoardsHealth'.
 *
 * Model version                  : 7.4
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Thu Jan 29 12:25:20 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef BoardsHealth_types_h_
#define BoardsHealth_types_h_
#include "rtwtypes.h"
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

/* Forward declaration for rtModel */
typedef struct tag_RTM_BoardsHealth_T RT_MODEL_BoardsHealth_T;

#endif                                 /* BoardsHealth_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
