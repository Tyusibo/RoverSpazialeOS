/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: BoardsHealth.h
 *
 * Code generated for Simulink model 'BoardsHealth'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Wed Jan 28 15:38:45 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef BoardsHealth_h_
#define BoardsHealth_h_
#ifndef BoardsHealth_COMMON_INCLUDES_
#define BoardsHealth_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#endif                                 /* BoardsHealth_COMMON_INCLUDES_ */

#include "BoardsHealth_types.h"

/* user code (top of header file) */
#include "controller_masks.h"
#include "serialize.h"
#include "deserialize.h"
#include "comparator.h"
#include "uart_functions.h"
#include "crc_functions.h"
#include "freeRTOS_functions.h"

extern void BoardsHealth_Init(void);
extern void BoardsHealth_UpdateBoardsStatus(const ENUM_Error *rtu_errorB1, const
  ENUM_Error *rtu_errorB2, ENUM_BoardStatus *rty_Status_Board1, ENUM_BoardStatus
  *rty_Status_Board2);

/* Model reference registration function */
extern void BoardsHealth_initialize(const char_T **rt_errorStatus);

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
 * '<Root>' : 'BoardsHealth'
 * '<S1>'   : 'BoardsHealth/BoardsHealth'
 */
#endif                                 /* BoardsHealth_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
