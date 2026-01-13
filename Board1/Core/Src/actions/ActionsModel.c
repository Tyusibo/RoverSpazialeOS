/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ActionsModel.c
 *
 * Code generated for Simulink model 'ActionsModel'.
 *
 * Model version                  : 4.4
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Mon Jan 12 16:33:53 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ActionsModel.h"
#include "enums.h"
#include "ActionsModel_private.h"
#include "rtwtypes.h"
#include "sensors.h"
#include "decision.h"
#include <math.h>
#include "rt_nonfinite.h"

/* Named constants for Chart: '<Root>/MovingObstacles' */
#define Ac_IN_PossibleStartingFromRight ((uint8_T)4U)
#define Act_IN_PossibleStartingFromLeft ((uint8_T)3U)
#define Acti_IN_TrackingMovingObstacles ((uint8_T)2U)
#define ActionsMod_IN_StartingFromRight ((uint8_T)5U)
#define ActionsModel_IN_Inactive       ((uint8_T)1U)
#define ActionsModel_IN_Monitoring     ((uint8_T)1U)
#define ActionsModel_IN_MovedFromLeft  ((uint8_T)2U)
#define ActionsModel_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define ActionsModel_OBSTACLE_DETECTED ((uint16_T)300U)
#define ActionsModel_OFF               ((uint8_T)0U)
#define ActionsModel_ON                ((uint8_T)1U)

/* Named constants for Chart: '<Root>/RoverAction' */
#define Action_IN_BW_FW_EndedSafeAction ((uint8_T)1U)
#define Action_IN_BW_RL_EndedSafeAction ((uint8_T)1U)
#define Action_IN_BW_RR_EndedSafeAction ((uint8_T)1U)
#define ActionsM_IN_BW_FW_BrakingSmooth ((uint8_T)2U)
#define ActionsM_IN_BW_FW_SL_RotateLeft ((uint8_T)3U)
#define ActionsM_IN_BW_FW_SL_StopMotors ((uint8_T)4U)
#define ActionsM_IN_BW_FW_SR_StopMotors ((uint8_T)6U)
#define ActionsM_MIN_DISTANCE_TO_ROTATE ((uint16_T)20U)
#define ActionsMo_IN_FW_EndedSafeAction ((uint8_T)1U)
#define ActionsMo_IN_RL_EndedSafeAction ((uint8_T)1U)
#define ActionsMo_IN_RR_EndedSafeAction ((uint8_T)1U)
#define ActionsMod_IN_BW_FW_BrakingHard ((uint8_T)1U)
#define ActionsMod_IN_BW_RR_RotateRight ((uint8_T)2U)
#define ActionsMod_IN_FW_SR_RotateRight ((uint8_T)5U)
#define ActionsMod_IN_NO_ACTIVE_CHILD_k ((uint8_T)0U)
#define ActionsMode_IN_BW_FW_SafeAction ((uint8_T)2U)
#define ActionsMode_IN_BW_FW_UserAction ((uint8_T)3U)
#define ActionsMode_IN_BW_RL_SafeAction ((uint8_T)2U)
#define ActionsMode_IN_BW_RL_UserAction ((uint8_T)3U)
#define ActionsMode_IN_BW_RR_SafeAction ((uint8_T)2U)
#define ActionsMode_IN_BW_RR_UserAction ((uint8_T)3U)
#define ActionsMode_IN_FW_BrakingSmooth ((uint8_T)2U)
#define ActionsMode_IN_FW_SL_RotateLeft ((uint8_T)3U)
#define ActionsMode_IN_FW_SL_StopMotors ((uint8_T)4U)
#define ActionsMode_IN_FW_SR_StopMotors ((uint8_T)6U)
#define ActionsMode_OBSTACLE_DETECTED_h ((uint16_T)300U)
#define ActionsModel_BACKWARD_DEGREE   (180.0F)
#define ActionsModel_BRAKE_DISTANCE    ((uint16_T)150U)
#define ActionsModel_CRITICAL_DISTANCE ((uint16_T)75U)
#define ActionsModel_IN_BW_Forward     ((uint8_T)1U)
#define ActionsModel_IN_BW_RotateLeft  ((uint8_T)3U)
#define ActionsModel_IN_BW_StopMotors1 ((uint8_T)4U)
#define ActionsModel_IN_BW_StopMotors2 ((uint8_T)5U)
#define ActionsModel_IN_Backward       ((uint8_T)1U)
#define ActionsModel_IN_BrakingHard    ((uint8_T)2U)
#define ActionsModel_IN_BrakingSmooth  ((uint8_T)3U)
#define ActionsModel_IN_FW_BrakingHard ((uint8_T)1U)
#define ActionsModel_IN_FW_SafeAction  ((uint8_T)2U)
#define ActionsModel_IN_FW_UserAction  ((uint8_T)3U)
#define ActionsModel_IN_Forward        ((uint8_T)4U)
#define ActionsModel_IN_None           ((uint8_T)5U)
#define ActionsModel_IN_RL_RotateLeft  ((uint8_T)1U)
#define ActionsModel_IN_RL_SafeAction  ((uint8_T)2U)
#define ActionsModel_IN_RL_StopMotors  ((uint8_T)2U)
#define ActionsModel_IN_RL_UserAction  ((uint8_T)3U)
#define ActionsModel_IN_RR_RotateRight ((uint8_T)1U)
#define ActionsModel_IN_RR_SafeAction  ((uint8_T)2U)
#define ActionsModel_IN_RR_StopMotors  ((uint8_T)2U)
#define ActionsModel_IN_RR_UserAction  ((uint8_T)3U)
#define ActionsModel_IN_RotateLeft     ((uint8_T)6U)
#define ActionsModel_IN_RotateRight    ((uint8_T)7U)
#define ActionsModel_MAX_SPEED_X       (80.0F)
#define ActionsModel_MAX_SPEED_Y       (100.0F)
#define ActionsModel_NO_SPEED          (10.0F)
#define ActionsModel_SPEED_SWERVE      (40.0F)
#define ActionsModel_SWERVE_DEGREE     (45.0F)
#define Actions_IN_BW_FW_SR_RotateRight ((uint8_T)5U)

MdlrefDW_ActionsModel_T ActionsModel_MdlrefDW;

/* Block signals (default storage) */
B_ActionsModel_c_T ActionsModel_B;

/* Block states (default storage) */
DW_ActionsModel_f_T ActionsModel_DW;

/* Forward declaration for local functions */
static ENUM_SafeAction ActionsMo_checkSafetyFromRotate(uint16_T obstacleDetected);
static ENUM_SafeAction ActionsM_checkSafetyFromForward(ENUM_TrackingObstacles
  movingObstacle, uint16_T sonarFront);
static uint8_T ActionsModel_areAllSpeedsZero(real32_T speed1, real32_T speed2,
  real32_T speed3, real32_T speed4, real32_T zero_velocity);
static uint8_T Acti_trackGyroAngleChangeRobust(real32_T *accumulatedChange,
  real32_T previousValue, real32_T currentValue, real32_T threshold);
static void enter_internal_BW_FW_SafeAction(const real32_T *rtu_gyroscope,
  BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction);
static void ActionsModel_BW_Forward(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever, const real32_T
  *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint *rty_setPoint,
  ENUM_RoverAction *rty_roverAction, ENUM_SafeAction *rty_safeAction,
  ENUM_TrackingObstacles *rty_statusObstacles);
static void ActionsModel_BW_RotateLeft(const ENUM_UserAction
  *rtu_currentUserAction, const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever,
  const real32_T *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint
  *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction);
static void Ac_enter_internal_FW_SafeAction(const real32_T *rtu_gyroscope,
  BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction);
static void ActionsModel_Backward(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever, const real32_T
  *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint *rty_setPoint,
  ENUM_RoverAction *rty_roverAction, ENUM_SafeAction *rty_safeAction,
  ENUM_TrackingObstacles *rty_statusObstacles, BUS_RedLeds *rty_redLeds);
static void ActionsModel_Forward(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever, const real32_T
  *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint *rty_setPoint,
  ENUM_RoverAction *rty_roverAction, ENUM_SafeAction *rty_safeAction,
  ENUM_TrackingObstacles *rty_statusObstacles, BUS_RedLeds *rty_redLeds);
static void ActionsModel_RotateLeft(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_x_lever, const int16_T
  *rtu_y_lever, const real32_T *rtu_gyroscope, const BUS_Sonar *rtu_sonar,
  BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction, ENUM_TrackingObstacles *rty_statusObstacles, BUS_RedLeds
  *rty_redLeds);
static void ActionsModel_RotateRight(const ENUM_UserAction
  *rtu_currentUserAction, const BUS_Speed *rtu_speed, const int16_T *rtu_x_lever,
  const int16_T *rtu_y_lever, const real32_T *rtu_gyroscope, const BUS_Sonar
  *rtu_sonar, BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction,
  ENUM_SafeAction *rty_safeAction, ENUM_TrackingObstacles *rty_statusObstacles,
  BUS_RedLeds *rty_redLeds);

/* System initialize for function-call system: '<Root>/MovingObstacles' */
void ActionsMod_MovingObstacles_Init(ENUM_TrackingObstacles *rty_statusObstacles,
  DW_MovingObstacles_ActionsMod_T *localDW)
{
  /* Chart: '<Root>/MovingObstacles' */
  /*  Il chart deve attivarsi all'avvio, e il rover all'avvio sarà in IDLE
     [set == 0] */
  localDW->is_c2_ActionsModel = ActionsModel_IN_Inactive;
  *rty_statusObstacles = NOT_TRACKING;
}

/* Output and update for function-call system: '<Root>/MovingObstacles' */
void ActionsModel_MovingObstacles(uint8_T rtu_set, const BUS_Sonar *rtu_sonar,
  ENUM_TrackingObstacles *rty_statusObstacles, DW_MovingObstacles_ActionsMod_T
  *localDW)
{
  if (localDW->is_c2_ActionsModel == ActionsModel_IN_Inactive) {
    *rty_statusObstacles = NOT_TRACKING;
    if (rtu_set == ActionsModel_ON) {
      localDW->is_c2_ActionsModel = Acti_IN_TrackingMovingObstacles;
      if ((rtu_sonar->left <= ActionsModel_OBSTACLE_DETECTED) &&
          (rtu_sonar->front <= ActionsModel_OBSTACLE_DETECTED)) {
        localDW->is_TrackingMovingObstacles = Act_IN_PossibleStartingFromLeft;
        *rty_statusObstacles = NO_OBSTACLE;
      } else if ((rtu_sonar->right <= ActionsModel_OBSTACLE_DETECTED) &&
                 (rtu_sonar->front <= ActionsModel_OBSTACLE_DETECTED)) {
        localDW->is_TrackingMovingObstacles = Ac_IN_PossibleStartingFromRight;
        *rty_statusObstacles = NO_OBSTACLE;
      } else {
        localDW->is_TrackingMovingObstacles = ActionsModel_IN_Monitoring;
        *rty_statusObstacles = NO_OBSTACLE;
      }
    }

    /* case IN_TrackingMovingObstacles: */
  } else if (rtu_set == ActionsModel_OFF) {
    localDW->is_TrackingMovingObstacles = ActionsModel_IN_NO_ACTIVE_CHILD;
    localDW->is_c2_ActionsModel = ActionsModel_IN_Inactive;
    *rty_statusObstacles = NOT_TRACKING;
  } else {
    switch (localDW->is_TrackingMovingObstacles) {
     case ActionsModel_IN_Monitoring:
      *rty_statusObstacles = NO_OBSTACLE;
      if ((rtu_sonar->left <= ActionsModel_OBSTACLE_DETECTED) &&
          (rtu_sonar->front <= ActionsModel_OBSTACLE_DETECTED)) {
        localDW->is_TrackingMovingObstacles = Act_IN_PossibleStartingFromLeft;
        *rty_statusObstacles = NO_OBSTACLE;
      } else if ((rtu_sonar->right <= ActionsModel_OBSTACLE_DETECTED) &&
                 (rtu_sonar->front <= ActionsModel_OBSTACLE_DETECTED)) {
        localDW->is_TrackingMovingObstacles = Ac_IN_PossibleStartingFromRight;
        *rty_statusObstacles = NO_OBSTACLE;
      }
      break;

     case ActionsModel_IN_MovedFromLeft:
      *rty_statusObstacles = OBSTACLE_FROM_LEFT;
      if (rtu_sonar->front > ActionsModel_OBSTACLE_DETECTED) {
        localDW->is_TrackingMovingObstacles = ActionsModel_IN_Monitoring;
        *rty_statusObstacles = NO_OBSTACLE;
      }
      break;

     case Act_IN_PossibleStartingFromLeft:
      *rty_statusObstacles = NO_OBSTACLE;
      if ((rtu_sonar->left > ActionsModel_OBSTACLE_DETECTED) &&
          (rtu_sonar->front <= ActionsModel_OBSTACLE_DETECTED)) {
        localDW->is_TrackingMovingObstacles = ActionsModel_IN_MovedFromLeft;
        *rty_statusObstacles = OBSTACLE_FROM_LEFT;
      } else if (rtu_sonar->front > ActionsModel_OBSTACLE_DETECTED) {
        localDW->is_TrackingMovingObstacles = ActionsModel_IN_Monitoring;
        *rty_statusObstacles = NO_OBSTACLE;
      }
      break;

     case Ac_IN_PossibleStartingFromRight:
      *rty_statusObstacles = NO_OBSTACLE;
      if ((rtu_sonar->right > ActionsModel_OBSTACLE_DETECTED) &&
          (rtu_sonar->front <= ActionsModel_OBSTACLE_DETECTED)) {
        localDW->is_TrackingMovingObstacles = ActionsMod_IN_StartingFromRight;
        *rty_statusObstacles = OBSTACLE_FROM_RIGHT;
      } else if (rtu_sonar->front > ActionsModel_OBSTACLE_DETECTED) {
        localDW->is_TrackingMovingObstacles = ActionsModel_IN_Monitoring;
        *rty_statusObstacles = NO_OBSTACLE;
      }
      break;

     default:
      /* case IN_StartingFromRight: */
      *rty_statusObstacles = OBSTACLE_FROM_RIGHT;
      if (rtu_sonar->front > ActionsModel_OBSTACLE_DETECTED) {
        localDW->is_TrackingMovingObstacles = ActionsModel_IN_Monitoring;
        *rty_statusObstacles = NO_OBSTACLE;
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static ENUM_SafeAction ActionsMo_checkSafetyFromRotate(uint16_T obstacleDetected)
{
  ENUM_SafeAction emergencyAction;
  emergencyAction = SA_NONE;
  if (obstacleDetected <= ActionsM_MIN_DISTANCE_TO_ROTATE) {
    emergencyAction = SA_BRAKING_HARD;
  }

  return emergencyAction;
}

/* Function for Chart: '<Root>/RoverAction' */
static ENUM_SafeAction ActionsM_checkSafetyFromForward(ENUM_TrackingObstacles
  movingObstacle, uint16_T sonarFront)
{
  ENUM_SafeAction emergencyAction;
  emergencyAction = SA_NONE;
  if (sonarFront <= ActionsMode_OBSTACLE_DETECTED_h) {
    if (sonarFront <= ActionsModel_BRAKE_DISTANCE) {
      if (sonarFront <= ActionsModel_CRITICAL_DISTANCE) {
        emergencyAction = SA_BRAKING_HARD;
      } else {
        emergencyAction = SA_BRAKING_SMOOTH;
      }
    } else {
      switch (movingObstacle) {
       case OBSTACLE_FROM_LEFT:
        emergencyAction = SA_SWERVE_LEFT;
        break;

       case OBSTACLE_FROM_RIGHT:
        emergencyAction = SA_SWERVE_RIGHT;
        break;
      }
    }
  }

  return emergencyAction;
}

/* Function for Chart: '<Root>/RoverAction' */
static uint8_T ActionsModel_areAllSpeedsZero(real32_T speed1, real32_T speed2,
  real32_T speed3, real32_T speed4, real32_T zero_velocity)
{
  return (uint8_T)((fabsf(speed1) <= zero_velocity) && ((fabsf(speed2) <=
    zero_velocity) && ((fabsf(speed3) <= zero_velocity) && (fabsf(speed4) <=
    zero_velocity))));
}

/* Function for Chart: '<Root>/RoverAction' */
static uint8_T Acti_trackGyroAngleChangeRobust(real32_T *accumulatedChange,
  real32_T previousValue, real32_T currentValue, real32_T threshold)
{
  uint8_T hasChanged;
  if (rtIsInfF(previousValue) || rtIsNaNF(previousValue)) {
    hasChanged = 0U;
  } else if (rtIsInfF(currentValue) || rtIsNaNF(currentValue)) {
    hasChanged = 0U;
  } else if (rtIsInfF(threshold) || rtIsNaNF(threshold)) {
    hasChanged = 0U;
  } else {
    real32_T deltaRaw;
    deltaRaw = currentValue - previousValue;
    if (deltaRaw > 180.0F) {
      deltaRaw -= 360.0F;
    } else if (deltaRaw < -180.0F) {
      deltaRaw += 360.0F;
    }

    *accumulatedChange += deltaRaw;
    hasChanged = (uint8_T)(fabsf(*accumulatedChange) >= fabsf(threshold));
  }

  return hasChanged;
}

/* Function for Chart: '<Root>/RoverAction' */
static void enter_internal_BW_FW_SafeAction(const real32_T *rtu_gyroscope,
  BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction)
{
  switch (*rty_safeAction) {
   case SA_BRAKING_HARD:
    ActionsModel_DW.is_BW_FW_SafeAction = ActionsMod_IN_BW_FW_BrakingHard;
    *rty_roverAction = RA_BRAKING_HARD;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;

   case SA_BRAKING_SMOOTH:
    ActionsModel_DW.is_BW_FW_SafeAction = ActionsM_IN_BW_FW_BrakingSmooth;
    *rty_roverAction = RA_BRAKING_SMOOTH;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;

   case SA_SWERVE_RIGHT:
    ActionsModel_DW.accumulated_change = 0.0F;
    ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
    ActionsModel_DW.is_BW_FW_SafeAction = ActionsM_IN_BW_FW_SR_StopMotors;
    *rty_roverAction = RA_BRAKING_HARD;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;

   default:
    /* [safeAction == ENUM_SafeAction.SA_SWERVE_LEFT] */
    ActionsModel_DW.accumulated_change = 0.0F;
    ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
    ActionsModel_DW.is_BW_FW_SafeAction = ActionsM_IN_BW_FW_SL_StopMotors;
    *rty_roverAction = RA_BRAKING_HARD;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static void ActionsModel_BW_Forward(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever, const real32_T
  *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint *rty_setPoint,
  ENUM_RoverAction *rty_roverAction, ENUM_SafeAction *rty_safeAction,
  ENUM_TrackingObstacles *rty_statusObstacles)
{
  uint8_T rotation_ended;
  *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
    rtu_sonar->front);
  switch (ActionsModel_DW.is_BW_Forward) {
   case Action_IN_BW_FW_EndedSafeAction:
    if (*rtu_currentUserAction != UA_BACKWARD) {
      ActionsModel_DW.is_BW_Forward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_BW_Forward = 2U;
    } else if (*rty_safeAction == SA_NONE) {
      /* l'utente vorrebbe continuare ad andare avanti e non c'è più l'ostacolo */
      ActionsModel_DW.is_BW_Forward = ActionsMode_IN_BW_FW_UserAction;

      /*  Start tracking */
      ActionsModel_B.set = 1U;

      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
        &ActionsModel_DW.sf_MovingObstacles);

      /*  Action */
      *rty_roverAction = RA_FORWARD;

      /*  Set point */
      rty_setPoint->rightAxis = (real32_T)-*rtu_y_lever / 512.0F *
        ActionsModel_MAX_SPEED_Y;
      rty_setPoint->leftAxis = rty_setPoint->rightAxis;
    }
    break;

   case ActionsMode_IN_BW_FW_SafeAction:
    switch (ActionsModel_DW.is_BW_FW_SafeAction) {
     case ActionsMod_IN_BW_FW_BrakingHard:
      /*     */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_BW_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_FW_SafeActio = 2U;
      }
      break;

     case ActionsM_IN_BW_FW_BrakingSmooth:
      /*     */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_BW_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_FW_SafeActio = 2U;
      } else if (*rty_safeAction == SA_BRAKING_HARD) {
        ActionsModel_DW.is_BW_FW_SafeAction = ActionsMod_IN_BW_FW_BrakingHard;
        *rty_roverAction = RA_BRAKING_HARD;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      }
      break;

     case ActionsM_IN_BW_FW_SL_RotateLeft:
      rotation_ended = Acti_trackGyroAngleChangeRobust
        (&ActionsModel_DW.accumulated_change, ActionsModel_DW.previousGyroscope,
         *rtu_gyroscope, ActionsModel_SWERVE_DEGREE);
      if (rotation_ended == 1) {
        ActionsModel_DW.is_BW_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_FW_SafeActio = 2U;
      } else {
        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
      }
      break;

     case ActionsM_IN_BW_FW_SL_StopMotors:
      /*     */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_BW_FW_SafeAction = ActionsM_IN_BW_FW_SL_RotateLeft;
        *rty_roverAction = RA_ROTATE_LEFT;

        /*  Set point */
        rty_setPoint->rightAxis = ActionsModel_SPEED_SWERVE;
        rty_setPoint->leftAxis = -40.0F;

        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
      }
      break;

     case Actions_IN_BW_FW_SR_RotateRight:
      rotation_ended = Acti_trackGyroAngleChangeRobust
        (&ActionsModel_DW.accumulated_change, ActionsModel_DW.previousGyroscope,
         *rtu_gyroscope, ActionsModel_SWERVE_DEGREE);
      if (rotation_ended == 1) {
        ActionsModel_DW.is_BW_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_FW_SafeActio = 2U;
      } else {
        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
      }
      break;

     default:
      /* case IN_BW_FW_SR_StopMotors: */
      /*     */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_BW_FW_SafeAction = Actions_IN_BW_FW_SR_RotateRight;
        *rty_roverAction = RA_ROTATE_RIGHT;

        /*  Set point */
        rty_setPoint->rightAxis = -40.0F;
        rty_setPoint->leftAxis = ActionsModel_SPEED_SWERVE;

        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
      }
      break;
    }

    if (ActionsModel_DW.exit_port_index_BW_FW_SafeActio == 2U) {
      ActionsModel_DW.exit_port_index_BW_FW_SafeActio = 0U;
      if (*rtu_currentUserAction != UA_BACKWARD) {
        ActionsModel_DW.is_BW_Forward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_Forward = 2U;
      } else if (*rty_safeAction == SA_NONE) {
        /* l'utente vorrebbe continuare ad andare avanti e non c'è più l'ostacolo */
        ActionsModel_DW.is_BW_Forward = ActionsMode_IN_BW_FW_UserAction;

        /*  Start tracking */
        ActionsModel_B.set = 1U;

        /* Chart: '<Root>/MovingObstacles' */
        /* Chart: '<Root>/MovingObstacles' */
        ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
          &ActionsModel_DW.sf_MovingObstacles);

        /*  Action */
        *rty_roverAction = RA_FORWARD;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)-*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_Y;
        rty_setPoint->leftAxis = rty_setPoint->rightAxis;
      } else {
        ActionsModel_DW.is_BW_Forward = Action_IN_BW_FW_EndedSafeAction;
        *rty_roverAction = RA_IDLE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      }
    }
    break;

   default:
    /* case IN_BW_FW_UserAction: */
    if (*rty_safeAction != SA_NONE) {
      /*  Stop tracking */
      ActionsModel_B.set = 0U;

      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      ActionsModel_MovingObstacles(0, rtu_sonar, rty_statusObstacles,
        &ActionsModel_DW.sf_MovingObstacles);
      ActionsModel_DW.is_BW_Forward = ActionsMode_IN_BW_FW_SafeAction;
      enter_internal_BW_FW_SafeAction(rtu_gyroscope, rty_setPoint,
        rty_roverAction, rty_safeAction);
    } else if (*rtu_currentUserAction != UA_BACKWARD) {
      /*  Stop tracking */
      ActionsModel_B.set = 0U;

      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      ActionsModel_MovingObstacles(0, rtu_sonar, rty_statusObstacles,
        &ActionsModel_DW.sf_MovingObstacles);
      ActionsModel_DW.is_BW_Forward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_BW_Forward = 2U;
    } else {
      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      /*  Keep tracking */
      ActionsModel_MovingObstacles(ActionsModel_B.set, rtu_sonar,
        rty_statusObstacles, &ActionsModel_DW.sf_MovingObstacles);

      /*  Action */
      *rty_roverAction = RA_FORWARD;

      /*  Set point */
      rty_setPoint->rightAxis = (real32_T)-*rtu_y_lever / 512.0F *
        ActionsModel_MAX_SPEED_Y;
      rty_setPoint->leftAxis = rty_setPoint->rightAxis;
    }
    break;
  }

  if (ActionsModel_DW.exit_port_index_BW_Forward == 2U) {
    ActionsModel_DW.exit_port_index_BW_Forward = 0U;
    ActionsModel_DW.is_Backward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
    ActionsModel_DW.exit_port_index_Backward = 2U;
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static void ActionsModel_BW_RotateLeft(const ENUM_UserAction
  *rtu_currentUserAction, const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever,
  const real32_T *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint
  *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction)
{
  uint8_T rotation_ended;
  *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->left);
  switch (ActionsModel_DW.is_BW_RotateLeft) {
   case Action_IN_BW_RL_EndedSafeAction:
    if (*rtu_currentUserAction != UA_BACKWARD) {
      ActionsModel_DW.is_BW_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_BW_RotateLeft = 2U;
    } else if (*rty_safeAction == SA_NONE) {
      /* l'utente vorrebbe ruotare a SX e non c'è più l'ostacolo */
      ActionsModel_DW.is_BW_RotateLeft = ActionsMode_IN_BW_RL_UserAction;

      /*  Degrees */
      ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

      /*  Action */
      *rty_roverAction = RA_ROTATE_LEFT;

      /*  Set point */
      rty_setPoint->leftAxis = (real32_T)*rtu_y_lever / 512.0F *
        ActionsModel_MAX_SPEED_X;
      rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
    }
    break;

   case ActionsMode_IN_BW_RL_SafeAction:
    /*     */
    if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
         rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
      ActionsModel_DW.exit_port_index_BW_RL_SafeActio = 2U;
    }

    if (ActionsModel_DW.exit_port_index_BW_RL_SafeActio == 2U) {
      ActionsModel_DW.exit_port_index_BW_RL_SafeActio = 0U;
      if (*rtu_currentUserAction != UA_BACKWARD) {
        ActionsModel_DW.is_BW_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_RotateLeft = 2U;
      } else if (*rty_safeAction == SA_NONE) {
        /* l'utente vorrebbe ruotare a SX e non c'è più l'ostacolo */
        ActionsModel_DW.is_BW_RotateLeft = ActionsMode_IN_BW_RL_UserAction;

        /*  Degrees */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

        /*  Action */
        *rty_roverAction = RA_ROTATE_LEFT;

        /*  Set point */
        rty_setPoint->leftAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_X;
        rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
      } else {
        ActionsModel_DW.is_BW_RotateLeft = Action_IN_BW_RL_EndedSafeAction;
        *rty_roverAction = RA_IDLE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      }
    }
    break;

   default:
    /* case IN_BW_RL_UserAction: */
    if (*rty_safeAction != SA_NONE) {
      ActionsModel_DW.is_BW_RotateLeft = ActionsMode_IN_BW_RL_SafeAction;
      *rty_roverAction = RA_BRAKING_HARD;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
    } else if (*rtu_currentUserAction != UA_BACKWARD) {
      ActionsModel_DW.is_BW_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_BW_RotateLeft = 2U;
    } else {
      rotation_ended = Acti_trackGyroAngleChangeRobust
        (&ActionsModel_DW.accumulated_change, ActionsModel_DW.previousGyroscope,
         *rtu_gyroscope, ActionsModel_BACKWARD_DEGREE);
      if (rotation_ended == 1) {
        ActionsModel_DW.is_BW_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_RotateLeft = 3U;
      } else {
        /*  Degrees */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

        /*  Action */
        *rty_roverAction = RA_ROTATE_LEFT;

        /*  Set point */
        rty_setPoint->leftAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_X;
        rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
      }
    }
    break;
  }

  switch (ActionsModel_DW.exit_port_index_BW_RotateLeft) {
   case 2:
    ActionsModel_DW.exit_port_index_BW_RotateLeft = 0U;
    ActionsModel_DW.is_Backward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
    ActionsModel_DW.exit_port_index_Backward = 2U;
    break;

   case 3:
    ActionsModel_DW.exit_port_index_BW_RotateLeft = 0U;
    ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors2;

    /*  Actions */
    *rty_roverAction = RA_BRAKING_SMOOTH;
    *rty_safeAction = SA_NONE;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static void Ac_enter_internal_FW_SafeAction(const real32_T *rtu_gyroscope,
  BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction)
{
  switch (*rty_safeAction) {
   case SA_BRAKING_HARD:
    ActionsModel_DW.is_FW_SafeAction = ActionsModel_IN_FW_BrakingHard;
    *rty_roverAction = RA_BRAKING_HARD;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;

   case SA_BRAKING_SMOOTH:
    ActionsModel_DW.is_FW_SafeAction = ActionsMode_IN_FW_BrakingSmooth;
    *rty_roverAction = RA_BRAKING_SMOOTH;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;

   case SA_SWERVE_RIGHT:
    ActionsModel_DW.accumulated_change = 0.0F;
    ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
    ActionsModel_DW.is_FW_SafeAction = ActionsMode_IN_FW_SR_StopMotors;
    *rty_roverAction = RA_BRAKING_HARD;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;

   default:
    /* [safeAction == ENUM_SafeAction.SA_SWERVE_LEFT] */
    ActionsModel_DW.accumulated_change = 0.0F;
    ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
    ActionsModel_DW.is_FW_SafeAction = ActionsMode_IN_FW_SL_StopMotors;
    *rty_roverAction = RA_BRAKING_HARD;

    /*  Set point */
    rty_setPoint->rightAxis = 0.0F;
    rty_setPoint->leftAxis = 0.0F;
    break;
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static void ActionsModel_Backward(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever, const real32_T
  *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint *rty_setPoint,
  ENUM_RoverAction *rty_roverAction, ENUM_SafeAction *rty_safeAction,
  ENUM_TrackingObstacles *rty_statusObstacles, BUS_RedLeds *rty_redLeds)
{
  uint8_T rotation_ended;
  switch (ActionsModel_DW.is_Backward) {
   case ActionsModel_IN_BW_Forward:
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    ActionsModel_BW_Forward(rtu_currentUserAction, rtu_speed, rtu_y_lever,
      rtu_gyroscope, rtu_sonar, rty_setPoint, rty_roverAction, rty_safeAction,
      rty_statusObstacles);

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    break;

   case ActionsMod_IN_BW_RR_RotateRight:
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->right);

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    switch (ActionsModel_DW.is_BW_RR_RotateRight) {
     case Action_IN_BW_RR_EndedSafeAction:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      if (*rtu_currentUserAction != UA_BACKWARD) {
        ActionsModel_DW.is_BW_RR_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_RR_RotateRig = 2U;
      } else if (*rty_safeAction == SA_NONE) {
        /* l'utente vorrebbe continuare a ruotare a DX e non c'è più l'ostacolo */
        ActionsModel_DW.is_BW_RR_RotateRight = ActionsMode_IN_BW_RR_UserAction;

        /*  Degrees */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

        /*  Action */
        *rty_roverAction = RA_ROTATE_RIGHT;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_X;
        rty_setPoint->leftAxis = -rty_setPoint->rightAxis;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;

     case ActionsMode_IN_BW_RR_SafeAction:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /*     */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.exit_port_index_BW_RR_SafeActio = 2U;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      if (ActionsModel_DW.exit_port_index_BW_RR_SafeActio == 2U) {
        ActionsModel_DW.exit_port_index_BW_RR_SafeActio = 0U;

        /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
         *  Chart: '<Root>/RoverAction'
         */
        /* Chart: '<Root>/RoverAction' */
        if (*rtu_currentUserAction != UA_BACKWARD) {
          ActionsModel_DW.is_BW_RR_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
          ActionsModel_DW.exit_port_index_BW_RR_RotateRig = 2U;
        } else if (*rty_safeAction == SA_NONE) {
          /* l'utente vorrebbe continuare a ruotare a DX e non c'è più l'ostacolo */
          ActionsModel_DW.is_BW_RR_RotateRight = ActionsMode_IN_BW_RR_UserAction;

          /*  Degrees */
          ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

          /*  Action */
          *rty_roverAction = RA_ROTATE_RIGHT;

          /*  Set point */
          rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
            ActionsModel_MAX_SPEED_X;
          rty_setPoint->leftAxis = -rty_setPoint->rightAxis;
        } else {
          ActionsModel_DW.is_BW_RR_RotateRight = Action_IN_BW_RR_EndedSafeAction;
          *rty_roverAction = RA_IDLE;

          /*  Set point */
          rty_setPoint->rightAxis = 0.0F;
          rty_setPoint->leftAxis = 0.0F;
        }

        /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      }
      break;

     default:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /* case IN_BW_RR_UserAction: */
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_BW_RR_RotateRight = ActionsMode_IN_BW_RR_SafeAction;
        *rty_roverAction = RA_BRAKING_HARD;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      } else if (*rtu_currentUserAction != UA_BACKWARD) {
        ActionsModel_DW.is_BW_RR_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_BW_RR_RotateRig = 2U;
      } else {
        rotation_ended = Acti_trackGyroAngleChangeRobust
          (&ActionsModel_DW.accumulated_change,
           ActionsModel_DW.previousGyroscope, *rtu_gyroscope,
           ActionsModel_BACKWARD_DEGREE);
        if (rotation_ended == 1) {
          ActionsModel_DW.is_BW_RR_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
          ActionsModel_DW.exit_port_index_BW_RR_RotateRig = 3U;
        } else {
          /*  Degrees */
          ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

          /*  Action */
          *rty_roverAction = RA_ROTATE_RIGHT;

          /*  Set point */
          rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
            ActionsModel_MAX_SPEED_X;
          rty_setPoint->leftAxis = -rty_setPoint->rightAxis;
        }
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;
    }

    switch (ActionsModel_DW.exit_port_index_BW_RR_RotateRig) {
     case 2:
      ActionsModel_DW.exit_port_index_BW_RR_RotateRig = 0U;
      ActionsModel_DW.is_Backward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_Backward = 2U;
      break;

     case 3:
      ActionsModel_DW.exit_port_index_BW_RR_RotateRig = 0U;
      ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors2;

      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;
    }
    break;

   case ActionsModel_IN_BW_RotateLeft:
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    ActionsModel_BW_RotateLeft(rtu_currentUserAction, rtu_speed, rtu_y_lever,
      rtu_gyroscope, rtu_sonar, rty_setPoint, rty_roverAction, rty_safeAction);

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    break;

   case ActionsModel_IN_BW_StopMotors1:
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    if (*rtu_currentUserAction != UA_BACKWARD) {
      ActionsModel_DW.is_Backward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_Backward = 2U;

      /*     */
    } else if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1,
                rtu_speed->motor2, rtu_speed->motor3, rtu_speed->motor4,
                ActionsModel_NO_SPEED) != 0) {
      if (rtu_sonar->left > ActionsM_MIN_DISTANCE_TO_ROTATE) {
        ActionsModel_DW.is_Backward = ActionsModel_IN_BW_RotateLeft;
        *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->left);

        /*  I valori per la rotazione vanno resettati nel Superstate
           Se interrompessi la user action per eseguire la safe action, al mio ritorno non devo perdere gli accumulated change del delta  */
        ActionsModel_DW.accumulated_change = 0.0F;
        if (*rty_safeAction != SA_NONE) {
          ActionsModel_DW.is_BW_RotateLeft = ActionsMode_IN_BW_RL_SafeAction;
          *rty_roverAction = RA_BRAKING_HARD;

          /*  Set point */
          rty_setPoint->rightAxis = 0.0F;
          rty_setPoint->leftAxis = 0.0F;
        } else {
          /* [safeAction == ENUM_SafeAction.SA_NONE] */
          ActionsModel_DW.is_BW_RotateLeft = ActionsMode_IN_BW_RL_UserAction;

          /*  Degrees */
          ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

          /*  Action */
          *rty_roverAction = RA_ROTATE_LEFT;

          /*  Set point */
          rty_setPoint->leftAxis = (real32_T)*rtu_y_lever / 512.0F *
            ActionsModel_MAX_SPEED_X;
          rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
        }
      } else if (rtu_sonar->right > ActionsM_MIN_DISTANCE_TO_ROTATE) {
        ActionsModel_DW.is_Backward = ActionsMod_IN_BW_RR_RotateRight;
        *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->right);

        /*  I valori per la rotazione vanno resettati nel Superstate
           Se interrompessi la user action per eseguire la safe action, al mio ritorno non devo perdere gli accumulated change del delta  */
        ActionsModel_DW.accumulated_change = 0.0F;
        if (*rty_safeAction != SA_NONE) {
          ActionsModel_DW.is_BW_RR_RotateRight = ActionsMode_IN_BW_RR_SafeAction;
          *rty_roverAction = RA_BRAKING_HARD;

          /*  Set point */
          rty_setPoint->rightAxis = 0.0F;
          rty_setPoint->leftAxis = 0.0F;
        } else {
          /* [safeAction == ENUM_SafeAction.SA_NONE] */
          ActionsModel_DW.is_BW_RR_RotateRight = ActionsMode_IN_BW_RR_UserAction;

          /*  Degrees */
          ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

          /*  Action */
          *rty_roverAction = RA_ROTATE_RIGHT;

          /*  Set point */
          rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
            ActionsModel_MAX_SPEED_X;
          rty_setPoint->leftAxis = -rty_setPoint->rightAxis;
        }
      }
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    break;

   default:
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    /* case IN_BW_StopMotors2: */
    if (*rtu_currentUserAction != UA_BACKWARD) {
      ActionsModel_DW.is_Backward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_Backward = 2U;

      /*      */
    } else if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1,
                rtu_speed->motor2, rtu_speed->motor3, rtu_speed->motor4,
                ActionsModel_NO_SPEED) != 0) {
      ActionsModel_DW.is_Backward = ActionsModel_IN_BW_Forward;
      *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
        rtu_sonar->front);
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_BW_Forward = ActionsMode_IN_BW_FW_SafeAction;
        enter_internal_BW_FW_SafeAction(rtu_gyroscope, rty_setPoint,
          rty_roverAction, rty_safeAction);
      } else {
        /*  [safeAction == ENUM_SafeAction.SA_NONE] */
        ActionsModel_DW.is_BW_Forward = ActionsMode_IN_BW_FW_UserAction;

        /*  Start tracking */
        ActionsModel_B.set = 1U;

        /* Chart: '<Root>/MovingObstacles' */
        /* Chart: '<Root>/MovingObstacles' */
        ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
          &ActionsModel_DW.sf_MovingObstacles);

        /*  Action */
        *rty_roverAction = RA_FORWARD;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)-*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_Y;
        rty_setPoint->leftAxis = rty_setPoint->rightAxis;
      }
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    break;
  }

  if (ActionsModel_DW.exit_port_index_Backward == 2U) {
    ActionsModel_DW.exit_port_index_Backward = 0U;

    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    switch (*rtu_currentUserAction) {
     case UA_BACKWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Backward;
      ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors1;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_RIGHT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateRight;
      ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_LEFT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateLeft;
      ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_FORWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Forward;
      *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
        rtu_sonar->front);
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
        Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
          rty_roverAction, rty_safeAction);
      } else {
        /*  [safeAction == ENUM_SafeAction.SA_NONE] */
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

        /*  Start tracking */
        ActionsModel_B.set = 1U;

        /* Chart: '<Root>/MovingObstacles' */
        /* Chart: '<Root>/MovingObstacles' */
        ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
          &ActionsModel_DW.sf_MovingObstacles);

        /*  Action */
        *rty_roverAction = RA_FORWARD;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_Y;
        rty_setPoint->leftAxis = rty_setPoint->rightAxis;
      }
      break;

     case UA_BRAKING_SMOOTH:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingSmooth;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_BRAKING_HARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingHard;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_HARD;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     default:
      /*  Transizione incondizionata siccome le exit junction hanno bisogno di uno stato garantito in cui entrare
         [currentUserAction == ENUM_UserAction.UA_NONE] */
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_None;

      /*  Actions */
      *rty_roverAction = RA_IDLE;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;

      /*  Led */
      rty_redLeds->left = RED_OFF;
      rty_redLeds->right = RED_OFF;
      break;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static void ActionsModel_Forward(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_y_lever, const real32_T
  *rtu_gyroscope, const BUS_Sonar *rtu_sonar, BUS_SetPoint *rty_setPoint,
  ENUM_RoverAction *rty_roverAction, ENUM_SafeAction *rty_safeAction,
  ENUM_TrackingObstacles *rty_statusObstacles, BUS_RedLeds *rty_redLeds)
{
  uint8_T rotation_ended;

  /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
   *  Chart: '<Root>/RoverAction'
   */
  /* Chart: '<Root>/RoverAction' */
  *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
    rtu_sonar->front);

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
  switch (ActionsModel_DW.is_Forward) {
   case ActionsMo_IN_FW_EndedSafeAction:
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    if (*rtu_currentUserAction != UA_FORWARD) {
      ActionsModel_DW.is_Forward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_Forward = 2U;
    } else if (*rty_safeAction == SA_NONE) {
      /* l'utente vorrebbe continuare ad andare avanti e non c'è più l'ostacolo */
      ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

      /*  Start tracking */
      ActionsModel_B.set = 1U;

      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
        &ActionsModel_DW.sf_MovingObstacles);

      /*  Action */
      *rty_roverAction = RA_FORWARD;

      /*  Set point */
      rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
        ActionsModel_MAX_SPEED_Y;
      rty_setPoint->leftAxis = rty_setPoint->rightAxis;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    break;

   case ActionsModel_IN_FW_SafeAction:
    switch (ActionsModel_DW.is_FW_SafeAction) {
     case ActionsModel_IN_FW_BrakingHard:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_FW_SafeAction = 2U;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;

     case ActionsMode_IN_FW_BrakingSmooth:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_FW_SafeAction = 2U;
      } else if (*rty_safeAction == SA_BRAKING_HARD) {
        ActionsModel_DW.is_FW_SafeAction = ActionsModel_IN_FW_BrakingHard;
        *rty_roverAction = RA_BRAKING_HARD;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;

     case ActionsMode_IN_FW_SL_RotateLeft:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      rotation_ended = Acti_trackGyroAngleChangeRobust
        (&ActionsModel_DW.accumulated_change, ActionsModel_DW.previousGyroscope,
         *rtu_gyroscope, ActionsModel_SWERVE_DEGREE);

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      if (rotation_ended == 1) {
        ActionsModel_DW.is_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_FW_SafeAction = 2U;
      } else {
        /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
         *  Chart: '<Root>/RoverAction'
         */
        /* Chart: '<Root>/RoverAction' */
        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

        /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      }
      break;

     case ActionsMode_IN_FW_SL_StopMotors:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_FW_SafeAction = ActionsMode_IN_FW_SL_RotateLeft;
        *rty_roverAction = RA_ROTATE_LEFT;

        /*  Set point */
        rty_setPoint->rightAxis = ActionsModel_SPEED_SWERVE;
        rty_setPoint->leftAxis = -40.0F;

        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;

     case ActionsMod_IN_FW_SR_RotateRight:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      rotation_ended = Acti_trackGyroAngleChangeRobust
        (&ActionsModel_DW.accumulated_change, ActionsModel_DW.previousGyroscope,
         *rtu_gyroscope, ActionsModel_SWERVE_DEGREE);

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      if (rotation_ended == 1) {
        ActionsModel_DW.is_FW_SafeAction = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_FW_SafeAction = 2U;
      } else {
        /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
         *  Chart: '<Root>/RoverAction'
         */
        /* Chart: '<Root>/RoverAction' */
        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;

        /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      }
      break;

     default:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /* case IN_FW_SR_StopMotors: */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.is_FW_SafeAction = ActionsMod_IN_FW_SR_RotateRight;
        *rty_roverAction = RA_ROTATE_RIGHT;

        /*  Set point */
        rty_setPoint->rightAxis = -40.0F;
        rty_setPoint->leftAxis = ActionsModel_SPEED_SWERVE;

        /*  Degree */
        ActionsModel_DW.previousGyroscope = *rtu_gyroscope;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;
    }

    if (ActionsModel_DW.exit_port_index_FW_SafeAction == 2U) {
      ActionsModel_DW.exit_port_index_FW_SafeAction = 0U;

      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      if (*rtu_currentUserAction != UA_FORWARD) {
        ActionsModel_DW.is_Forward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_Forward = 2U;
      } else if (*rty_safeAction == SA_NONE) {
        /* l'utente vorrebbe continuare ad andare avanti e non c'è più l'ostacolo */
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

        /*  Start tracking */
        ActionsModel_B.set = 1U;

        /* Chart: '<Root>/MovingObstacles' */
        /* Chart: '<Root>/MovingObstacles' */
        ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
          &ActionsModel_DW.sf_MovingObstacles);

        /*  Action */
        *rty_roverAction = RA_FORWARD;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_Y;
        rty_setPoint->leftAxis = rty_setPoint->rightAxis;
      } else {
        ActionsModel_DW.is_Forward = ActionsMo_IN_FW_EndedSafeAction;
        *rty_roverAction = RA_IDLE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    }
    break;

   default:
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    /* case IN_FW_UserAction: */
    if (*rty_safeAction != SA_NONE) {
      /*  Stop tracking */
      ActionsModel_B.set = 0U;

      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      ActionsModel_MovingObstacles(0, rtu_sonar, rty_statusObstacles,
        &ActionsModel_DW.sf_MovingObstacles);
      ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
      Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
        rty_roverAction, rty_safeAction);
    } else if (*rtu_currentUserAction != UA_FORWARD) {
      /*  Stop tracking */
      ActionsModel_B.set = 0U;

      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      ActionsModel_MovingObstacles(0, rtu_sonar, rty_statusObstacles,
        &ActionsModel_DW.sf_MovingObstacles);
      ActionsModel_DW.is_Forward = ActionsMod_IN_NO_ACTIVE_CHILD_k;
      ActionsModel_DW.exit_port_index_Forward = 2U;
    } else {
      /* Chart: '<Root>/MovingObstacles' */
      /* Chart: '<Root>/MovingObstacles' */
      /*  Keep tracking */
      ActionsModel_MovingObstacles(ActionsModel_B.set, rtu_sonar,
        rty_statusObstacles, &ActionsModel_DW.sf_MovingObstacles);

      /*  Action */
      *rty_roverAction = RA_FORWARD;

      /*  Set point */
      rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
        ActionsModel_MAX_SPEED_Y;
      rty_setPoint->leftAxis = rty_setPoint->rightAxis;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    break;
  }

  if (ActionsModel_DW.exit_port_index_Forward == 2U) {
    ActionsModel_DW.exit_port_index_Forward = 0U;

    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    switch (*rtu_currentUserAction) {
     case UA_BACKWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Backward;
      ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors1;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_RIGHT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateRight;
      ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_LEFT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateLeft;
      ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_FORWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Forward;
      *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
        rtu_sonar->front);
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
        Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
          rty_roverAction, rty_safeAction);
      } else {
        /*  [safeAction == ENUM_SafeAction.SA_NONE] */
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

        /*  Start tracking */
        ActionsModel_B.set = 1U;

        /* Chart: '<Root>/MovingObstacles' */
        /* Chart: '<Root>/MovingObstacles' */
        ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
          &ActionsModel_DW.sf_MovingObstacles);

        /*  Action */
        *rty_roverAction = RA_FORWARD;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_Y;
        rty_setPoint->leftAxis = rty_setPoint->rightAxis;
      }
      break;

     case UA_BRAKING_SMOOTH:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingSmooth;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_BRAKING_HARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingHard;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_HARD;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     default:
      /*  Transizione incondizionata siccome le exit junction hanno bisogno di uno stato garantito in cui entrare
         [currentUserAction == ENUM_UserAction.UA_NONE] */
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_None;

      /*  Actions */
      *rty_roverAction = RA_IDLE;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;

      /*  Led */
      rty_redLeds->left = RED_OFF;
      rty_redLeds->right = RED_OFF;
      break;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static void ActionsModel_RotateLeft(const ENUM_UserAction *rtu_currentUserAction,
  const BUS_Speed *rtu_speed, const int16_T *rtu_x_lever, const int16_T
  *rtu_y_lever, const real32_T *rtu_gyroscope, const BUS_Sonar *rtu_sonar,
  BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction, ENUM_SafeAction
  *rty_safeAction, ENUM_TrackingObstacles *rty_statusObstacles, BUS_RedLeds
  *rty_redLeds)
{
  if (ActionsModel_DW.is_RotateLeft == ActionsModel_IN_RL_RotateLeft) {
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->left);

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    switch (ActionsModel_DW.is_RL_RotateLeft) {
     case ActionsMo_IN_RL_EndedSafeAction:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      if (*rtu_currentUserAction != UA_ROTATE_LEFT) {
        ActionsModel_DW.is_RL_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.is_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_RotateLeft = 2U;
      } else if (*rty_safeAction == SA_NONE) {
        /* l'utente vorrebbe continuare a ruotare a SX e non c'è più l'ostacolo */
        ActionsModel_DW.is_RL_RotateLeft = ActionsModel_IN_RL_UserAction;
        *rty_roverAction = RA_ROTATE_LEFT;

        /*  Set point */
        rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
          ActionsModel_MAX_SPEED_X;
        rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;

     case ActionsModel_IN_RL_SafeAction:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /*     */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.exit_port_index_RL_SafeAction = 2U;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      if (ActionsModel_DW.exit_port_index_RL_SafeAction == 2U) {
        ActionsModel_DW.exit_port_index_RL_SafeAction = 0U;

        /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
         *  Chart: '<Root>/RoverAction'
         */
        /* Chart: '<Root>/RoverAction' */
        if (*rtu_currentUserAction != UA_ROTATE_LEFT) {
          ActionsModel_DW.is_RL_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
          ActionsModel_DW.is_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
          ActionsModel_DW.exit_port_index_RotateLeft = 2U;
        } else if (*rty_safeAction == SA_NONE) {
          /* l'utente vorrebbe continuare a ruotare a SX e non c'è più l'ostacolo */
          ActionsModel_DW.is_RL_RotateLeft = ActionsModel_IN_RL_UserAction;
          *rty_roverAction = RA_ROTATE_LEFT;

          /*  Set point */
          rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
            ActionsModel_MAX_SPEED_X;
          rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
        } else {
          ActionsModel_DW.is_RL_RotateLeft = ActionsMo_IN_RL_EndedSafeAction;
          *rty_roverAction = RA_IDLE;

          /*  Set point */
          rty_setPoint->rightAxis = 0.0F;
          rty_setPoint->leftAxis = 0.0F;
        }

        /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      }
      break;

     default:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /* case IN_RL_UserAction: */
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_RL_RotateLeft = ActionsModel_IN_RL_SafeAction;
        *rty_roverAction = RA_BRAKING_HARD;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      } else if (*rtu_currentUserAction != UA_ROTATE_LEFT) {
        ActionsModel_DW.is_RL_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.is_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_RotateLeft = 2U;
      } else {
        *rty_roverAction = RA_ROTATE_LEFT;

        /*  Set point */
        rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
          ActionsModel_MAX_SPEED_X;
        rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;
    }

    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    /* case IN_RL_StopMotors: */
  } else if (*rtu_currentUserAction != UA_ROTATE_LEFT) {
    ActionsModel_DW.is_RotateLeft = ActionsMod_IN_NO_ACTIVE_CHILD_k;
    ActionsModel_DW.exit_port_index_RotateLeft = 2U;
  } else if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
              rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0)
  {
    ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_RotateLeft;
    *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->left);
    if (*rty_safeAction != SA_NONE) {
      ActionsModel_DW.is_RL_RotateLeft = ActionsModel_IN_RL_SafeAction;
      *rty_roverAction = RA_BRAKING_HARD;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
    } else {
      /* [safeAction == ENUM_SafeAction.SA_NONE] */
      ActionsModel_DW.is_RL_RotateLeft = ActionsModel_IN_RL_UserAction;
      *rty_roverAction = RA_ROTATE_LEFT;

      /*  Set point */
      rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
        ActionsModel_MAX_SPEED_X;
      rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
  }

  if (ActionsModel_DW.exit_port_index_RotateLeft == 2U) {
    ActionsModel_DW.exit_port_index_RotateLeft = 0U;

    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    switch (*rtu_currentUserAction) {
     case UA_BACKWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Backward;
      ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors1;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_RIGHT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateRight;
      ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_LEFT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateLeft;
      ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_FORWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Forward;
      *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
        rtu_sonar->front);
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
        Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
          rty_roverAction, rty_safeAction);
      } else {
        /*  [safeAction == ENUM_SafeAction.SA_NONE] */
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

        /*  Start tracking */
        ActionsModel_B.set = 1U;

        /* Chart: '<Root>/MovingObstacles' */
        /* Chart: '<Root>/MovingObstacles' */
        ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
          &ActionsModel_DW.sf_MovingObstacles);

        /*  Action */
        *rty_roverAction = RA_FORWARD;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_Y;
        rty_setPoint->leftAxis = rty_setPoint->rightAxis;
      }
      break;

     case UA_BRAKING_SMOOTH:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingSmooth;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_BRAKING_HARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingHard;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_HARD;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     default:
      /*  Transizione incondizionata siccome le exit junction hanno bisogno di uno stato garantito in cui entrare
         [currentUserAction == ENUM_UserAction.UA_NONE] */
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_None;

      /*  Actions */
      *rty_roverAction = RA_IDLE;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;

      /*  Led */
      rty_redLeds->left = RED_OFF;
      rty_redLeds->right = RED_OFF;
      break;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
  }
}

/* Function for Chart: '<Root>/RoverAction' */
static void ActionsModel_RotateRight(const ENUM_UserAction
  *rtu_currentUserAction, const BUS_Speed *rtu_speed, const int16_T *rtu_x_lever,
  const int16_T *rtu_y_lever, const real32_T *rtu_gyroscope, const BUS_Sonar
  *rtu_sonar, BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction,
  ENUM_SafeAction *rty_safeAction, ENUM_TrackingObstacles *rty_statusObstacles,
  BUS_RedLeds *rty_redLeds)
{
  if (ActionsModel_DW.is_RotateRight == ActionsModel_IN_RR_RotateRight) {
    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->right);

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
    switch (ActionsModel_DW.is_RR_RotateRight) {
     case ActionsMo_IN_RR_EndedSafeAction:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      if (*rtu_currentUserAction != UA_ROTATE_RIGHT) {
        ActionsModel_DW.is_RR_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.is_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_RotateRight = 2U;
      } else if (*rty_safeAction == SA_NONE) {
        /* l'utente vorrebbe continuare a ruotare a DX e non c'è più l'ostacolo */
        ActionsModel_DW.is_RR_RotateRight = ActionsModel_IN_RR_UserAction;
        *rty_roverAction = RA_ROTATE_RIGHT;

        /*  Set point */
        rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
          ActionsModel_MAX_SPEED_X;
        rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;

     case ActionsModel_IN_RR_SafeAction:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /*     */
      if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
           rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0) {
        ActionsModel_DW.exit_port_index_RR_SafeAction = 2U;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      if (ActionsModel_DW.exit_port_index_RR_SafeAction == 2U) {
        ActionsModel_DW.exit_port_index_RR_SafeAction = 0U;

        /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
         *  Chart: '<Root>/RoverAction'
         */
        /* Chart: '<Root>/RoverAction' */
        if (*rtu_currentUserAction != UA_ROTATE_RIGHT) {
          ActionsModel_DW.is_RR_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
          ActionsModel_DW.is_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
          ActionsModel_DW.exit_port_index_RotateRight = 2U;
        } else if (*rty_safeAction == SA_NONE) {
          /* l'utente vorrebbe continuare a ruotare a DX e non c'è più l'ostacolo */
          ActionsModel_DW.is_RR_RotateRight = ActionsModel_IN_RR_UserAction;
          *rty_roverAction = RA_ROTATE_RIGHT;

          /*  Set point */
          rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
            ActionsModel_MAX_SPEED_X;
          rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
        } else {
          ActionsModel_DW.is_RR_RotateRight = ActionsMo_IN_RR_EndedSafeAction;
          *rty_roverAction = RA_IDLE;

          /*  Set point */
          rty_setPoint->rightAxis = 0.0F;
          rty_setPoint->leftAxis = 0.0F;
        }

        /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      }
      break;

     default:
      /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
       *  Chart: '<Root>/RoverAction'
       */
      /* Chart: '<Root>/RoverAction' */
      /* case IN_RR_UserAction: */
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_RR_RotateRight = ActionsModel_IN_RR_SafeAction;
        *rty_roverAction = RA_BRAKING_HARD;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
      } else if (*rtu_currentUserAction != UA_ROTATE_RIGHT) {
        ActionsModel_DW.is_RR_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.is_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
        ActionsModel_DW.exit_port_index_RotateRight = 2U;
      } else {
        *rty_roverAction = RA_ROTATE_RIGHT;

        /*  Set point */
        rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
          ActionsModel_MAX_SPEED_X;
        rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
      }

      /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
      break;
    }

    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    /* case IN_RR_StopMotors: */
  } else if (*rtu_currentUserAction != UA_ROTATE_RIGHT) {
    ActionsModel_DW.is_RotateRight = ActionsMod_IN_NO_ACTIVE_CHILD_k;
    ActionsModel_DW.exit_port_index_RotateRight = 2U;

    /*     */
  } else if (ActionsModel_areAllSpeedsZero(rtu_speed->motor1, rtu_speed->motor2,
              rtu_speed->motor3, rtu_speed->motor4, ActionsModel_NO_SPEED) != 0)
  {
    ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_RotateRight;
    *rty_safeAction = ActionsMo_checkSafetyFromRotate(rtu_sonar->right);
    if (*rty_safeAction != SA_NONE) {
      ActionsModel_DW.is_RR_RotateRight = ActionsModel_IN_RR_SafeAction;
      *rty_roverAction = RA_BRAKING_HARD;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
    } else {
      /* [safeAction == ENUM_SafeAction.SA_NONE] */
      ActionsModel_DW.is_RR_RotateRight = ActionsModel_IN_RR_UserAction;
      *rty_roverAction = RA_ROTATE_RIGHT;

      /*  Set point */
      rty_setPoint->leftAxis = (real32_T)*rtu_x_lever / 512.0F *
        ActionsModel_MAX_SPEED_X;
      rty_setPoint->rightAxis = -rty_setPoint->leftAxis;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
  }

  if (ActionsModel_DW.exit_port_index_RotateRight == 2U) {
    ActionsModel_DW.exit_port_index_RotateRight = 0U;

    /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
     *  Chart: '<Root>/RoverAction'
     */
    /* Chart: '<Root>/RoverAction' */
    switch (*rtu_currentUserAction) {
     case UA_BACKWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Backward;
      ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors1;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_RIGHT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateRight;
      ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_ROTATE_LEFT:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateLeft;
      ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_StopMotors;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_FORWARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Forward;
      *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
        rtu_sonar->front);
      if (*rty_safeAction != SA_NONE) {
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
        Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
          rty_roverAction, rty_safeAction);
      } else {
        /*  [safeAction == ENUM_SafeAction.SA_NONE] */
        ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

        /*  Start tracking */
        ActionsModel_B.set = 1U;

        /* Chart: '<Root>/MovingObstacles' */
        /* Chart: '<Root>/MovingObstacles' */
        ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
          &ActionsModel_DW.sf_MovingObstacles);

        /*  Action */
        *rty_roverAction = RA_FORWARD;

        /*  Set point */
        rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
          ActionsModel_MAX_SPEED_Y;
        rty_setPoint->leftAxis = rty_setPoint->rightAxis;
      }
      break;

     case UA_BRAKING_SMOOTH:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingSmooth;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_SMOOTH;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     case UA_BRAKING_HARD:
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingHard;

      /*  Actions */
      *rty_roverAction = RA_BRAKING_HARD;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;
      break;

     default:
      /*  Transizione incondizionata siccome le exit junction hanno bisogno di uno stato garantito in cui entrare
         [currentUserAction == ENUM_UserAction.UA_NONE] */
      ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_None;

      /*  Actions */
      *rty_roverAction = RA_IDLE;
      *rty_safeAction = SA_NONE;

      /*  Set point */
      rty_setPoint->rightAxis = 0.0F;
      rty_setPoint->leftAxis = 0.0F;

      /*  Led */
      rty_redLeds->left = RED_OFF;
      rty_redLeds->right = RED_OFF;
      break;
    }

    /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
  }
}

/* System initialize for referenced model: 'ActionsModel' */
void ActionsModel_Init(BUS_SetPoint *rty_setPoint, ENUM_TrackingObstacles
  *rty_statusObstacles, BUS_RedLeds *rty_redLeds)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
   *  Chart: '<Root>/RoverAction'
   */
  /* Chart: '<Root>/RoverAction' */
  /*  Outputs initialization */
  /*  Superstate inutile? Forse con lo stato degradato ha senso se questo chart cambia stato */
  ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_None;

  /*  Actions */
  /*  Set point */
  rty_setPoint->rightAxis = 0.0F;
  rty_setPoint->leftAxis = 0.0F;

  /*  Led */
  rty_redLeds->left = RED_OFF;
  rty_redLeds->right = RED_OFF;

  /* SystemInitialize for Chart: '<Root>/RoverAction' incorporates:
   *  Chart: '<Root>/MovingObstacles'
   */
  /* SystemInitialize for Chart: '<Root>/MovingObstacles' */
  ActionsMod_MovingObstacles_Init(rty_statusObstacles,
    &ActionsModel_DW.sf_MovingObstacles);

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
}

/* Output and update for referenced model: 'ActionsModel' */
void ActionsModel_ComputeRoverAction(const ENUM_UserAction
  *rtu_currentUserAction, const BUS_Speed *rtu_speed, const int16_T *rtu_x_lever,
  const int16_T *rtu_y_lever, const real32_T *rtu_gyroscope, const BUS_Sonar
  *rtu_sonar, BUS_SetPoint *rty_setPoint, ENUM_RoverAction *rty_roverAction,
  ENUM_SafeAction *rty_safeAction, ENUM_TrackingObstacles *rty_statusObstacles,
  BUS_RedLeds *rty_redLeds)
{
  /* RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' incorporates:
   *  Chart: '<Root>/RoverAction'
   */
  /* Chart: '<Root>/RoverAction' */
  switch (ActionsModel_DW.is_CurrentRoverAction) {
   case ActionsModel_IN_Backward:
    ActionsModel_Backward(rtu_currentUserAction, rtu_speed, rtu_y_lever,
                          rtu_gyroscope, rtu_sonar, rty_setPoint,
                          rty_roverAction, rty_safeAction, rty_statusObstacles,
                          rty_redLeds);
    break;

   case ActionsModel_IN_BrakingHard:
    if (*rtu_currentUserAction != UA_BRAKING_HARD) {
      switch (*rtu_currentUserAction) {
       case UA_BACKWARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Backward;
        ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors1;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_ROTATE_RIGHT:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateRight;
        ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_StopMotors;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_ROTATE_LEFT:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateLeft;
        ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_StopMotors;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_FORWARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Forward;
        *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
          rtu_sonar->front);
        if (*rty_safeAction != SA_NONE) {
          ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
          Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
            rty_roverAction, rty_safeAction);
        } else {
          /*  [safeAction == ENUM_SafeAction.SA_NONE] */
          ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

          /*  Start tracking */
          ActionsModel_B.set = 1U;

          /* Chart: '<Root>/MovingObstacles' */
          /* Chart: '<Root>/MovingObstacles' */
          ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
            &ActionsModel_DW.sf_MovingObstacles);

          /*  Action */
          *rty_roverAction = RA_FORWARD;

          /*  Set point */
          rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
            ActionsModel_MAX_SPEED_Y;
          rty_setPoint->leftAxis = rty_setPoint->rightAxis;
        }
        break;

       case UA_BRAKING_SMOOTH:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingSmooth;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_BRAKING_HARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingHard;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_HARD;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       default:
        /*  Transizione incondizionata siccome le exit junction hanno bisogno di uno stato garantito in cui entrare
           [currentUserAction == ENUM_UserAction.UA_NONE] */
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_None;

        /*  Actions */
        *rty_roverAction = RA_IDLE;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;

        /*  Led */
        rty_redLeds->left = RED_OFF;
        rty_redLeds->right = RED_OFF;
        break;
      }
    }
    break;

   case ActionsModel_IN_BrakingSmooth:
    /*  [currentUserAction ~= ENUM_UserAction.UA_BRAKING_SMOOTH && currentUserAction ~= ENUM_UserAction.UA_NONE] */
    if (*rtu_currentUserAction != UA_BRAKING_SMOOTH) {
      switch (*rtu_currentUserAction) {
       case UA_BACKWARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Backward;
        ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors1;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_ROTATE_RIGHT:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateRight;
        ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_StopMotors;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_ROTATE_LEFT:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateLeft;
        ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_StopMotors;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_FORWARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Forward;
        *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
          rtu_sonar->front);
        if (*rty_safeAction != SA_NONE) {
          ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
          Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
            rty_roverAction, rty_safeAction);
        } else {
          /*  [safeAction == ENUM_SafeAction.SA_NONE] */
          ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

          /*  Start tracking */
          ActionsModel_B.set = 1U;

          /* Chart: '<Root>/MovingObstacles' */
          /* Chart: '<Root>/MovingObstacles' */
          ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
            &ActionsModel_DW.sf_MovingObstacles);

          /*  Action */
          *rty_roverAction = RA_FORWARD;

          /*  Set point */
          rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
            ActionsModel_MAX_SPEED_Y;
          rty_setPoint->leftAxis = rty_setPoint->rightAxis;
        }
        break;

       case UA_BRAKING_HARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingHard;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_HARD;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       default:
        /*  Transizione incondizionata siccome le exit junction hanno bisogno di uno stato garantito in cui entrare
           [currentUserAction == ENUM_UserAction.UA_NONE] */
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_None;

        /*  Actions */
        *rty_roverAction = RA_IDLE;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;

        /*  Led */
        rty_redLeds->left = RED_OFF;
        rty_redLeds->right = RED_OFF;
        break;
      }
    }
    break;

   case ActionsModel_IN_Forward:
    ActionsModel_Forward(rtu_currentUserAction, rtu_speed, rtu_y_lever,
                         rtu_gyroscope, rtu_sonar, rty_setPoint, rty_roverAction,
                         rty_safeAction, rty_statusObstacles, rty_redLeds);
    break;

   case ActionsModel_IN_None:
    if (*rtu_currentUserAction != UA_NONE) {
      switch (*rtu_currentUserAction) {
       case UA_BACKWARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Backward;
        ActionsModel_DW.is_Backward = ActionsModel_IN_BW_StopMotors1;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_ROTATE_RIGHT:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateRight;
        ActionsModel_DW.is_RotateRight = ActionsModel_IN_RR_StopMotors;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_ROTATE_LEFT:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_RotateLeft;
        ActionsModel_DW.is_RotateLeft = ActionsModel_IN_RL_StopMotors;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       case UA_FORWARD:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_Forward;
        *rty_safeAction = ActionsM_checkSafetyFromForward(*rty_statusObstacles,
          rtu_sonar->front);
        if (*rty_safeAction != SA_NONE) {
          ActionsModel_DW.is_Forward = ActionsModel_IN_FW_SafeAction;
          Ac_enter_internal_FW_SafeAction(rtu_gyroscope, rty_setPoint,
            rty_roverAction, rty_safeAction);
        } else {
          /*  [safeAction == ENUM_SafeAction.SA_NONE] */
          ActionsModel_DW.is_Forward = ActionsModel_IN_FW_UserAction;

          /*  Start tracking */
          ActionsModel_B.set = 1U;

          /* Chart: '<Root>/MovingObstacles' */
          /* Chart: '<Root>/MovingObstacles' */
          ActionsModel_MovingObstacles(1, rtu_sonar, rty_statusObstacles,
            &ActionsModel_DW.sf_MovingObstacles);

          /*  Action */
          *rty_roverAction = RA_FORWARD;

          /*  Set point */
          rty_setPoint->rightAxis = (real32_T)*rtu_y_lever / 512.0F *
            ActionsModel_MAX_SPEED_Y;
          rty_setPoint->leftAxis = rty_setPoint->rightAxis;
        }
        break;

       case UA_BRAKING_SMOOTH:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingSmooth;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_SMOOTH;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;

       default:
        ActionsModel_DW.is_CurrentRoverAction = ActionsModel_IN_BrakingHard;

        /*  Actions */
        *rty_roverAction = RA_BRAKING_HARD;
        *rty_safeAction = SA_NONE;

        /*  Set point */
        rty_setPoint->rightAxis = 0.0F;
        rty_setPoint->leftAxis = 0.0F;
        break;
      }
    }
    break;

   case ActionsModel_IN_RotateLeft:
    ActionsModel_RotateLeft(rtu_currentUserAction, rtu_speed, rtu_x_lever,
      rtu_y_lever, rtu_gyroscope, rtu_sonar, rty_setPoint, rty_roverAction,
      rty_safeAction, rty_statusObstacles, rty_redLeds);
    break;

   default:
    /* case IN_RotateRight: */
    ActionsModel_RotateRight(rtu_currentUserAction, rtu_speed, rtu_x_lever,
      rtu_y_lever, rtu_gyroscope, rtu_sonar, rty_setPoint, rty_roverAction,
      rty_safeAction, rty_statusObstacles, rty_redLeds);
    break;
  }

  /* End of Chart: '<Root>/RoverAction' */
  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ComputeRoverAction' */
}

/* Model initialize function */
void ActionsModel_initialize(const char_T **rt_errorStatus)
{
  RT_MODEL_ActionsModel_T *const ActionsModel_M = &(ActionsModel_MdlrefDW.rtm);

  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatusPointer(ActionsModel_M, rt_errorStatus);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
