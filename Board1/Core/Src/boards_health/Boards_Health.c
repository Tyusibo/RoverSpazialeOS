/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Boards_Health.c
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

#include "Boards_Health.h"
#include "enums.h"
#include "rtwtypes.h"
#include "Boards_Health_private.h"

/* Named constants for Chart: '<Root>/Boards_health' */
#define Boards_Health_IN_Error         ((uint8_T)1U)
#define Boards_Health_IN_Fault         ((uint8_T)2U)
#define Boards_Health_IN_OK            ((uint8_T)3U)

MdlrefDW_Boards_Health_T Boards_Health_MdlrefDW;

/* Block states (default storage) */
DW_Boards_Health_f_T Boards_Health_DW;

/* System initialize for referenced model: 'Boards_Health' */
void Boards_Health_Init(void)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/Update_boards_status' incorporates:
   *  Chart: '<Root>/Boards_health'
   */
  /* Chart: '<Root>/Boards_health' */
  Boards_Health_DW.is_B1 = Boards_Health_IN_OK;
  Boards_Health_DW.is_B2 = Boards_Health_IN_OK;

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/Update_boards_status' */
}

/* Output and update for referenced model: 'Boards_Health' */
void Boards_Hea_Update_boards_status(const ENUM_Error *rtu_errorB1, const
  ENUM_Error *rtu_errorB2, ENUM_BoardStatus *rty_Status_Board1, ENUM_BoardStatus
  *rty_Status_Board2)
{
  boolean_T guard1;

  /* RootInportFunctionCallGenerator generated from: '<Root>/Update_boards_status' incorporates:
   *  Chart: '<Root>/Boards_health'
   */
  /* Chart: '<Root>/Boards_health' */
  Boards_Health_DW.count_updateB1++;
  Boards_Health_DW.count_updateB2++;
  guard1 = false;
  switch (Boards_Health_DW.is_B1) {
   case Boards_Health_IN_Error:
    *rty_Status_Board1 = BOARD_ERROR;
    if (Boards_Health_DW.count_updateB1 == 5) {
      Boards_Health_DW.count_updateB1 = 0U;
      if (Boards_Health_DW.count_errorB1 >= 2) {
        guard1 = true;
      } else {
        Boards_Health_DW.count_errorB1 = 0U;
        Boards_Health_DW.is_B1 = Boards_Health_IN_Error;
        *rty_Status_Board1 = BOARD_ERROR;
      }
    } else if (*rtu_errorB1 == ERROR_PRESENT) {
      Boards_Health_DW.count_errorB1++;
      guard1 = true;
    } else {
      Boards_Health_DW.is_B1 = Boards_Health_IN_OK;
      *rty_Status_Board1 = BOARD_OK;
    }
    break;

   case Boards_Health_IN_Fault:
    *rty_Status_Board1 = BOARD_FAULT;
    break;

   default:
    /* case IN_OK: */
    *rty_Status_Board1 = BOARD_OK;
    if (*rtu_errorB1 == ERROR_PRESENT) {
      Boards_Health_DW.count_errorB1++;
      Boards_Health_DW.is_B1 = Boards_Health_IN_Error;
      *rty_Status_Board1 = BOARD_ERROR;
    } else if (Boards_Health_DW.count_updateB1 == 5) {
      Boards_Health_DW.count_updateB1 = 0U;
      if (Boards_Health_DW.count_errorB1 >= 2) {
        Boards_Health_DW.is_B1 = Boards_Health_IN_Fault;
        *rty_Status_Board1 = BOARD_FAULT;
      } else {
        Boards_Health_DW.count_errorB1 = 0U;
        Boards_Health_DW.is_B1 = Boards_Health_IN_OK;
        *rty_Status_Board1 = BOARD_OK;
      }
    }
    break;
  }

  if (guard1) {
    Boards_Health_DW.is_B1 = Boards_Health_IN_Fault;
    *rty_Status_Board1 = BOARD_FAULT;
  }

  guard1 = false;
  switch (Boards_Health_DW.is_B2) {
   case Boards_Health_IN_Error:
    *rty_Status_Board2 = BOARD_ERROR;
    if (Boards_Health_DW.count_updateB2 == 5) {
      Boards_Health_DW.count_updateB2 = 0U;
      if (Boards_Health_DW.count_errorB2 >= 2) {
        guard1 = true;
      } else {
        Boards_Health_DW.count_errorB2 = 0U;
        Boards_Health_DW.is_B2 = Boards_Health_IN_Error;
        *rty_Status_Board2 = BOARD_ERROR;
      }
    } else if (*rtu_errorB2 == ERROR_PRESENT) {
      Boards_Health_DW.count_errorB2++;
      guard1 = true;
    } else {
      Boards_Health_DW.is_B2 = Boards_Health_IN_OK;
      *rty_Status_Board2 = BOARD_OK;
    }
    break;

   case Boards_Health_IN_Fault:
    *rty_Status_Board2 = BOARD_FAULT;
    break;

   default:
    /* case IN_OK: */
    *rty_Status_Board2 = BOARD_OK;
    if (*rtu_errorB2 == ERROR_PRESENT) {
      Boards_Health_DW.count_errorB2++;
      Boards_Health_DW.is_B2 = Boards_Health_IN_Error;
      *rty_Status_Board2 = BOARD_ERROR;
    } else if (Boards_Health_DW.count_updateB2 == 5) {
      Boards_Health_DW.count_updateB2 = 0U;
      if (Boards_Health_DW.count_errorB2 >= 2) {
        Boards_Health_DW.is_B2 = Boards_Health_IN_Fault;
        *rty_Status_Board2 = BOARD_FAULT;
      } else {
        Boards_Health_DW.count_errorB2 = 0U;
        Boards_Health_DW.is_B2 = Boards_Health_IN_OK;
        *rty_Status_Board2 = BOARD_OK;
      }
    }
    break;
  }

  if (guard1) {
    Boards_Health_DW.is_B2 = Boards_Health_IN_Fault;
    *rty_Status_Board2 = BOARD_FAULT;
  }

  /* End of Chart: '<Root>/Boards_health' */
  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/Update_boards_status' */
}

/* Model initialize function */
void Boards_Health_initialize(const char_T **rt_errorStatus)
{
  RT_MODEL_Boards_Health_T *const Boards_Health_M = &(Boards_Health_MdlrefDW.rtm);

  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatusPointer(Boards_Health_M, rt_errorStatus);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
