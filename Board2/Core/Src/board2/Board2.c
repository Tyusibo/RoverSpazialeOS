/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board2.c
 *
 * Code generated for Simulink model 'Board2'.
 *
 * Model version                  : 4.55
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Tue Jan 20 22:12:22 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Board2.h"
#include "enums.h"
#include "rtwtypes.h"
#include "controller_masks.h"
#include "crc_functions.h"
#include "frame_size.h"
#include "ActionsModel.h"

/* Named constants for Chart: '<Root>/SupervisorB2' */
#define B_IN_StateCoherenceVerification ((uint8_T)5U)
#define Boar_IN_ComputingOwnGlobalState ((uint8_T)1U)
#define Board2_IN_ActuatorSelection    ((uint8_T)1U)
#define Board2_IN_ArmingReceive        ((uint8_T)1U)
#define Board2_IN_CheckCRC             ((uint8_T)2U)
#define Board2_IN_CommunicationPhase   ((uint8_T)1U)
#define Board2_IN_ComputeDecision      ((uint8_T)1U)
#define Board2_IN_D_Receive            ((uint8_T)1U)
#define Board2_IN_D_Transmit           ((uint8_T)2U)
#define Board2_IN_EnterDegraded        ((uint8_T)3U)
#define Board2_IN_EnterDegraded_e      ((uint8_T)1U)
#define Board2_IN_ExchangeDecision     ((uint8_T)2U)
#define Board2_IN_ExchangeGlobalState  ((uint8_T)3U)
#define Board2_IN_ExchangeLocalState   ((uint8_T)4U)
#define Board2_IN_Execution            ((uint8_T)3U)
#define Board2_IN_GL_Receive           ((uint8_T)2U)
#define Board2_IN_GL_Transmit          ((uint8_T)3U)
#define Board2_IN_LS_Receive           ((uint8_T)1U)
#define Board2_IN_LS_Transmit          ((uint8_T)2U)
#define Board2_IN_LightEvaluation      ((uint8_T)3U)
#define Board2_IN_NO_ACTIVE_CHILD      ((uint8_T)0U)
#define Board2_IN_NotCommunicating     ((uint8_T)2U)
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
#define Board2_IN_Trasmit              ((uint8_T)6U)
#define Board2_IN_UserActionComputation ((uint8_T)6U)
#define Board2_IN_WaitAck              ((uint8_T)7U)
#define Board2_TRESHOLD_ACTUATOR_SWITCH (1000U)
#define Board_IN_EmergencyStateAnalysis ((uint8_T)2U)
#define Board_IN_RoverActionComputation ((uint8_T)4U)

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
static void Board2_ExchangeLocalState(void);
static ENUM_UserAction Board2_computeUserAction(int16_T x_lever, int16_T y_lever,
  uint16_T buttons, uint16_T braking_hard_mask, uint16_T braking_smooth_mask);
static ENUM_UserAction Board2_continueBraking(ENUM_UserAction currentUserAction,
  ENUM_UserAction lastUserAction);
static void Board2_actuatorSelection(ENUM_Actuator lastActuator, uint32_T
  lastCounter, uint32_T treshold, ENUM_Actuator *nextActuator, uint32_T
  *nextCounter);
static ENUM_StatusWhiteLed Board2_evaluateLed(uint16_T buttons, uint16_T
  lastButtons, ENUM_StatusWhiteLed previousLedStatus, uint16_T button_mask);
static void Board2_ExchangeGlobalState(void);

/* Function for Chart: '<Root>/SupervisorB2' */
static void Board2_ExchangeLocalState(void)
{
  if (Board2_DW.is_ExchangeLocalState == Board2_IN_LS_Receive) {
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
  } else {
    /* case IN_LS_Transmit: */
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
      Board2_DW.is_ExchangeLocalState = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.exit_port_index_ExchangeLocalSt = 2U;
    }
  }

  if (Board2_DW.exit_port_index_ExchangeLocalSt == 2U) {
    Board2_DW.exit_port_index_ExchangeLocalSt = 0U;
    Board2_DW.is_CommunicationPhase = Board2_IN_ExchangeGlobalState;

    /*  Entry exchange global state */
    Board2_DW.txPayload = ((uint8_T)GLOBAL_STATE_FRAME_SIZE);
    Board2_DW.rxPayload = ((uint8_T)GLOBAL_STATE_FRAME_SIZE);
    Board2_DW.is_ExchangeGlobalState = Boar_IN_ComputingOwnGlobalState;
    Board2_B.board2GlobalState.localStateB1 = Board2_DW.board1LocalState;

    /*  forse dovrei fare una copia del backup all'inizio del mio stato,
       siccome durante la trasmissione le variabili possono cambiare */
    Board2_B.board2GlobalState.localStateB2 = Board2_DW.board2LocalState;
  }
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
static ENUM_UserAction Board2_continueBraking(ENUM_UserAction currentUserAction,
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

/* Function for Chart: '<Root>/SupervisorB2' */
static void Board2_actuatorSelection(ENUM_Actuator lastActuator, uint32_T
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

      /* Outport: '<Root>/tx_buffer' */
      serializeGlobalState(&Board2_Y.tx_buffer[0], &Board2_B.board2GlobalState);
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
  }
}

/* Model step function */
void Board2_step(void)
{
  real_T continua_prev;
  uint32_T tmp;
  ENUM_Actuator nextActuator;

  /* Chart: '<Root>/SupervisorB2' incorporates:
   *  Inport: '<Root>/continua'
   *  Inport: '<Root>/gyroscope'
   *  Inport: '<Root>/remoteController'
   *  Inport: '<Root>/rx_buffer'
   *  Inport: '<Root>/sonar'
   *  Outport: '<Root>/currentUserAction'
   *  Outport: '<Root>/roverAction'
   *  Outport: '<Root>/safeAction'
   *  Outport: '<Root>/setPoint'
   *  Outport: '<Root>/tx_buffer'
   */
  continua_prev = Board2_DW.continua_start;
  Board2_DW.continua_start = Board2_U.continua;
  if (Board2_DW.is_active_c3_Board2 == 0U) {
    Board2_DW.is_active_c3_Board2 = 1U;
    Board2_DW.board2Decision.actuator = BOARD1;

    /* Outport: '<Root>/currentUserAction' */
    Board2_Y.currentUserAction = UA_NONE;
    Board2_DW.actuatorCounter = 0U;
    Board2_DW.previousButtons = 0U;
    Board2_DW.previousWhiteLeftLed = WHITE_OFF;
    Board2_DW.previousWhiteRightLed = WHITE_OFF;
    Board2_DW.is_RoverState = Board2_IN_NotCommunicating;

    /* Outport: '<Root>/supervision_ended' */
    Board2_Y.supervision_ended = 0U;
  } else if (Board2_DW.is_RoverState == Board2_IN_CommunicationPhase) {
    switch (Board2_DW.is_CommunicationPhase) {
     case Board2_IN_ComputeDecision:
      switch (Board2_DW.is_ComputeDecision) {
       case Board2_IN_ActuatorSelection:
        Board2_DW.is_ComputeDecision = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.exit_port_index_ComputeDecision = 2U;
        break;

       case Board_IN_EmergencyStateAnalysis:
        Board2_DW.is_ComputeDecision = Board2_IN_UserActionComputation;
        Board2_DW.board2Decision.userAction = Board2_computeUserAction
          (Board2_B.board2GlobalState.localStateB2.remoteController.x_lever,
           Board2_B.board2GlobalState.localStateB2.remoteController.y_lever,
           Board2_B.board2GlobalState.localStateB2.remoteController.buttons,
           ((uint16_T)BRAKING_HARD_MASK), ((uint16_T)BRAKING_SMOOTH_MASK));
        Board2_DW.board2Decision.userAction = Board2_continueBraking
          (Board2_DW.board2Decision.userAction, Board2_Y.currentUserAction);

        /* Outport: '<Root>/currentUserAction' */
        Board2_Y.currentUserAction = Board2_DW.board2Decision.userAction;
        break;

       case Board2_IN_LightEvaluation:
        Board2_DW.is_ComputeDecision = Board2_IN_ActuatorSelection;
        Board2_actuatorSelection(Board2_DW.board2Decision.actuator,
          Board2_DW.actuatorCounter, Board2_TRESHOLD_ACTUATOR_SWITCH,
          &nextActuator, &tmp);
        Board2_DW.actuatorCounter = tmp;
        Board2_DW.board2Decision.actuator = nextActuator;
        break;

       case Board_IN_RoverActionComputation:
        Board2_DW.is_ComputeDecision = Board2_IN_LightEvaluation;
        Board2_DW.board2Decision.leds.white.left = Board2_evaluateLed
          (Board2_B.board2GlobalState.localStateB2.remoteController.buttons,
           Board2_DW.previousButtons, Board2_DW.previousWhiteLeftLed, ((uint16_T)
            WHITE_LEFT_LED_MASK));
        Board2_DW.board2Decision.leds.white.right = Board2_evaluateLed
          (Board2_B.board2GlobalState.localStateB2.remoteController.buttons,
           Board2_DW.previousButtons, Board2_DW.previousWhiteRightLed,
           ((uint16_T)WHITE_RIGHT_LED_MASK));

        /*  Aggiornamento variabili di stato */
        Board2_DW.previousButtons =
          Board2_B.board2GlobalState.localStateB2.remoteController.buttons;
        Board2_DW.previousWhiteLeftLed =
          Board2_DW.board2Decision.leds.white.left;
        Board2_DW.previousWhiteRightLed =
          Board2_DW.board2Decision.leds.white.right;
        break;

       case B_IN_StateCoherenceVerification:
        Board2_DW.is_ComputeDecision = Board_IN_EmergencyStateAnalysis;
        break;

       default:
        /* case IN_UserActionComputation: */
        Board2_DW.is_ComputeDecision = Board_IN_RoverActionComputation;

        /* ModelReference: '<Root>/ActionsModel' incorporates:
         *  Outport: '<Root>/currentUserAction'
         *  Outport: '<Root>/roverAction'
         *  Outport: '<Root>/safeAction'
         *  Outport: '<Root>/setPoint'
         *  Outport: '<Root>/statusObstacles'
         */
        ActionsModel_ComputeRoverAction(&Board2_Y.currentUserAction,
          &Board2_B.board2GlobalState.localStateB1.speed,
          &Board2_B.board2GlobalState.localStateB2.remoteController.x_lever,
          &Board2_B.board2GlobalState.localStateB2.remoteController.y_lever,
          &Board2_B.board2GlobalState.localStateB2.gyroscope,
          &Board2_B.board2GlobalState.localStateB2.sonar, &Board2_Y.setPoint,
          &Board2_Y.roverAction, &Board2_Y.safeAction, &Board2_Y.statusObstacles,
          &Board2_B.redLeds);
        Board2_DW.board2Decision.roverAction = Board2_Y.roverAction;
        Board2_DW.board2Decision.safeAction = Board2_Y.safeAction;
        Board2_DW.board2Decision.setPoint = Board2_Y.setPoint;

        /*  Light update */
        Board2_DW.board2Decision.leds.red = Board2_B.redLeds;
        break;
      }

      if (Board2_DW.exit_port_index_ComputeDecision == 2U) {
        Board2_DW.exit_port_index_ComputeDecision = 0U;
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
      switch (Board2_DW.is_ExchangeDecision) {
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
          Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0],
            (uint64_T)(Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
          break;
        }

        if (Board2_DW.exit_port_index_D_Receive == 2U) {
          Board2_DW.exit_port_index_D_Receive = 0U;
          deserializeDecision(&Board2_U.rx_buffer[0], Board2_DW.rxPayload,
                              &Board2_DW.board1Decision);
          serializeDecision(&Board2_Y.tx_buffer[0], &Board2_DW.board2Decision);
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
            UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)
                            (Board2_DW.txPayload + ((uint8_T)CRC_SIZE)));
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
            UART_TransmitIT(&Board2_Y.tx_buffer[0], (uint64_T)
                            (Board2_DW.txPayload + ((uint8_T)CRC_SIZE)));
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
          Board2_DW.is_ExchangeDecision = Board2_IN_Execution;

          /* Outport: '<Root>/supervision_ended' */
          Board2_Y.supervision_ended = 1U;

          /* printGlobalState(board1GlobalState); printGlobalState(board2GlobalState);
             printDecision(board1Decision); printDecision(board2Decision); */
        }
        break;

       default:
        /* Outport: '<Root>/supervision_ended' */
        /* case IN_Execution: */
        Board2_Y.supervision_ended = 1U;
        if (continua_prev != Board2_DW.continua_start) {
          Board2_DW.is_ExchangeDecision = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.exit_port_index_ExchangeDecisio = 2U;
        }
        break;
      }

      if (Board2_DW.exit_port_index_ExchangeDecisio == 2U) {
        Board2_DW.exit_port_index_ExchangeDecisio = 0U;
        Board2_DW.is_CommunicationPhase = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.exit_port_index_CommunicationPh = 2U;
      }
      break;

     case Board2_IN_ExchangeGlobalState:
      Board2_ExchangeGlobalState();
      break;

     default:
      /* case IN_ExchangeLocalState: */
      Board2_ExchangeLocalState();
      break;
    }

    if (Board2_DW.exit_port_index_CommunicationPh == 2U) {
      Board2_DW.exit_port_index_CommunicationPh = 0U;
      Board2_DW.is_RoverState = Board2_IN_NotCommunicating;

      /* Outport: '<Root>/supervision_ended' */
      Board2_Y.supervision_ended = 0U;
    }
  } else {
    /* Outport: '<Root>/supervision_ended' */
    /* case IN_NotCommunicating: */
    Board2_Y.supervision_ended = 0U;
    Board2_DW.board2LocalState.sonar = Board2_U.sonar;
    Board2_DW.board2LocalState.gyroscope = Board2_U.gyroscope;
    Board2_DW.board2LocalState.remoteController = Board2_U.remoteController;
    Board2_DW.is_RoverState = Board2_IN_CommunicationPhase;
    Board2_DW.is_CommunicationPhase = Board2_IN_ExchangeLocalState;

    /*  Entry exchange local state */
    Board2_DW.txPayload = ((uint8_T)LOCAL_STATE_B2_FRAME_SIZE);
    Board2_DW.rxPayload = ((uint8_T)LOCAL_STATE_B1_FRAME_SIZE);
    Board2_DW.is_ExchangeLocalState = Board2_IN_LS_Receive;
    Board2_DW.is_LS_Receive = Board2_IN_ArmingReceive;
    Board2_DW.receiveArmed = UART_ReceiveIT(&Board2_U.rx_buffer[0], (uint64_T)
      (Board2_DW.rxPayload + ((uint8_T)CRC_SIZE)));
  }

  /* End of Chart: '<Root>/SupervisorB2' */
}

/* Model initialize function */
void Board2_initialize(void)
{
  /* Model Initialize function for ModelReference Block: '<Root>/ActionsModel' */
  ActionsModel_initialize(rtmGetErrorStatusPointer(Board2_M));

  /* SystemInitialize for ModelReference: '<Root>/ActionsModel' incorporates:
   *  Outport: '<Root>/currentUserAction'
   *  Outport: '<Root>/roverAction'
   *  Outport: '<Root>/safeAction'
   *  Outport: '<Root>/setPoint'
   *  Outport: '<Root>/statusObstacles'
   */
  ActionsModel_Init(&Board2_Y.setPoint, &Board2_Y.statusObstacles,
                    &Board2_B.redLeds);
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
