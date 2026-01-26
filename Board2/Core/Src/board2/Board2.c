/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board2.c
 *
 * Code generated for Simulink model 'Board2'.
 *
 * Model version                  : 6.39
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Mon Jan 26 15:56:59 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Board2.h"
#include "rtwtypes.h"
#include "sensors.h"
#include "states.h"
#include "Board2_types.h"
#include "decision.h"
#include <math.h>
#include "controller_masks.h"
#include "frame_size.h"
#include "crc_functions.h"
#include "ActionsModel.h"
#include "BoardsHealth.h"
#include "MotorsHealth.h"

/* Named constants for Chart: '<Root>/SupervisorB2' */
#define B_IN_StateCoherenceVerification ((uint8_T)4U)
#define Boar_IN_ComputingOwnGlobalState ((uint8_T)1U)
#define Boar_IN_EmergencyStateAnalysis1 ((uint8_T)1U)
#define Board2_IN_ArmingReceive        ((uint8_T)1U)
#define Board2_IN_ChangeActuatorToB2   ((uint8_T)1U)
#define Board2_IN_CheckCRC             ((uint8_T)2U)
#define Board2_IN_CommunicationPhase   ((uint8_T)1U)
#define Board2_IN_CompareDecision      ((uint8_T)1U)
#define Board2_IN_ComputeDecision      ((uint8_T)1U)
#define Board2_IN_D_Receive            ((uint8_T)2U)
#define Board2_IN_D_Transmit           ((uint8_T)3U)
#define Board2_IN_Degraded             ((uint8_T)2U)
#define Board2_IN_Emergency            ((uint8_T)3U)
#define Board2_IN_EmergencyCase        ((uint8_T)2U)
#define Board2_IN_EnterDegraded        ((uint8_T)3U)
#define Board2_IN_EnterDegraded_e      ((uint8_T)1U)
#define Board2_IN_ErrorStateDecision   ((uint8_T)2U)
#define Board2_IN_ExchangeDecision     ((uint8_T)3U)
#define Board2_IN_ExchangeGlobalState  ((uint8_T)4U)
#define Board2_IN_ExchangeLocalState   ((uint8_T)5U)
#define Board2_IN_Faulty               ((uint8_T)4U)
#define Board2_IN_GL_Receive           ((uint8_T)2U)
#define Board2_IN_GL_Transmit          ((uint8_T)3U)
#define Board2_IN_LS_Receive           ((uint8_T)1U)
#define Board2_IN_LS_Transmit          ((uint8_T)2U)
#define Board2_IN_LightEvaluation      ((uint8_T)2U)
#define Board2_IN_NO_ACTIVE_CHILD      ((uint8_T)0U)
#define Board2_IN_NotCommunicating     ((uint8_T)5U)
#define Board2_IN_R_ArmingReceive      ((uint8_T)4U)
#define Board2_IN_R_CheckCRC           ((uint8_T)5U)
#define Board2_IN_R_ReadyToReceive     ((uint8_T)6U)
#define Board2_IN_R_ReceivingRTR       ((uint8_T)2U)
#define Board2_IN_R_SendNack           ((uint8_T)7U)
#define Board2_IN_R_Trasmit            ((uint8_T)3U)
#define Board2_IN_R_WaitAck            ((uint8_T)4U)
#define Board2_IN_ReadyToReceive       ((uint8_T)8U)
#define Board2_IN_ReceivingRTR         ((uint8_T)5U)
#define Board2_IN_SendAck              ((uint8_T)9U)
#define Board2_IN_SendNack             ((uint8_T)10U)
#define Board2_IN_SupervisionEnded     ((uint8_T)4U)
#define Board2_IN_Trasmit              ((uint8_T)6U)
#define Board2_IN_UpdateLed            ((uint8_T)3U)
#define Board2_IN_UpdateRoverState     ((uint8_T)5U)
#define Board2_IN_UpdateRoverState_b   ((uint8_T)4U)
#define Board2_IN_UserActionComputation ((uint8_T)6U)
#define Board2_IN_WaitAck              ((uint8_T)7U)
#define Board_IN_RoverActionComputation ((uint8_T)3U)
#define IN_ValidateReceivedSensorReadin ((uint8_T)3U)

/* Block signals (default storage) */
B_Board2_T Board2_B;

/* Block states (default storage) */
DW_Board2_T Board2_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Board2_T Board2_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Board2_T Board2_Y;

/* Real-time model */
static RT_MODEL_Board2_T Board2_M_;
RT_MODEL_Board2_T *const Board2_M = &Board2_M_;

/* Forward declaration for local functions */
static ENUM_Error Board2_validateSensorReadings(uint8_T sensorReadings, real_T
  errorMask);
static void Board2_ExchangeLocalState(void);
static ENUM_StatusWhiteLed Board2_evaluateLed(uint16_T buttons, uint16_T
  lastButtons, ENUM_StatusWhiteLed previousLedStatus, uint16_T button_mask);
static void Board2_emergencyCheck(real32_T globalStateRover_localStateB1_s,
  real32_T globalStateRover_localStateB1_0, real32_T
  globalStateRover_localStateB1_1, real32_T globalStateRover_localStateB1_2,
  Temperature globalStateRover_localStateB1_t, BatteryLevel
  globalStateRover_localStateB1_b, uint16_T globalStateRover_localStateB2_s,
  uint16_T globalStateRover_localStateB2_0, uint16_T
  globalStateRover_localStateB2_1, Gyroscope globalStateRover_localStateB2_g,
  int16_T globalStateRover_localStateB2_r, int16_T
  globalStateRover_localStateB2_2, ENUM_Error *errB1, ENUM_Error *errB2);
static ENUM_StatusRover Board2_convertInENUMRoverStatus(ENUM_BoardStatus
  B1_status, ENUM_BoardStatus B2_status);
static ENUM_UserAction Board2_computeUserAction(int16_T x_lever, int16_T y_lever,
  uint16_T buttons, uint16_T braking_hard_mask, uint16_T braking_smooth_mask);
static void Board2_ExchangeDecision(void);
static void Board2_coherenceVerification(real32_T localStateSent_speed_motor1,
  real32_T localStateSent_speed_motor2, real32_T localStateSent_speed_motor3,
  real32_T localStateSent_speed_motor4, Temperature localStateSent_temperature,
  BatteryLevel localStateSent_batteryLevel, uint8_T
  localStateSent_sensorReadings, uint16_T localStateReceived_sonar_left,
  uint16_T localStateReceived_sonar_front, uint16_T
  localStateReceived_sonar_right, Gyroscope localStateReceived_gyroscope,
  int16_T localStateReceived_remoteContro, int16_T
  localStateReceived_remoteCont_0, uint16_T localStateReceived_remoteCont_1,
  uint8_T localStateReceived_sensorReadin, const BUS_LocalStateB1
  globalStateReceived_localStateB, const BUS_LocalStateB2
  globalStateReceived_localStat_0, ENUM_Error *errB1, ENUM_Error *errB2);
static void Board2_ExchangeGlobalState(void);

/* Function for Chart: '<Root>/SupervisorB2' */
static ENUM_Error Board2_validateSensorReadings(uint8_T sensorReadings, real_T
  errorMask)
{
  ENUM_Error resultSensorReadings;
  if ((sensorReadings & (uint8_T)errorMask) != 0) {
    resultSensorReadings = ERROR_PRESENT;
  } else {
    resultSensorReadings = NO_ERROR;
  }

  return resultSensorReadings;
}

/* Function for Chart: '<Root>/SupervisorB2' */
static void Board2_ExchangeLocalState(void)
{
  switch (Board2_DW.is_ExchangeLocalState) {
   case Board2_IN_LS_Receive:
    switch (Board2_DW.is_LS_Receive) {
     case Board2_IN_ArmingReceive:
      if (Board2_DW.receiveArmed == 1) {
        Board2_DW.is_LS_Receive = Board2_IN_ReadyToReceive;
        setRTR();
      }
      break;

     case Board2_IN_CheckCRC:
      switch (Board2_DW.flagCRC) {
       case 1:
        Board2_DW.is_LS_Receive = Board2_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board2_DW.is_LS_Receive = Board2_IN_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board2_IN_EnterDegraded:
      break;

     case Board2_IN_R_ArmingReceive:
      if (Board2_DW.receiveArmed == 1) {
        Board2_DW.is_LS_Receive = Board2_IN_R_ReadyToReceive;
        setRTR();
      }
      break;

     case Board2_IN_R_CheckCRC:
      switch (Board2_DW.flagCRC) {
       case 1:
        Board2_DW.is_LS_Receive = Board2_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board2_DW.is_LS_Receive = Board2_IN_R_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board2_IN_R_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board2_DW.is_LS_Receive = Board2_IN_R_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board2_DW.flagCRC = 0U;
        resetRTR();
        Board2_DW.is_LS_Receive = Board2_IN_R_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board2_DW.flagCRC = compareCRC(&Board2_U.rx_buffer[0],
          Board2_DW.rxPayload);
      }
      break;

     case Board2_IN_R_SendNack:
      Board2_DW.is_LS_Receive = Board2_IN_EnterDegraded;
      enterDegraded();
      break;

     case Board2_IN_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board2_DW.is_LS_Receive = Board2_IN_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board2_DW.flagCRC = 0U;
        resetRTR();
        Board2_DW.is_LS_Receive = Board2_IN_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board2_DW.flagCRC = compareCRC(&Board2_U.rx_buffer[0],
          Board2_DW.rxPayload);
      }
      break;

     case Board2_IN_SendAck:
      Board2_DW.is_LS_Receive = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_LS_Receive = 2U;
      break;

     default:
      /* case IN_SendNack: */
      Board2_DW.is_LS_Receive = Board2_IN_R_ArmingReceive;

      /* Inport: '<Root>/rx_buffer' */
      Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0], (uint64_T)
        (Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
      break;
    }

    if (Board2_DW.exit_port_index_LS_Receive == 2U) {
      Board2_DW.exit_port_index_LS_Receive = 0U;

      /* Inport: '<Root>/rx_buffer' */
      deserializeLocalStateB1(&Board2_U.rx_buffer[0], Board2_DW.rxPayload,
        &Board2_DW.board1LocalState);

      /* Outport: '<Root>/tx_buffer' */
      serializeLocalStateB2(&Board2_Y.tx_buffer[0], &Board2_DW.board2LocalState);
      computeCRC(&Board2_Y.tx_buffer[0], Board2_DW.txPayload);
      Board2_DW.is_ExchangeLocalState = Board2_IN_LS_Transmit;
      Board2_DW.is_LS_Transmit = Board2_IN_ReceivingRTR;
    }
    break;

   case Board2_IN_LS_Transmit:
    switch (Board2_DW.is_LS_Transmit) {
     case Board2_IN_EnterDegraded_e:
      break;

     case Board2_IN_R_ReceivingRTR:
      if (checkRTR() != 0) {
        Board2_DW.is_LS_Transmit = Board2_IN_R_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)(Board2_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board2_IN_R_Trasmit:
      Board2_DW.is_LS_Transmit = Board2_IN_R_WaitAck;
      break;

     case Board2_IN_R_WaitAck:
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 0) {
          Board2_DW.is_LS_Transmit = Board2_IN_EnterDegraded_e;
          enterDegraded();
        } else if (UART_CheckAck() == 1) {
          Board2_DW.is_LS_Transmit = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_LS_Transmit = 2U;
        }
      }
      break;

     case Board2_IN_ReceivingRTR:
      if (checkRTR() != 0) {
        Board2_DW.is_LS_Transmit = Board2_IN_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)(Board2_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board2_IN_Trasmit:
      Board2_DW.is_LS_Transmit = Board2_IN_WaitAck;
      break;

     default:
      /* case IN_WaitAck: */
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 1) {
          Board2_DW.is_LS_Transmit = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_LS_Transmit = 2U;
        } else if (UART_CheckAck() == 0) {
          Board2_DW.is_LS_Transmit = Board2_IN_R_ReceivingRTR;
        }
      }
      break;
    }

    if (Board2_DW.exit_port_index_LS_Transmit == 2U) {
      Board2_DW.exit_port_index_LS_Transmit = 0U;
      Board2_DW.is_ExchangeLocalState = IN_ValidateReceivedSensorReadin;
      Board2_B.errorB1 = Board2_validateSensorReadings
        (Board2_DW.board1LocalState.sensorReadings, 7.0);
      Board2_B.errorB2 = Board2_validateSensorReadings
        (Board2_DW.board2LocalState.sensorReadings, 7.0);
    }
    break;

   default:
    /* case IN_ValidateReceivedSensorReadings: */
    if ((Board2_B.errorB1 == NO_ERROR) && (Board2_B.errorB2 == NO_ERROR)) {
      Board2_DW.is_ExchangeLocalState = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeLocalSt = 3U;
    } else {
      Board2_DW.is_ExchangeLocalState = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeLocalSt = 2U;
    }
    break;
  }

  switch (Board2_DW.exit_port_index_ExchangeLocalSt) {
   case 2:
    Board2_DW.exit_port_index_ExchangeLocalSt = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_ErrorStateDecision;
    Board2_DW.is_ErrorStateDecision = Board2_IN_UpdateRoverState_b;

    /* ModelReference: '<Root>/BoardsHealth' */
    BoardsHealth_UpdateBoardsStatus(&Board2_B.errorB1, &Board2_B.errorB2,
      &Board2_B.Status_Board1, &Board2_B.Status_Board2);
    break;

   case 3:
    Board2_DW.exit_port_index_ExchangeLocalSt = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_ExchangeGlobalState;

    /*  Entry exchange global state */
    Board2_DW.txPayload = ((uint8_T)GLOBAL_STATE_FRAME_SIZE);
    Board2_DW.rxPayload = ((uint8_T)GLOBAL_STATE_FRAME_SIZE);
    Board2_DW.is_ExchangeGlobalState = Boar_IN_ComputingOwnGlobalState;

    /* Outport: '<Root>/board1GlobalState' */
    Board2_Y.board1GlobalState.localStateB1 = Board2_DW.board1LocalState;
    Board2_Y.board1GlobalState.localStateB2 = Board2_DW.board2LocalState;
    break;
  }
}

/* Function for Chart: '<Root>/SupervisorB2' */
static ENUM_StatusWhiteLed Board2_evaluateLed(uint16_T buttons, uint16_T
  lastButtons, ENUM_StatusWhiteLed previousLedStatus, uint16_T button_mask)
{
  ENUM_StatusWhiteLed nextLedStatus;

  /*  Carico il vecchio stato di default */
  nextLedStatus = previousLedStatus;

  /*  se è premuto */
  if (((buttons & button_mask) == button_mask) && ((lastButtons & button_mask)
       != button_mask)) {
    /*  prima non era stato premuto (rising edge) */
    if (previousLedStatus == WHITE_OFF) {
      nextLedStatus = WHITE_ON;
    } else {
      nextLedStatus = WHITE_OFF;
    }
  }

  return nextLedStatus;
}

/* Function for Chart: '<Root>/SupervisorB2' */
static void Board2_emergencyCheck(real32_T globalStateRover_localStateB1_s,
  real32_T globalStateRover_localStateB1_0, real32_T
  globalStateRover_localStateB1_1, real32_T globalStateRover_localStateB1_2,
  Temperature globalStateRover_localStateB1_t, BatteryLevel
  globalStateRover_localStateB1_b, uint16_T globalStateRover_localStateB2_s,
  uint16_T globalStateRover_localStateB2_0, uint16_T
  globalStateRover_localStateB2_1, Gyroscope globalStateRover_localStateB2_g,
  int16_T globalStateRover_localStateB2_r, int16_T
  globalStateRover_localStateB2_2, ENUM_Error *errB1, ENUM_Error *errB2)
{
  *errB1 = NO_ERROR;
  *errB2 = NO_ERROR;
  if (fabsf(globalStateRover_localStateB1_s) > 200.0F) {
    *errB1 = ERROR_PRESENT;
  } else if (fabsf(globalStateRover_localStateB1_0) > 200.0F) {
    *errB1 = ERROR_PRESENT;
  } else if (fabsf(globalStateRover_localStateB1_1) > 200.0F) {
    *errB1 = ERROR_PRESENT;
  } else if (fabsf(globalStateRover_localStateB1_2) > 200.0F) {
    *errB1 = ERROR_PRESENT;
  }

  if ((globalStateRover_localStateB1_t < 0.0F) ||
      (globalStateRover_localStateB1_t > 60.0F)) {
    *errB1 = ERROR_PRESENT;
  }

  if ((globalStateRover_localStateB1_b < 23) || (globalStateRover_localStateB1_b
       > 100)) {
    *errB1 = ERROR_PRESENT;
  }

  if ((globalStateRover_localStateB2_s > 400) ||
      (globalStateRover_localStateB2_0 > 400) ||
      (globalStateRover_localStateB2_1 > 400)) {
    *errB2 = ERROR_PRESENT;
  }

  if (globalStateRover_localStateB2_g > 360) {
    *errB2 = ERROR_PRESENT;
  }

  if ((globalStateRover_localStateB2_2 < -512) ||
      (globalStateRover_localStateB2_2 > 512) ||
      (globalStateRover_localStateB2_r < -512) ||
      (globalStateRover_localStateB2_r > 512)) {
    *errB2 = ERROR_PRESENT;
  }
}

/* Function for Chart: '<Root>/SupervisorB2' */
static ENUM_StatusRover Board2_convertInENUMRoverStatus(ENUM_BoardStatus
  B1_status, ENUM_BoardStatus B2_status)
{
  ENUM_StatusRover res;
  if ((B1_status == BOARD_FAULT) && (B2_status == BOARD_FAULT)) {
    res = EMERGENCY;
  } else if (B1_status == BOARD_FAULT) {
    res = FAULTY_B1_DEGRADED_B2;
  } else if (B2_status == BOARD_FAULT) {
    res = FAULTY_B2_DEGRADED_B1;
  } else if ((B1_status == BOARD_ERROR) && (B2_status == BOARD_ERROR)) {
    res = ERROR_BOTH;
  } else if (B1_status == BOARD_ERROR) {
    res = ERROR_B1;
  } else if (B2_status == BOARD_ERROR) {
    res = ERROR_B2;
  } else {
    res = NORMAL;
  }

  return res;
}

/* Function for Chart: '<Root>/SupervisorB2' */
static ENUM_UserAction Board2_computeUserAction(int16_T x_lever, int16_T y_lever,
  uint16_T buttons, uint16_T braking_hard_mask, uint16_T braking_smooth_mask)
{
  ENUM_UserAction userAction;
  userAction = UA_NONE;
  if ((buttons & braking_hard_mask) == braking_hard_mask) {
    userAction = UA_BRAKING_HARD;
  } else if ((buttons & braking_smooth_mask) == braking_smooth_mask) {
    userAction = UA_BRAKING_SMOOTH;
  } else if (x_lever < 0) {
    userAction = UA_ROTATE_LEFT;
  } else if (x_lever > 0) {
    userAction = UA_ROTATE_RIGHT;
  } else if (y_lever > 0) {
    userAction = UA_FORWARD;
  } else if (y_lever < 0) {
    userAction = UA_BACKWARD;
  }

  return userAction;
}

/* Function for Chart: '<Root>/SupervisorB2' */
static void Board2_ExchangeDecision(void)
{
  switch (Board2_DW.is_ExchangeDecision) {
   case Board2_IN_CompareDecision:
    /* [result == 1 || result == 0] */
    if (Board2_DW.result == 1) {
      Board2_DW.is_ExchangeDecision = Board2_IN_SupervisionEnded;

      /* Outport: '<Root>/supervision_ended' */
      Board2_Y.supervision_ended = 1U;

      /* ModelReference: '<Root>/MotorsHealth' incorporates:
       *  Outport: '<Root>/board1Decision'
       *  Outport: '<Root>/board1GlobalState'
       */
      /* printGlobalState(board1GlobalState); printGlobalState(board2GlobalState);
         printDecision(board1Decision); printDecision(board2Decision); */
      MotorsHealth_checkMotorHealth(&Board2_Y.board1Decision.setPoint,
        &Board2_Y.board1GlobalState.localStateB1.speed, &Board2_B.motorsHealth);
    }
    break;

   case Board2_IN_D_Receive:
    switch (Board2_DW.is_D_Receive) {
     case Board2_IN_ArmingReceive:
      if (Board2_DW.receiveArmed == 1) {
        Board2_DW.is_D_Receive = Board2_IN_ReadyToReceive;
        setRTR();
      }
      break;

     case Board2_IN_CheckCRC:
      switch (Board2_DW.flagCRC) {
       case 1:
        Board2_DW.is_D_Receive = Board2_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board2_DW.is_D_Receive = Board2_IN_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board2_IN_EnterDegraded:
      break;

     case Board2_IN_R_ArmingReceive:
      if (Board2_DW.receiveArmed == 1) {
        Board2_DW.is_D_Receive = Board2_IN_R_ReadyToReceive;
        setRTR();
      }
      break;

     case Board2_IN_R_CheckCRC:
      switch (Board2_DW.flagCRC) {
       case 1:
        Board2_DW.is_D_Receive = Board2_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board2_DW.is_D_Receive = Board2_IN_R_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board2_IN_R_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board2_DW.is_D_Receive = Board2_IN_R_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board2_DW.flagCRC = 0U;
        resetRTR();
        Board2_DW.is_D_Receive = Board2_IN_R_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board2_DW.flagCRC = compareCRC(&Board2_U.rx_buffer[0],
          Board2_DW.rxPayload);
      }
      break;

     case Board2_IN_R_SendNack:
      Board2_DW.is_D_Receive = Board2_IN_EnterDegraded;
      enterDegraded();
      break;

     case Board2_IN_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board2_DW.is_D_Receive = Board2_IN_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board2_DW.flagCRC = 0U;
        resetRTR();
        Board2_DW.is_D_Receive = Board2_IN_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board2_DW.flagCRC = compareCRC(&Board2_U.rx_buffer[0],
          Board2_DW.rxPayload);
      }
      break;

     case Board2_IN_SendAck:
      Board2_DW.is_D_Receive = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_D_Receive = 2U;
      break;

     default:
      /* case IN_SendNack: */
      Board2_DW.is_D_Receive = Board2_IN_R_ArmingReceive;

      /* Inport: '<Root>/rx_buffer' */
      Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0], (uint64_T)
        (Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
      break;
    }

    if (Board2_DW.exit_port_index_D_Receive == 2U) {
      Board2_DW.exit_port_index_D_Receive = 0U;

      /* Inport: '<Root>/rx_buffer' */
      deserializeDecision(&Board2_U.rx_buffer[0], Board2_DW.rxPayload,
                          &Board2_DW.board1Decision);

      /* Outport: '<Root>/tx_buffer' incorporates:
       *  Outport: '<Root>/board1Decision'
       */
      serializeDecision(&Board2_Y.tx_buffer[0], &Board2_Y.board1Decision);
      computeCRC(&Board2_Y.tx_buffer[0], Board2_DW.txPayload);
      Board2_DW.is_ExchangeDecision = Board2_IN_D_Transmit;
      Board2_DW.is_D_Transmit = Board2_IN_ReceivingRTR;
    }
    break;

   case Board2_IN_D_Transmit:
    switch (Board2_DW.is_D_Transmit) {
     case Board2_IN_EnterDegraded_e:
      break;

     case Board2_IN_R_ReceivingRTR:
      if (checkRTR() != 0) {
        Board2_DW.is_D_Transmit = Board2_IN_R_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)(Board2_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board2_IN_R_Trasmit:
      Board2_DW.is_D_Transmit = Board2_IN_R_WaitAck;
      break;

     case Board2_IN_R_WaitAck:
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 0) {
          Board2_DW.is_D_Transmit = Board2_IN_EnterDegraded_e;
          enterDegraded();
        } else if (UART_CheckAck() == 1) {
          Board2_DW.is_D_Transmit = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_D_Transmit = 2U;
        }
      }
      break;

     case Board2_IN_ReceivingRTR:
      if (checkRTR() != 0) {
        Board2_DW.is_D_Transmit = Board2_IN_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)(Board2_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board2_IN_Trasmit:
      Board2_DW.is_D_Transmit = Board2_IN_WaitAck;
      break;

     default:
      /* case IN_WaitAck: */
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 1) {
          Board2_DW.is_D_Transmit = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_D_Transmit = 2U;
        } else if (UART_CheckAck() == 0) {
          Board2_DW.is_D_Transmit = Board2_IN_R_ReceivingRTR;
        }
      }
      break;
    }

    if (Board2_DW.exit_port_index_D_Transmit == 2U) {
      Board2_DW.exit_port_index_D_Transmit = 0U;
      Board2_DW.is_ExchangeDecision = Board2_IN_CompareDecision;

      /* Outport: '<Root>/board1Decision' */
      Board2_DW.result = BUS_Decision_Equals(&Board2_DW.board1Decision,
        &Board2_Y.board1Decision);
    }
    break;

   default:
    /* Outport: '<Root>/supervision_ended' */
    /* case IN_SupervisionEnded: */
    Board2_Y.supervision_ended = 1U;
    switch (Board2_DW.board1Decision.roverState) {
     case EMERGENCY:
      Board2_DW.is_ExchangeDecision = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeDecisio = 3U;
      break;

     case FAULTY_B2_DEGRADED_B1:
      Board2_DW.is_ExchangeDecision = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeDecisio = 4U;
      break;

     case FAULTY_B1_DEGRADED_B2:
      Board2_DW.is_ExchangeDecision = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeDecisio = 2U;
      break;

     default:
      Board2_DW.is_ExchangeDecision = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeDecisio = 5U;
      break;
    }
    break;
  }

  switch (Board2_DW.exit_port_index_ExchangeDecisio) {
   case 2:
    Board2_DW.exit_port_index_ExchangeDecisio = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_NO_ACTIVE_CHILD;
    Board2_DW.exit_port_index_CommunicationPh = 2U;
    break;

   case 3:
    Board2_DW.exit_port_index_ExchangeDecisio = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_NO_ACTIVE_CHILD;
    Board2_DW.exit_port_index_CommunicationPh = 3U;
    break;

   case 4:
    Board2_DW.exit_port_index_ExchangeDecisio = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_NO_ACTIVE_CHILD;
    Board2_DW.exit_port_index_CommunicationPh = 4U;
    break;

   case 5:
    Board2_DW.exit_port_index_ExchangeDecisio = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_NO_ACTIVE_CHILD;
    Board2_DW.exit_port_index_CommunicationPh = 5U;
    break;
  }
}

/* Function for Chart: '<Root>/SupervisorB2' */
static void Board2_coherenceVerification(real32_T localStateSent_speed_motor1,
  real32_T localStateSent_speed_motor2, real32_T localStateSent_speed_motor3,
  real32_T localStateSent_speed_motor4, Temperature localStateSent_temperature,
  BatteryLevel localStateSent_batteryLevel, uint8_T
  localStateSent_sensorReadings, uint16_T localStateReceived_sonar_left,
  uint16_T localStateReceived_sonar_front, uint16_T
  localStateReceived_sonar_right, Gyroscope localStateReceived_gyroscope,
  int16_T localStateReceived_remoteContro, int16_T
  localStateReceived_remoteCont_0, uint16_T localStateReceived_remoteCont_1,
  uint8_T localStateReceived_sensorReadin, const BUS_LocalStateB1
  globalStateReceived_localStateB, const BUS_LocalStateB2
  globalStateReceived_localStat_0, ENUM_Error *errB1, ENUM_Error *errB2)
{
  boolean_T p;
  boolean_T s_p;
  *errB1 = NO_ERROR;
  *errB2 = NO_ERROR;
  p = false;
  if (localStateSent_sensorReadings ==
      globalStateReceived_localStateB.sensorReadings) {
    if (localStateSent_batteryLevel ==
        globalStateReceived_localStateB.batteryLevel) {
      if (localStateSent_temperature ==
          globalStateReceived_localStateB.temperature) {
        if (localStateSent_speed_motor4 ==
            globalStateReceived_localStateB.speed.motor4) {
          if (localStateSent_speed_motor3 ==
              globalStateReceived_localStateB.speed.motor3) {
            s_p = ((localStateSent_speed_motor2 ==
                    globalStateReceived_localStateB.speed.motor2) &&
                   (localStateSent_speed_motor1 ==
                    globalStateReceived_localStateB.speed.motor1));
          } else {
            s_p = false;
          }
        } else {
          s_p = false;
        }
      } else {
        s_p = false;
      }
    } else {
      s_p = false;
    }
  } else {
    s_p = false;
  }

  if (s_p) {
    p = true;
  }

  if (!p) {
    *errB1 = ERROR_PRESENT;
  }

  p = false;
  if (localStateReceived_sensorReadin ==
      globalStateReceived_localStat_0.sensorReadings) {
    if (localStateReceived_remoteCont_1 ==
        globalStateReceived_localStat_0.remoteController.buttons) {
      s_p = ((localStateReceived_remoteCont_0 ==
              globalStateReceived_localStat_0.remoteController.x_lever) &&
             (localStateReceived_remoteContro ==
              globalStateReceived_localStat_0.remoteController.y_lever));
    } else {
      s_p = false;
    }

    if (s_p) {
      if (localStateReceived_gyroscope ==
          globalStateReceived_localStat_0.gyroscope) {
        if (localStateReceived_sonar_right ==
            globalStateReceived_localStat_0.sonar.right) {
          s_p = ((localStateReceived_sonar_front ==
                  globalStateReceived_localStat_0.sonar.front) &&
                 (localStateReceived_sonar_left ==
                  globalStateReceived_localStat_0.sonar.left));
        } else {
          s_p = false;
        }
      } else {
        s_p = false;
      }
    } else {
      s_p = false;
    }
  } else {
    s_p = false;
  }

  if (s_p) {
    p = true;
  }

  if (!p) {
    *errB2 = ERROR_PRESENT;
  }
}

/* Function for Chart: '<Root>/SupervisorB2' */
static void Board2_ExchangeGlobalState(void)
{
  switch (Board2_DW.is_ExchangeGlobalState) {
   case Boar_IN_ComputingOwnGlobalState:
    Board2_DW.is_ExchangeGlobalState = Board2_IN_GL_Receive;
    Board2_DW.is_GL_Receive = Board2_IN_ArmingReceive;

    /* Inport: '<Root>/rx_buffer' */
    Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0], (uint64_T)
      (Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
    break;

   case Board2_IN_GL_Receive:
    switch (Board2_DW.is_GL_Receive) {
     case Board2_IN_ArmingReceive:
      if (Board2_DW.receiveArmed == 1) {
        Board2_DW.is_GL_Receive = Board2_IN_ReadyToReceive;
        setRTR();
      }
      break;

     case Board2_IN_CheckCRC:
      switch (Board2_DW.flagCRC) {
       case 1:
        Board2_DW.is_GL_Receive = Board2_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board2_DW.is_GL_Receive = Board2_IN_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board2_IN_EnterDegraded:
      break;

     case Board2_IN_R_ArmingReceive:
      if (Board2_DW.receiveArmed == 1) {
        Board2_DW.is_GL_Receive = Board2_IN_R_ReadyToReceive;
        setRTR();
      }
      break;

     case Board2_IN_R_CheckCRC:
      switch (Board2_DW.flagCRC) {
       case 1:
        Board2_DW.is_GL_Receive = Board2_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board2_DW.is_GL_Receive = Board2_IN_R_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board2_IN_R_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board2_DW.is_GL_Receive = Board2_IN_R_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board2_DW.flagCRC = 0U;
        resetRTR();
        Board2_DW.is_GL_Receive = Board2_IN_R_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board2_DW.flagCRC = compareCRC(&Board2_U.rx_buffer[0],
          Board2_DW.rxPayload);
      }
      break;

     case Board2_IN_R_SendNack:
      Board2_DW.is_GL_Receive = Board2_IN_EnterDegraded;
      enterDegraded();
      break;

     case Board2_IN_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board2_DW.is_GL_Receive = Board2_IN_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board2_DW.flagCRC = 0U;
        resetRTR();
        Board2_DW.is_GL_Receive = Board2_IN_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board2_DW.flagCRC = compareCRC(&Board2_U.rx_buffer[0],
          Board2_DW.rxPayload);
      }
      break;

     case Board2_IN_SendAck:
      Board2_DW.is_GL_Receive = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_GL_Receive = 2U;
      break;

     default:
      /* case IN_SendNack: */
      Board2_DW.is_GL_Receive = Board2_IN_R_ArmingReceive;

      /* Inport: '<Root>/rx_buffer' */
      Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0], (uint64_T)
        (Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
      break;
    }

    if (Board2_DW.exit_port_index_GL_Receive == 2U) {
      Board2_DW.exit_port_index_GL_Receive = 0U;

      /* Inport: '<Root>/rx_buffer' */
      deserializeGlobalState(&Board2_U.rx_buffer[0], Board2_DW.rxPayload,
        &Board2_DW.board1GlobalState);

      /* Outport: '<Root>/tx_buffer' incorporates:
       *  Outport: '<Root>/board1GlobalState'
       */
      serializeGlobalState(&Board2_Y.tx_buffer[0], &Board2_Y.board1GlobalState);
      computeCRC(&Board2_Y.tx_buffer[0], Board2_DW.txPayload);
      Board2_DW.is_ExchangeGlobalState = Board2_IN_GL_Transmit;
      Board2_DW.is_GL_Transmit = Board2_IN_ReceivingRTR;
    }
    break;

   default:
    /* case IN_GL_Transmit: */
    switch (Board2_DW.is_GL_Transmit) {
     case Board2_IN_EnterDegraded_e:
      break;

     case Board2_IN_R_ReceivingRTR:
      if (checkRTR() != 0) {
        Board2_DW.is_GL_Transmit = Board2_IN_R_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)(Board2_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board2_IN_R_Trasmit:
      Board2_DW.is_GL_Transmit = Board2_IN_R_WaitAck;
      break;

     case Board2_IN_R_WaitAck:
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 0) {
          Board2_DW.is_GL_Transmit = Board2_IN_EnterDegraded_e;
          enterDegraded();
        } else if (UART_CheckAck() == 1) {
          Board2_DW.is_GL_Transmit = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_GL_Transmit = 2U;
        }
      }
      break;

     case Board2_IN_ReceivingRTR:
      if (checkRTR() != 0) {
        Board2_DW.is_GL_Transmit = Board2_IN_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)(Board2_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board2_IN_Trasmit:
      Board2_DW.is_GL_Transmit = Board2_IN_WaitAck;
      break;

     default:
      /* case IN_WaitAck: */
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 1) {
          Board2_DW.is_GL_Transmit = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_GL_Transmit = 2U;
        } else if (UART_CheckAck() == 0) {
          Board2_DW.is_GL_Transmit = Board2_IN_R_ReceivingRTR;
        }
      }
      break;
    }

    if (Board2_DW.exit_port_index_GL_Transmit == 2U) {
      Board2_DW.exit_port_index_GL_Transmit = 0U;
      Board2_DW.is_ExchangeGlobalState = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeGlobalS = 2U;
    }
    break;
  }

  if (Board2_DW.exit_port_index_ExchangeGlobalS == 2U) {
    Board2_DW.exit_port_index_ExchangeGlobalS = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_ComputeDecision;

    /*  Entry compute decision */
    Board2_DW.is_ComputeDecision = B_IN_StateCoherenceVerification;

    /* Outport: '<Root>/board1GlobalState' */
    Board2_coherenceVerification(Board2_DW.board1LocalState.speed.motor1,
      Board2_DW.board1LocalState.speed.motor2,
      Board2_DW.board1LocalState.speed.motor3,
      Board2_DW.board1LocalState.speed.motor4,
      Board2_DW.board1LocalState.temperature,
      Board2_DW.board1LocalState.batteryLevel,
      Board2_DW.board1LocalState.sensorReadings,
      Board2_DW.board2LocalState.sonar.left,
      Board2_DW.board2LocalState.sonar.front,
      Board2_DW.board2LocalState.sonar.right,
      Board2_DW.board2LocalState.gyroscope,
      Board2_DW.board2LocalState.remoteController.y_lever,
      Board2_DW.board2LocalState.remoteController.x_lever,
      Board2_DW.board2LocalState.remoteController.buttons,
      Board2_DW.board2LocalState.sensorReadings,
      Board2_Y.board1GlobalState.localStateB1,
      Board2_Y.board1GlobalState.localStateB2, &Board2_B.errorB1,
      &Board2_B.errorB2);
  }
}

/* Model step function */
void Board2_step(void)
{
  /* Chart: '<Root>/SupervisorB2' incorporates:
   *  Inport: '<Root>/areSensorsValid'
   *  Inport: '<Root>/gyroscope'
   *  Inport: '<Root>/remoteController'
   *  Inport: '<Root>/rx_buffer'
   *  Inport: '<Root>/sonar'
   *  Outport: '<Root>/board1Decision'
   *  Outport: '<Root>/board1GlobalState'
   */
  if (Board2_DW.is_active_c3_Board2 == 0U) {
    Board2_DW.is_active_c3_Board2 = 1U;
    Board2_Y.board1Decision.actuator = BOARD1;
    Board2_B.previousUserAction = UA_NONE;
    Board2_DW.previousButtons = 0U;
    Board2_DW.previousWhiteLeftLed = WHITE_OFF;
    Board2_DW.previousWhiteRightLed = WHITE_OFF;
    Board2_DW.is_RoverState = Board2_IN_NotCommunicating;

    /* Outport: '<Root>/supervision_ended' incorporates:
     *  Outport: '<Root>/board1Decision'
     */
    Board2_Y.supervision_ended = 0U;
  } else {
    switch (Board2_DW.is_RoverState) {
     case Board2_IN_CommunicationPhase:
      switch (Board2_DW.is_CommunicationPhase) {
       case Board2_IN_ComputeDecision:
        switch (Board2_DW.is_ComputeDecision) {
         case Boar_IN_EmergencyStateAnalysis1:
          if ((Board2_B.errorB1 == NO_ERROR) && (Board2_B.errorB2 == NO_ERROR))
          {
            Board2_DW.is_ComputeDecision = Board2_IN_UpdateRoverState;

            /* ModelReference: '<Root>/BoardsHealth' */
            BoardsHealth_UpdateBoardsStatus(&Board2_B.errorB1, &Board2_B.errorB2,
              &Board2_B.Status_Board1, &Board2_B.Status_Board2);
          } else {
            Board2_DW.is_ComputeDecision = Board2_IN_NO_ACTIVE_CHILD;
            Board2_DW.exit_port_index_ComputeDecision = 2U;
          }
          break;

         case Board2_IN_LightEvaluation:
          Board2_DW.is_ComputeDecision = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_ComputeDecision = 3U;
          break;

         case Board_IN_RoverActionComputation:
          Board2_DW.is_ComputeDecision = Board2_IN_LightEvaluation;
          Board2_Y.board1Decision.leds.white.left = Board2_evaluateLed
            (Board2_Y.board1GlobalState.localStateB2.remoteController.buttons,
             Board2_DW.previousButtons, Board2_DW.previousWhiteLeftLed,
             ((uint16_T)WHITE_LEFT_LED_MASK));
          Board2_Y.board1Decision.leds.white.right = Board2_evaluateLed
            (Board2_Y.board1GlobalState.localStateB2.remoteController.buttons,
             Board2_DW.previousButtons, Board2_DW.previousWhiteRightLed,
             ((uint16_T)WHITE_RIGHT_LED_MASK));

          /*  Aggiornamento variabili di stato */
          Board2_DW.previousButtons =
            Board2_Y.board1GlobalState.localStateB2.remoteController.buttons;
          Board2_DW.previousWhiteLeftLed =
            Board2_Y.board1Decision.leds.white.left;
          Board2_DW.previousWhiteRightLed =
            Board2_Y.board1Decision.leds.white.right;
          break;

         case B_IN_StateCoherenceVerification:
          if ((Board2_B.errorB1 == NO_ERROR) && (Board2_B.errorB2 == NO_ERROR))
          {
            Board2_DW.is_ComputeDecision = Boar_IN_EmergencyStateAnalysis1;
            Board2_emergencyCheck
              (Board2_Y.board1GlobalState.localStateB1.speed.motor1,
               Board2_Y.board1GlobalState.localStateB1.speed.motor2,
               Board2_Y.board1GlobalState.localStateB1.speed.motor3,
               Board2_Y.board1GlobalState.localStateB1.speed.motor4,
               Board2_Y.board1GlobalState.localStateB1.temperature,
               Board2_Y.board1GlobalState.localStateB1.batteryLevel,
               Board2_Y.board1GlobalState.localStateB2.sonar.left,
               Board2_Y.board1GlobalState.localStateB2.sonar.front,
               Board2_Y.board1GlobalState.localStateB2.sonar.right,
               Board2_Y.board1GlobalState.localStateB2.gyroscope,
               Board2_Y.board1GlobalState.localStateB2.remoteController.y_lever,
               Board2_Y.board1GlobalState.localStateB2.remoteController.x_lever,
               &Board2_B.errorB1, &Board2_B.errorB2);
          } else {
            Board2_DW.is_ComputeDecision = Board2_IN_NO_ACTIVE_CHILD;
            Board2_DW.exit_port_index_ComputeDecision = 2U;
          }
          break;

         case Board2_IN_UpdateRoverState:
          Board2_Y.board1Decision.roverState = Board2_convertInENUMRoverStatus
            (Board2_B.Status_Board1, Board2_B.Status_Board2);
          Board2_DW.is_ComputeDecision = Board2_IN_UserActionComputation;
          Board2_Y.board1Decision.userAction = Board2_computeUserAction
            (Board2_Y.board1GlobalState.localStateB2.remoteController.x_lever,
             Board2_Y.board1GlobalState.localStateB2.remoteController.y_lever,
             Board2_Y.board1GlobalState.localStateB2.remoteController.buttons,
             ((uint16_T)BRAKING_HARD_MASK), ((uint16_T)BRAKING_SMOOTH_MASK));
          Board2_B.previousUserAction = Board2_Y.board1Decision.userAction;
          break;

         default:
          /* case IN_UserActionComputation: */
          Board2_DW.is_ComputeDecision = Board_IN_RoverActionComputation;

          /* ModelReference: '<Root>/ActionsModel' incorporates:
           *  Outport: '<Root>/board1GlobalState'
           */
          ActionsModel_ComputeRoverAction(&Board2_B.previousUserAction,
            &Board2_Y.board1GlobalState.localStateB1.speed,
            &Board2_Y.board1GlobalState.localStateB2.remoteController.x_lever,
            &Board2_Y.board1GlobalState.localStateB2.remoteController.y_lever,
            &Board2_Y.board1GlobalState.localStateB2.gyroscope,
            &Board2_Y.board1GlobalState.localStateB2.sonar, &Board2_B.setPoint,
            &Board2_B.roverAction, &Board2_B.safeAction, &Board2_B.redLeds);
          Board2_Y.board1Decision.roverAction = Board2_B.roverAction;
          Board2_Y.board1Decision.safeAction = Board2_B.safeAction;
          Board2_Y.board1Decision.setPoint = Board2_B.setPoint;

          /*  Light update */
          Board2_Y.board1Decision.leds.red = Board2_B.redLeds;
          break;
        }

        switch (Board2_DW.exit_port_index_ComputeDecision) {
         case 2:
          Board2_DW.exit_port_index_ComputeDecision = 0U;
          Board2_DW.is_CommunicationPhase = Board2_IN_ErrorStateDecision;
          Board2_DW.is_ErrorStateDecision = Board2_IN_UpdateRoverState_b;

          /* ModelReference: '<Root>/BoardsHealth' */
          BoardsHealth_UpdateBoardsStatus(&Board2_B.errorB1, &Board2_B.errorB2,
            &Board2_B.Status_Board1, &Board2_B.Status_Board2);
          break;

         case 3:
          Board2_DW.exit_port_index_ComputeDecision = 0U;
          Board2_DW.is_CommunicationPhase = Board2_IN_ExchangeDecision;

          /*  Entry exchange decision */
          Board2_DW.txPayload = ((uint8_T)DECISION_FRAME_SIZE);
          Board2_DW.rxPayload = ((uint8_T)DECISION_FRAME_SIZE);
          Board2_DW.is_ExchangeDecision = Board2_IN_D_Receive;
          Board2_DW.is_D_Receive = Board2_IN_ArmingReceive;
          Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0],
            (uint64_T)(Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
          break;
        }
        break;

       case Board2_IN_ErrorStateDecision:
        switch (Board2_DW.is_ErrorStateDecision) {
         case Board2_IN_ChangeActuatorToB2:
          Board2_DW.is_ErrorStateDecision = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_ErrorStateDecis = 2U;
          break;

         case Board2_IN_EmergencyCase:
          Board2_DW.is_ErrorStateDecision = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_ErrorStateDecis = 2U;
          break;

         case Board2_IN_UpdateLed:
          switch (Board2_Y.board1Decision.roverState) {
           case FAULTY_B1_DEGRADED_B2:
            Board2_DW.is_ErrorStateDecision = Board2_IN_ChangeActuatorToB2;
            Board2_Y.board1Decision.actuator = BOARD2;
            break;

           case EMERGENCY:
            Board2_DW.is_ErrorStateDecision = Board2_IN_EmergencyCase;

            /* CHI DEVE ATTUARE? */
            Board2_Y.board1Decision.safeAction = SA_BRAKING_HARD;
            Board2_Y.board1Decision.roverAction = RA_BRAKING_HARD;
            Board2_Y.board1Decision.userAction = UA_BRAKING_HARD;
            Board2_Y.board1Decision.setPoint.leftAxis = 0.0F;
            Board2_Y.board1Decision.setPoint.rightAxis = 0.0F;
            break;

           default:
            Board2_DW.is_ErrorStateDecision = Board2_IN_NO_ACTIVE_CHILD;
            Board2_DW.exit_port_index_ErrorStateDecis = 2U;
            break;
          }
          break;

         default:
          /* case IN_UpdateRoverState: */
          Board2_Y.board1Decision.roverState = Board2_convertInENUMRoverStatus
            (Board2_B.Status_Board1, Board2_B.Status_Board2);
          Board2_DW.is_ErrorStateDecision = Board2_IN_UpdateLed;
          Board2_Y.board1Decision.leds.red.left = RED_BLINKING;
          Board2_Y.board1Decision.leds.red.right =
            Board2_Y.board1Decision.leds.red.left;
          break;
        }

        if (Board2_DW.exit_port_index_ErrorStateDecis == 2U) {
          Board2_DW.exit_port_index_ErrorStateDecis = 0U;
          Board2_DW.is_CommunicationPhase = Board2_IN_ExchangeDecision;

          /*  Entry exchange decision */
          Board2_DW.txPayload = ((uint8_T)DECISION_FRAME_SIZE);
          Board2_DW.rxPayload = ((uint8_T)DECISION_FRAME_SIZE);
          Board2_DW.is_ExchangeDecision = Board2_IN_D_Receive;
          Board2_DW.is_D_Receive = Board2_IN_ArmingReceive;
          Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0],
            (uint64_T)(Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
        }
        break;

       case Board2_IN_ExchangeDecision:
        Board2_ExchangeDecision();
        break;

       case Board2_IN_ExchangeGlobalState:
        Board2_ExchangeGlobalState();
        break;

       default:
        /* case IN_ExchangeLocalState: */
        Board2_ExchangeLocalState();
        break;
      }

      switch (Board2_DW.exit_port_index_CommunicationPh) {
       case 2:
        Board2_DW.exit_port_index_CommunicationPh = 0U;
        Board2_DW.is_RoverState = Board2_IN_Degraded;
        enterDegraded();
        break;

       case 3:
        Board2_DW.exit_port_index_CommunicationPh = 0U;
        Board2_DW.is_RoverState = Board2_IN_Emergency;

        /* enterEmergency(); */
        break;

       case 4:
        Board2_DW.exit_port_index_CommunicationPh = 0U;
        Board2_DW.is_RoverState = Board2_IN_Faulty;

        /* enterFaulty(); */
        break;

       case 5:
        Board2_DW.exit_port_index_CommunicationPh = 0U;
        Board2_DW.is_RoverState = Board2_IN_NotCommunicating;

        /* Outport: '<Root>/supervision_ended' */
        Board2_Y.supervision_ended = 0U;
        break;
      }
      break;

     case Board2_IN_Degraded:
     case Board2_IN_Emergency:
     case Board2_IN_Faulty:
      break;

     default:
      /* Outport: '<Root>/supervision_ended' */
      /* case IN_NotCommunicating: */
      Board2_Y.supervision_ended = 0U;
      Board2_DW.board2LocalState.sonar = Board2_U.sonar;
      Board2_DW.board2LocalState.gyroscope = Board2_U.gyroscope;
      Board2_DW.board2LocalState.remoteController = Board2_U.remoteController;
      Board2_DW.board2LocalState.sensorReadings = Board2_U.areSensorsValid;
      Board2_DW.is_RoverState = Board2_IN_CommunicationPhase;
      Board2_DW.is_CommunicationPhase = Board2_IN_ExchangeLocalState;

      /*  Entry exchange local state */
      Board2_DW.txPayload = ((uint8_T)LOCAL_STATE_B2_FRAME_SIZE);
      Board2_DW.rxPayload = ((uint8_T)LOCAL_STATE_B1_FRAME_SIZE);
      Board2_DW.is_ExchangeLocalState = Board2_IN_LS_Receive;
      Board2_DW.is_LS_Receive = Board2_IN_ArmingReceive;
      Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0], (uint64_T)
        (Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
      break;
    }
  }

  /* End of Chart: '<Root>/SupervisorB2' */
}

/* Model initialize function */
void Board2_initialize(void)
{
  /* Model Initialize function for ModelReference Block: '<Root>/ActionsModel' */
  ActionsModel_initialize(rtmGetErrorStatusPointer(Board2_M));

  /* Model Initialize function for ModelReference Block: '<Root>/BoardsHealth' */
  BoardsHealth_initialize(rtmGetErrorStatusPointer(Board2_M));

  /* Model Initialize function for ModelReference Block: '<Root>/MotorsHealth' */
  MotorsHealth_initialize(rtmGetErrorStatusPointer(Board2_M));

  /* SystemInitialize for ModelReference: '<Root>/ActionsModel' incorporates:
   *  Outport: '<Root>/board1GlobalState'
   */
  ActionsModel_Init(&Board2_B.setPoint, &Board2_B.roverAction, &Board2_B.redLeds);

  /* SystemInitialize for ModelReference: '<Root>/BoardsHealth' */
  BoardsHealth_Init();

  /* SystemInitialize for ModelReference: '<Root>/MotorsHealth' incorporates:
   *  Outport: '<Root>/board1Decision'
   *  Outport: '<Root>/board1GlobalState'
   */
  MotorsHealth_Init();
}

/* Model terminate function */
void Board2_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
