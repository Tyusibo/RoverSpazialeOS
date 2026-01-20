/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board1.c
 *
 * Code generated for Simulink model 'Board1'.
 *
 * Model version                  : 4.75
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Tue Jan 20 22:09:20 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Board1.h"
#include "enums.h"
#include "rtwtypes.h"
#include "controller_masks.h"
#include "crc_functions.h"
#include "frame_size.h"
#include "ActionsModel.h"

/* Named constants for Chart: '<Root>/SupervisorB1' */
#define B_IN_StateCoherenceVerification ((uint8_T)5U)
#define Boar_IN_ComputingOwnGlobalState ((uint8_T)1U)
#define Board1_IN_ActuatorSelection    ((uint8_T)1U)
#define Board1_IN_ArmingReceive        ((uint8_T)1U)
#define Board1_IN_CheckCRC             ((uint8_T)2U)
#define Board1_IN_CommunicationPhase   ((uint8_T)1U)
#define Board1_IN_ComputeDecision      ((uint8_T)1U)
#define Board1_IN_D_Receive            ((uint8_T)1U)
#define Board1_IN_D_Transmit           ((uint8_T)2U)
#define Board1_IN_EnterDegraded        ((uint8_T)3U)
#define Board1_IN_EnterDegraded_e      ((uint8_T)1U)
#define Board1_IN_ExchangeDecision     ((uint8_T)2U)
#define Board1_IN_ExchangeGlobalState  ((uint8_T)3U)
#define Board1_IN_ExchangeLocalState   ((uint8_T)4U)
#define Board1_IN_Execution            ((uint8_T)3U)
#define Board1_IN_GL_Receive           ((uint8_T)2U)
#define Board1_IN_GL_Transmit          ((uint8_T)3U)
#define Board1_IN_LS_Receive           ((uint8_T)1U)
#define Board1_IN_LS_Transmit          ((uint8_T)2U)
#define Board1_IN_LightEvaluation      ((uint8_T)3U)
#define Board1_IN_NO_ACTIVE_CHILD      ((uint8_T)0U)
#define Board1_IN_NotCommunicating     ((uint8_T)2U)
#define Board1_IN_R_ArmingReceive      ((uint8_T)4U)
#define Board1_IN_R_CheckCRC           ((uint8_T)5U)
#define Board1_IN_R_ReadyToReceive     ((uint8_T)6U)
#define Board1_IN_R_ReceivingRTR       ((uint8_T)2U)
#define Board1_IN_R_SendNack           ((uint8_T)7U)
#define Board1_IN_R_Trasmit            ((uint8_T)3U)
#define Board1_IN_R_WaitAck            ((uint8_T)4U)
#define Board1_IN_ReadyToReceive       ((uint8_T)8U)
#define Board1_IN_ReceivingRTR         ((uint8_T)5U)
#define Board1_IN_SendAck              ((uint8_T)9U)
#define Board1_IN_SendNack             ((uint8_T)10U)
#define Board1_IN_Trasmit              ((uint8_T)6U)
#define Board1_IN_UserActionComputation ((uint8_T)6U)
#define Board1_IN_WaitAck              ((uint8_T)7U)
#define Board1_TRESHOLD_ACTUATOR_SWITCH (1000U)
#define Board_IN_EmergencyStateAnalysis ((uint8_T)2U)
#define Board_IN_RoverActionComputation ((uint8_T)4U)

/* Block signals (default storage) */
B_Board1_T Board1_B;

/* Block states (default storage) */
DW_Board1_T Board1_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Board1_T Board1_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Board1_T Board1_Y;

/* Real-time model */
static RT_MODEL_Board1_T Board1_M_;
RT_MODEL_Board1_T *const Board1_M = &Board1_M_;

/* Forward declaration for local functions */
static void Board1_ExchangeLocalState(void);
static ENUM_UserAction Board1_computeUserAction(int16_T x_lever, int16_T y_lever,
  uint16_T buttons, uint16_T braking_hard_mask, uint16_T braking_smooth_mask);
static ENUM_UserAction Board1_continueBraking(ENUM_UserAction currentUserAction,
  ENUM_UserAction lastUserAction);
static void Board1_actuatorSelection(ENUM_Actuator lastActuator, uint32_T
  lastCounter, uint32_T treshold, ENUM_Actuator *nextActuator, uint32_T
  *nextCounter);
static ENUM_StatusWhiteLed Board1_evaluateLed(uint16_T buttons, uint16_T
  lastButtons, ENUM_StatusWhiteLed previousLedStatus, uint16_T button_mask);
static void Board1_ExchangeGlobalState(void);

/* Function for Chart: '<Root>/SupervisorB1' */
static void Board1_ExchangeLocalState(void)
{
  if (Board1_DW.is_ExchangeLocalState == Board1_IN_LS_Receive) {
    switch (Board1_DW.is_LS_Receive) {
     case Board1_IN_ArmingReceive:
      if (Board1_DW.receiveArmed == 1) {
        Board1_DW.is_LS_Receive = Board1_IN_ReadyToReceive;
        setRTR();
      }
      break;

     case Board1_IN_CheckCRC:
      switch (Board1_DW.flagCRC) {
       case 1:
        Board1_DW.is_LS_Receive = Board1_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board1_DW.is_LS_Receive = Board1_IN_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board1_IN_EnterDegraded:
      break;

     case Board1_IN_R_ArmingReceive:
      if (Board1_DW.receiveArmed == 1) {
        Board1_DW.is_LS_Receive = Board1_IN_R_ReadyToReceive;
        setRTR();
      }
      break;

     case Board1_IN_R_CheckCRC:
      switch (Board1_DW.flagCRC) {
       case 1:
        Board1_DW.is_LS_Receive = Board1_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board1_DW.is_LS_Receive = Board1_IN_R_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board1_IN_R_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board1_DW.is_LS_Receive = Board1_IN_R_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board1_DW.flagCRC = 0U;
        resetRTR();
        Board1_DW.is_LS_Receive = Board1_IN_R_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board1_DW.flagCRC = compareCRC(&Board1_U.rx_buffer[0],
          Board1_DW.rxPayload);
      }
      break;

     case Board1_IN_R_SendNack:
      Board1_DW.is_LS_Receive = Board1_IN_EnterDegraded;
      enterDegraded();
      break;

     case Board1_IN_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board1_DW.is_LS_Receive = Board1_IN_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board1_DW.flagCRC = 0U;
        resetRTR();
        Board1_DW.is_LS_Receive = Board1_IN_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board1_DW.flagCRC = compareCRC(&Board1_U.rx_buffer[0],
          Board1_DW.rxPayload);
      }
      break;

     case Board1_IN_SendAck:
      Board1_DW.is_LS_Receive = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.exit_port_index_LS_Receive = 2U;
      break;

     default:
      /* case IN_SendNack: */
      Board1_DW.is_LS_Receive = Board1_IN_R_ArmingReceive;

      /* Inport: '<Root>/rx_buffer' */
      Board1_DW.receiveArmed = UART_ReceiveIT(&Board1_U.rx_buffer[0], (uint64_T)
        (Board1_DW.rxPayload + ((uint8_T)CRC_SIZE)));
      break;
    }

    if (Board1_DW.exit_port_index_LS_Receive == 2U) {
      Board1_DW.exit_port_index_LS_Receive = 0U;

      /* Inport: '<Root>/rx_buffer' */
      deserializeLocalStateB2(&Board1_U.rx_buffer[0], Board1_DW.rxPayload,
        &Board1_DW.board2LocalState);
      Board1_DW.is_ExchangeLocalState = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.exit_port_index_ExchangeLocalSt = 2U;
    }
  } else {
    /* case IN_LS_Transmit: */
    switch (Board1_DW.is_LS_Transmit) {
     case Board1_IN_EnterDegraded_e:
      break;

     case Board1_IN_R_ReceivingRTR:
      if (checkRTR() != 0) {
        Board1_DW.is_LS_Transmit = Board1_IN_R_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board1_Y.tx_buffer[0], (uint64_T)(Board1_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board1_IN_R_Trasmit:
      Board1_DW.is_LS_Transmit = Board1_IN_R_WaitAck;
      break;

     case Board1_IN_R_WaitAck:
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 0) {
          Board1_DW.is_LS_Transmit = Board1_IN_EnterDegraded_e;
          enterDegraded();
        } else if (UART_CheckAck() == 1) {
          Board1_DW.is_LS_Transmit = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.exit_port_index_LS_Transmit = 2U;
        }
      }
      break;

     case Board1_IN_ReceivingRTR:
      if (checkRTR() != 0) {
        Board1_DW.is_LS_Transmit = Board1_IN_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board1_Y.tx_buffer[0], (uint64_T)(Board1_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board1_IN_Trasmit:
      Board1_DW.is_LS_Transmit = Board1_IN_WaitAck;
      break;

     default:
      /* case IN_WaitAck: */
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 1) {
          Board1_DW.is_LS_Transmit = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.exit_port_index_LS_Transmit = 2U;
        } else if (UART_CheckAck() == 0) {
          Board1_DW.is_LS_Transmit = Board1_IN_R_ReceivingRTR;
        }
      }
      break;
    }

    if (Board1_DW.exit_port_index_LS_Transmit == 2U) {
      Board1_DW.exit_port_index_LS_Transmit = 0U;
      Board1_DW.is_ExchangeLocalState = Board1_IN_LS_Receive;
      Board1_DW.is_LS_Receive = Board1_IN_ArmingReceive;

      /* Inport: '<Root>/rx_buffer' */
      Board1_DW.receiveArmed = UART_ReceiveIT(&Board1_U.rx_buffer[0], (uint64_T)
        (Board1_DW.rxPayload + ((uint8_T)CRC_SIZE)));
    }
  }

  if (Board1_DW.exit_port_index_ExchangeLocalSt == 2U) {
    Board1_DW.exit_port_index_ExchangeLocalSt = 0U;
    Board1_DW.is_CommunicationPhase = Board1_IN_ExchangeGlobalState;

    /*  Entry exchange global state */
    Board1_DW.txPayload = ((uint8_T)GLOBAL_STATE_FRAME_SIZE);
    Board1_DW.rxPayload = ((uint8_T)GLOBAL_STATE_FRAME_SIZE);
    Board1_DW.is_ExchangeGlobalState = Boar_IN_ComputingOwnGlobalState;
    Board1_B.board1GlobalState.localStateB1 = Board1_DW.board1LocalState;
    Board1_B.board1GlobalState.localStateB2 = Board1_DW.board2LocalState;
  }
}

/* Function for Chart: '<Root>/SupervisorB1' */
static ENUM_UserAction Board1_computeUserAction(int16_T x_lever, int16_T y_lever,
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

/* Function for Chart: '<Root>/SupervisorB1' */
static ENUM_UserAction Board1_continueBraking(ENUM_UserAction currentUserAction,
  ENUM_UserAction lastUserAction)
{
  ENUM_UserAction userAction;
  userAction = currentUserAction;
  if (currentUserAction == UA_NONE) {
    switch (lastUserAction) {
     case UA_BRAKING_SMOOTH:
      userAction = UA_BRAKING_SMOOTH;
      break;

     case UA_BRAKING_HARD:
      userAction = UA_BRAKING_HARD;
      break;
    }
  }

  return userAction;
}

/* Function for Chart: '<Root>/SupervisorB1' */
static void Board1_actuatorSelection(ENUM_Actuator lastActuator, uint32_T
  lastCounter, uint32_T treshold, ENUM_Actuator *nextActuator, uint32_T
  *nextCounter)
{
  *nextActuator = lastActuator;
  *nextCounter = lastCounter + 1U;
  if (lastCounter + 1U == treshold) {
    *nextCounter = 0U;
    if (lastActuator == BOARD1) {
      *nextActuator = BOARD2;
    } else {
      *nextActuator = BOARD1;
    }
  }
}

/* Function for Chart: '<Root>/SupervisorB1' */
static ENUM_StatusWhiteLed Board1_evaluateLed(uint16_T buttons, uint16_T
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

/* Function for Chart: '<Root>/SupervisorB1' */
static void Board1_ExchangeGlobalState(void)
{
  switch (Board1_DW.is_ExchangeGlobalState) {
   case Boar_IN_ComputingOwnGlobalState:
    /* Outport: '<Root>/tx_buffer' */
    serializeGlobalState(&Board1_Y.tx_buffer[0], &Board1_B.board1GlobalState);
    computeCRC(&Board1_Y.tx_buffer[0], Board1_DW.txPayload);
    Board1_DW.is_ExchangeGlobalState = Board1_IN_GL_Transmit;
    Board1_DW.is_GL_Transmit = Board1_IN_ReceivingRTR;
    break;

   case Board1_IN_GL_Receive:
    switch (Board1_DW.is_GL_Receive) {
     case Board1_IN_ArmingReceive:
      if (Board1_DW.receiveArmed == 1) {
        Board1_DW.is_GL_Receive = Board1_IN_ReadyToReceive;
        setRTR();
      }
      break;

     case Board1_IN_CheckCRC:
      switch (Board1_DW.flagCRC) {
       case 1:
        Board1_DW.is_GL_Receive = Board1_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board1_DW.is_GL_Receive = Board1_IN_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board1_IN_EnterDegraded:
      break;

     case Board1_IN_R_ArmingReceive:
      if (Board1_DW.receiveArmed == 1) {
        Board1_DW.is_GL_Receive = Board1_IN_R_ReadyToReceive;
        setRTR();
      }
      break;

     case Board1_IN_R_CheckCRC:
      switch (Board1_DW.flagCRC) {
       case 1:
        Board1_DW.is_GL_Receive = Board1_IN_SendAck;
        UART_SendAckIT();
        break;

       case 0:
        Board1_DW.is_GL_Receive = Board1_IN_R_SendNack;
        UART_SendNackIT();
        break;
      }
      break;

     case Board1_IN_R_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board1_DW.is_GL_Receive = Board1_IN_R_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board1_DW.flagCRC = 0U;
        resetRTR();
        Board1_DW.is_GL_Receive = Board1_IN_R_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board1_DW.flagCRC = compareCRC(&Board1_U.rx_buffer[0],
          Board1_DW.rxPayload);
      }
      break;

     case Board1_IN_R_SendNack:
      Board1_DW.is_GL_Receive = Board1_IN_EnterDegraded;
      enterDegraded();
      break;

     case Board1_IN_ReadyToReceive:
      if (errorReceived() == 1) {
        resetRTR();
        Board1_DW.is_GL_Receive = Board1_IN_SendNack;
        UART_SendNackIT();
      } else if (hasReceived() == 1) {
        Board1_DW.flagCRC = 0U;
        resetRTR();
        Board1_DW.is_GL_Receive = Board1_IN_CheckCRC;

        /* Inport: '<Root>/rx_buffer' */
        Board1_DW.flagCRC = compareCRC(&Board1_U.rx_buffer[0],
          Board1_DW.rxPayload);
      }
      break;

     case Board1_IN_SendAck:
      Board1_DW.is_GL_Receive = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.exit_port_index_GL_Receive = 2U;
      break;

     default:
      /* case IN_SendNack: */
      Board1_DW.is_GL_Receive = Board1_IN_R_ArmingReceive;

      /* Inport: '<Root>/rx_buffer' */
      Board1_DW.receiveArmed = UART_ReceiveIT(&Board1_U.rx_buffer[0], (uint64_T)
        (Board1_DW.rxPayload + ((uint8_T)CRC_SIZE)));
      break;
    }

    if (Board1_DW.exit_port_index_GL_Receive == 2U) {
      Board1_DW.exit_port_index_GL_Receive = 0U;

      /* Inport: '<Root>/rx_buffer' */
      deserializeGlobalState(&Board1_U.rx_buffer[0], Board1_DW.rxPayload,
        &Board1_DW.board2GlobalState);
      Board1_DW.is_ExchangeGlobalState = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.exit_port_index_ExchangeGlobalS = 2U;
    }
    break;

   default:
    /* case IN_GL_Transmit: */
    switch (Board1_DW.is_GL_Transmit) {
     case Board1_IN_EnterDegraded_e:
      break;

     case Board1_IN_R_ReceivingRTR:
      if (checkRTR() != 0) {
        Board1_DW.is_GL_Transmit = Board1_IN_R_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board1_Y.tx_buffer[0], (uint64_T)(Board1_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board1_IN_R_Trasmit:
      Board1_DW.is_GL_Transmit = Board1_IN_R_WaitAck;
      break;

     case Board1_IN_R_WaitAck:
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 0) {
          Board1_DW.is_GL_Transmit = Board1_IN_EnterDegraded_e;
          enterDegraded();
        } else if (UART_CheckAck() == 1) {
          Board1_DW.is_GL_Transmit = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.exit_port_index_GL_Transmit = 2U;
        }
      }
      break;

     case Board1_IN_ReceivingRTR:
      if (checkRTR() != 0) {
        Board1_DW.is_GL_Transmit = Board1_IN_Trasmit;
        UART_ReceiveAckIT();

        /* Outport: '<Root>/tx_buffer' */
        UART_TransmitIT(&Board1_Y.tx_buffer[0], (uint64_T)(Board1_DW.txPayload +
          ((uint8_T)CRC_SIZE)));
      }
      break;

     case Board1_IN_Trasmit:
      Board1_DW.is_GL_Transmit = Board1_IN_WaitAck;
      break;

     default:
      /* case IN_WaitAck: */
      if (hasReceived() == 1) {
        if (UART_CheckAck() == 1) {
          Board1_DW.is_GL_Transmit = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.exit_port_index_GL_Transmit = 2U;
        } else if (UART_CheckAck() == 0) {
          Board1_DW.is_GL_Transmit = Board1_IN_R_ReceivingRTR;
        }
      }
      break;
    }

    if (Board1_DW.exit_port_index_GL_Transmit == 2U) {
      Board1_DW.exit_port_index_GL_Transmit = 0U;
      Board1_DW.is_ExchangeGlobalState = Board1_IN_GL_Receive;
      Board1_DW.is_GL_Receive = Board1_IN_ArmingReceive;

      /* Inport: '<Root>/rx_buffer' */
      Board1_DW.receiveArmed = UART_ReceiveIT(&Board1_U.rx_buffer[0], (uint64_T)
        (Board1_DW.rxPayload + ((uint8_T)CRC_SIZE)));
    }
    break;
  }

  if (Board1_DW.exit_port_index_ExchangeGlobalS == 2U) {
    Board1_DW.exit_port_index_ExchangeGlobalS = 0U;
    Board1_DW.is_CommunicationPhase = Board1_IN_ComputeDecision;

    /*  Entry compute decision */
    Board1_DW.is_ComputeDecision = B_IN_StateCoherenceVerification;
  }
}

/* Model step function */
void Board1_step(void)
{
  real_T continua_prev;
  uint32_T tmp;
  ENUM_Actuator nextActuator;

  /* Chart: '<Root>/SupervisorB1' incorporates:
   *  Inport: '<Root>/batteryLevel'
   *  Inport: '<Root>/continua'
   *  Inport: '<Root>/rx_buffer'
   *  Inport: '<Root>/speed'
   *  Inport: '<Root>/temperature'
   *  Outport: '<Root>/currentUserAction'
   *  Outport: '<Root>/roverAction'
   *  Outport: '<Root>/safeAction'
   *  Outport: '<Root>/setPoint'
   *  Outport: '<Root>/tx_buffer'
   */
  continua_prev = Board1_DW.continua_start;
  Board1_DW.continua_start = Board1_U.continua;
  if (Board1_DW.is_active_c15_Board1 == 0U) {
    Board1_DW.is_active_c15_Board1 = 1U;
    Board1_DW.board1Decision.actuator = BOARD1;

    /* Outport: '<Root>/currentUserAction' */
    Board1_Y.currentUserAction = UA_NONE;
    Board1_DW.actuatorCounter = 0U;
    Board1_DW.previousButtons = 0U;
    Board1_DW.previousWhiteLeftLed = WHITE_OFF;
    Board1_DW.previousWhiteRightLed = WHITE_OFF;
    Board1_DW.is_RoverState = Board1_IN_NotCommunicating;

    /* Outport: '<Root>/supervision_ended' */
    Board1_Y.supervision_ended = 0U;
  } else if (Board1_DW.is_RoverState == Board1_IN_CommunicationPhase) {
    switch (Board1_DW.is_CommunicationPhase) {
     case Board1_IN_ComputeDecision:
      switch (Board1_DW.is_ComputeDecision) {
       case Board1_IN_ActuatorSelection:
        Board1_DW.is_ComputeDecision = Board1_IN_NO_ACTIVE_CHILD;
        Board1_DW.exit_port_index_ComputeDecision = 2U;
        break;

       case Board_IN_EmergencyStateAnalysis:
        Board1_DW.is_ComputeDecision = Board1_IN_UserActionComputation;
        Board1_DW.board1Decision.userAction = Board1_computeUserAction
          (Board1_B.board1GlobalState.localStateB2.remoteController.x_lever,
           Board1_B.board1GlobalState.localStateB2.remoteController.y_lever,
           Board1_B.board1GlobalState.localStateB2.remoteController.buttons,
           ((uint16_T)BRAKING_HARD_MASK), ((uint16_T)BRAKING_SMOOTH_MASK));
        Board1_DW.board1Decision.userAction = Board1_continueBraking
          (Board1_DW.board1Decision.userAction, Board1_Y.currentUserAction);

        /* Outport: '<Root>/currentUserAction' */
        Board1_Y.currentUserAction = Board1_DW.board1Decision.userAction;
        break;

       case Board1_IN_LightEvaluation:
        Board1_DW.is_ComputeDecision = Board1_IN_ActuatorSelection;
        Board1_actuatorSelection(Board1_DW.board1Decision.actuator,
          Board1_DW.actuatorCounter, Board1_TRESHOLD_ACTUATOR_SWITCH,
          &nextActuator, &tmp);
        Board1_DW.actuatorCounter = tmp;
        Board1_DW.board1Decision.actuator = nextActuator;
        break;

       case Board_IN_RoverActionComputation:
        Board1_DW.is_ComputeDecision = Board1_IN_LightEvaluation;
        Board1_DW.board1Decision.leds.white.left = Board1_evaluateLed
          (Board1_B.board1GlobalState.localStateB2.remoteController.buttons,
           Board1_DW.previousButtons, Board1_DW.previousWhiteLeftLed, ((uint16_T)
            WHITE_LEFT_LED_MASK));
        Board1_DW.board1Decision.leds.white.right = Board1_evaluateLed
          (Board1_B.board1GlobalState.localStateB2.remoteController.buttons,
           Board1_DW.previousButtons, Board1_DW.previousWhiteRightLed,
           ((uint16_T)WHITE_RIGHT_LED_MASK));

        /*  Aggiornamento variabili di stato */
        Board1_DW.previousButtons =
          Board1_B.board1GlobalState.localStateB2.remoteController.buttons;
        Board1_DW.previousWhiteLeftLed =
          Board1_DW.board1Decision.leds.white.left;
        Board1_DW.previousWhiteRightLed =
          Board1_DW.board1Decision.leds.white.right;
        break;

       case B_IN_StateCoherenceVerification:
        Board1_DW.is_ComputeDecision = Board_IN_EmergencyStateAnalysis;
        break;

       default:
        /* case IN_UserActionComputation: */
        Board1_DW.is_ComputeDecision = Board_IN_RoverActionComputation;

        /* ModelReference: '<Root>/ActionsModel' incorporates:
         *  Outport: '<Root>/currentUserAction'
         *  Outport: '<Root>/roverAction'
         *  Outport: '<Root>/safeAction'
         *  Outport: '<Root>/setPoint'
         *  Outport: '<Root>/statusObstacles'
         */
        ActionsModel_ComputeRoverAction(&Board1_Y.currentUserAction,
          &Board1_B.board1GlobalState.localStateB1.speed,
          &Board1_B.board1GlobalState.localStateB2.remoteController.x_lever,
          &Board1_B.board1GlobalState.localStateB2.remoteController.y_lever,
          &Board1_B.board1GlobalState.localStateB2.gyroscope,
          &Board1_B.board1GlobalState.localStateB2.sonar, &Board1_Y.setPoint,
          &Board1_Y.roverAction, &Board1_Y.safeAction, &Board1_Y.statusObstacles,
          &Board1_B.redLeds);
        Board1_DW.board1Decision.roverAction = Board1_Y.roverAction;
        Board1_DW.board1Decision.safeAction = Board1_Y.safeAction;
        Board1_DW.board1Decision.setPoint = Board1_Y.setPoint;

        /*  Light update */
        Board1_DW.board1Decision.leds.red = Board1_B.redLeds;
        break;
      }

      if (Board1_DW.exit_port_index_ComputeDecision == 2U) {
        Board1_DW.exit_port_index_ComputeDecision = 0U;
        Board1_DW.is_CommunicationPhase = Board1_IN_ExchangeDecision;

        /*  Entry exchange decision */
        Board1_DW.txPayload = ((uint8_T)DECISION_FRAME_SIZE);
        Board1_DW.rxPayload = ((uint8_T)DECISION_FRAME_SIZE);
        serializeDecision(&Board1_Y.tx_buffer[0], &Board1_DW.board1Decision);
        computeCRC(&Board1_Y.tx_buffer[0], Board1_DW.txPayload);
        Board1_DW.is_ExchangeDecision = Board1_IN_D_Transmit;
        Board1_DW.is_D_Transmit = Board1_IN_ReceivingRTR;
      }
      break;

     case Board1_IN_ExchangeDecision:
      switch (Board1_DW.is_ExchangeDecision) {
       case Board1_IN_D_Receive:
        switch (Board1_DW.is_D_Receive) {
         case Board1_IN_ArmingReceive:
          if (Board1_DW.receiveArmed == 1) {
            Board1_DW.is_D_Receive = Board1_IN_ReadyToReceive;
            setRTR();
          }
          break;

         case Board1_IN_CheckCRC:
          switch (Board1_DW.flagCRC) {
           case 1:
            Board1_DW.is_D_Receive = Board1_IN_SendAck;
            UART_SendAckIT();
            break;

           case 0:
            Board1_DW.is_D_Receive = Board1_IN_SendNack;
            UART_SendNackIT();
            break;
          }
          break;

         case Board1_IN_EnterDegraded:
          break;

         case Board1_IN_R_ArmingReceive:
          if (Board1_DW.receiveArmed == 1) {
            Board1_DW.is_D_Receive = Board1_IN_R_ReadyToReceive;
            setRTR();
          }
          break;

         case Board1_IN_R_CheckCRC:
          switch (Board1_DW.flagCRC) {
           case 1:
            Board1_DW.is_D_Receive = Board1_IN_SendAck;
            UART_SendAckIT();
            break;

           case 0:
            Board1_DW.is_D_Receive = Board1_IN_R_SendNack;
            UART_SendNackIT();
            break;
          }
          break;

         case Board1_IN_R_ReadyToReceive:
          if (errorReceived() == 1) {
            resetRTR();
            Board1_DW.is_D_Receive = Board1_IN_R_SendNack;
            UART_SendNackIT();
          } else if (hasReceived() == 1) {
            Board1_DW.flagCRC = 0U;
            resetRTR();
            Board1_DW.is_D_Receive = Board1_IN_R_CheckCRC;
            Board1_DW.flagCRC = compareCRC(&Board1_U.rx_buffer[0],
              Board1_DW.rxPayload);
          }
          break;

         case Board1_IN_R_SendNack:
          Board1_DW.is_D_Receive = Board1_IN_EnterDegraded;
          enterDegraded();
          break;

         case Board1_IN_ReadyToReceive:
          if (errorReceived() == 1) {
            resetRTR();
            Board1_DW.is_D_Receive = Board1_IN_SendNack;
            UART_SendNackIT();
          } else if (hasReceived() == 1) {
            Board1_DW.flagCRC = 0U;
            resetRTR();
            Board1_DW.is_D_Receive = Board1_IN_CheckCRC;
            Board1_DW.flagCRC = compareCRC(&Board1_U.rx_buffer[0],
              Board1_DW.rxPayload);
          }
          break;

         case Board1_IN_SendAck:
          Board1_DW.is_D_Receive = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.exit_port_index_D_Receive = 2U;
          break;

         default:
          /* case IN_SendNack: */
          Board1_DW.is_D_Receive = Board1_IN_R_ArmingReceive;
          Board1_DW.receiveArmed = UART_ReceiveIT(&Board1_U.rx_buffer[0],
            (uint64_T)(Board1_DW.rxPayload + ((uint8_T)CRC_SIZE)));
          break;
        }

        if (Board1_DW.exit_port_index_D_Receive == 2U) {
          Board1_DW.exit_port_index_D_Receive = 0U;
          deserializeDecision(&Board1_U.rx_buffer[0], Board1_DW.rxPayload,
                              &Board1_DW.board2Decision);
          Board1_DW.is_ExchangeDecision = Board1_IN_Execution;

          /* Outport: '<Root>/supervision_ended' incorporates:
           *  Inport: '<Root>/rx_buffer'
           */
          Board1_Y.supervision_ended = 1U;

          /* printGlobalState(board1GlobalState); printGlobalState(board2GlobalState);
             printDecision(board1Decision); printDecision(board2Decision); */
        }
        break;

       case Board1_IN_D_Transmit:
        switch (Board1_DW.is_D_Transmit) {
         case Board1_IN_EnterDegraded_e:
          break;

         case Board1_IN_R_ReceivingRTR:
          if (checkRTR() != 0) {
            Board1_DW.is_D_Transmit = Board1_IN_R_Trasmit;
            UART_ReceiveAckIT();
            UART_TransmitIT(&Board1_Y.tx_buffer[0], (uint64_T)
                            (Board1_DW.txPayload + ((uint8_T)CRC_SIZE)));
          }
          break;

         case Board1_IN_R_Trasmit:
          Board1_DW.is_D_Transmit = Board1_IN_R_WaitAck;
          break;

         case Board1_IN_R_WaitAck:
          if (hasReceived() == 1) {
            if (UART_CheckAck() == 0) {
              Board1_DW.is_D_Transmit = Board1_IN_EnterDegraded_e;
              enterDegraded();
            } else if (UART_CheckAck() == 1) {
              Board1_DW.is_D_Transmit = Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.exit_port_index_D_Transmit = 2U;
            }
          }
          break;

         case Board1_IN_ReceivingRTR:
          if (checkRTR() != 0) {
            Board1_DW.is_D_Transmit = Board1_IN_Trasmit;
            UART_ReceiveAckIT();
            UART_TransmitIT(&Board1_Y.tx_buffer[0], (uint64_T)
                            (Board1_DW.txPayload + ((uint8_T)CRC_SIZE)));
          }
          break;

         case Board1_IN_Trasmit:
          Board1_DW.is_D_Transmit = Board1_IN_WaitAck;
          break;

         default:
          /* case IN_WaitAck: */
          if (hasReceived() == 1) {
            if (UART_CheckAck() == 1) {
              Board1_DW.is_D_Transmit = Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.exit_port_index_D_Transmit = 2U;
            } else if (UART_CheckAck() == 0) {
              Board1_DW.is_D_Transmit = Board1_IN_R_ReceivingRTR;
            }
          }
          break;
        }

        if (Board1_DW.exit_port_index_D_Transmit == 2U) {
          Board1_DW.exit_port_index_D_Transmit = 0U;
          Board1_DW.is_ExchangeDecision = Board1_IN_D_Receive;
          Board1_DW.is_D_Receive = Board1_IN_ArmingReceive;
          Board1_DW.receiveArmed = UART_ReceiveIT(&Board1_U.rx_buffer[0],
            (uint64_T)(Board1_DW.rxPayload + ((uint8_T)CRC_SIZE)));
        }
        break;

       default:
        /* Outport: '<Root>/supervision_ended' */
        /* case IN_Execution: */
        Board1_Y.supervision_ended = 1U;
        if (continua_prev != Board1_DW.continua_start) {
          Board1_DW.is_ExchangeDecision = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.exit_port_index_ExchangeDecisio = 2U;
        }
        break;
      }

      if (Board1_DW.exit_port_index_ExchangeDecisio == 2U) {
        Board1_DW.exit_port_index_ExchangeDecisio = 0U;
        Board1_DW.is_CommunicationPhase = Board1_IN_NO_ACTIVE_CHILD;
        Board1_DW.exit_port_index_CommunicationPh = 2U;
      }
      break;

     case Board1_IN_ExchangeGlobalState:
      Board1_ExchangeGlobalState();
      break;

     default:
      /* case IN_ExchangeLocalState: */
      Board1_ExchangeLocalState();
      break;
    }

    if (Board1_DW.exit_port_index_CommunicationPh == 2U) {
      Board1_DW.exit_port_index_CommunicationPh = 0U;
      Board1_DW.is_RoverState = Board1_IN_NotCommunicating;

      /* Outport: '<Root>/supervision_ended' */
      Board1_Y.supervision_ended = 0U;
    }
  } else {
    /* Outport: '<Root>/supervision_ended' */
    /* case IN_NotCommunicating: */
    Board1_Y.supervision_ended = 0U;
    Board1_DW.board1LocalState.speed = Board1_U.speed;
    Board1_DW.board1LocalState.temperature = Board1_U.temperature;
    Board1_DW.board1LocalState.batteryLevel = Board1_U.batteryLevel;
    Board1_DW.is_RoverState = Board1_IN_CommunicationPhase;
    Board1_DW.is_CommunicationPhase = Board1_IN_ExchangeLocalState;

    /*  Entry exchange local state */
    Board1_DW.txPayload = ((uint8_T)LOCAL_STATE_B1_FRAME_SIZE);
    Board1_DW.rxPayload = ((uint8_T)LOCAL_STATE_B2_FRAME_SIZE);
    serializeLocalStateB1(&Board1_Y.tx_buffer[0], &Board1_DW.board1LocalState);
    computeCRC(&Board1_Y.tx_buffer[0], Board1_DW.txPayload);
    Board1_DW.is_ExchangeLocalState = Board1_IN_LS_Transmit;
    Board1_DW.is_LS_Transmit = Board1_IN_ReceivingRTR;
  }

  /* End of Chart: '<Root>/SupervisorB1' */
}

/* Model initialize function */
void Board1_initialize(void)
{
  /* Model Initialize function for ModelReference Block: '<Root>/ActionsModel' */
  ActionsModel_initialize(rtmGetErrorStatusPointer(Board1_M));

  /* SystemInitialize for ModelReference: '<Root>/ActionsModel' incorporates:
   *  Outport: '<Root>/currentUserAction'
   *  Outport: '<Root>/roverAction'
   *  Outport: '<Root>/safeAction'
   *  Outport: '<Root>/setPoint'
   *  Outport: '<Root>/statusObstacles'
   */
  ActionsModel_Init(&Board1_Y.setPoint, &Board1_Y.statusObstacles,
                    &Board1_B.redLeds);
}

/* Model terminate function */
void Board1_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
