#ifndef ENUMS_H
#define ENUMS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* =========================
 * ENUM_Actuator
 * ========================= */
typedef enum {
    BOARD1 = 0,
    BOARD2 = 1
} ENUM_Actuator;

/* =========================
 * ENUM_CoherenceErrors
 * ========================= */
typedef enum {
    NO_ERROR = 0,

    /* The board received a global state where the previously received local state
       is not correctly represented */
    PARTNER_STATE_INCOHERENCE = 1,

    /* The board received a global state where its reported local state
       is not correctly represented */
    OWN_STATE_INCOHERENCE = 2,

    /* Both issues present */
    COMBO_ERROR = 3
} ENUM_CoherenceErrors;

/* =========================
 * ENUM_RoverAction
 * ========================= */
typedef enum {
    RA_IDLE = 0,
    RA_FORWARD = 1,
    RA_ROTATE_LEFT = 2,
    RA_ROTATE_RIGHT = 3,
    RA_BRAKING_HARD = 5,
    RA_BRAKING_SMOOTH = 6
} ENUM_RoverAction;

/* =========================
 * ENUM_SafeAction
 * ========================= */
typedef enum {
    SA_NONE = 0,
    SA_SWERVE_LEFT = 1,
    SA_SWERVE_RIGHT = 2,
    SA_BRAKING_HARD = 3,
    SA_BRAKING_SMOOTH = 4
} ENUM_SafeAction;

/* =========================
 * ENUM_StatusBattery
 * ========================= */
typedef enum {
    BATT_STATUS_OK = 0,
    BATT_STATUS_LOW_WARN = 1,
    BATT_STATUS_CRITICAL = 2
} ENUM_StatusBattery;

/* =========================
 * ENUM_StatusEncoder (bitmask)
 * ========================= */
typedef enum {
    STATUS_ALL_OK = 0,

    /* Single encoder failure */
    STATUS_E1_FAIL = 1,
    STATUS_E2_FAIL = 2,
    STATUS_E3_FAIL = 4,
    STATUS_E4_FAIL = 8,

    /* 2 encoder failures */
    STATUS_E1E2_FAIL = 3,
    STATUS_E1E3_FAIL = 5,
    STATUS_E1E4_FAIL = 9,
    STATUS_E2E3_FAIL = 6,
    STATUS_E2E4_FAIL = 10,
    STATUS_E3E4_FAIL = 12,

    /* 3 encoder failures */
    STATUS_E1E2E3_FAIL = 7,
    STATUS_E1E2E4_FAIL = 11,
    STATUS_E1E3E4_FAIL = 13,
    STATUS_E2E3E4_FAIL = 14,

    /* All encoders failure */
    STATUS_ALL_FAIL = 15
} ENUM_StatusEncoder;

/* =========================
 * ENUM_StatusRedLed
 * ========================= */
typedef enum {
    RED_OFF = 0,
    RED_BLINKING = 1,
    RED_ON = 2
} ENUM_StatusRedLed;

/* =========================
 * ENUM_StatusRemoteController
 * ========================= */
typedef enum {
    RC_STATUS_OK = 0,
    RC_STATUS_NOT_OK = 1
} ENUM_StatusRemoteController;

/* =========================
 * ENUM_StatusRover
 * ========================= */
typedef enum {
    NORMAL = 0,
    DEGRADED = 1
} ENUM_StatusRover;

/* =========================
 * ENUM_StatusTemperature
 * ========================= */
typedef enum {
    TEMP_DATA_OK = 0,
    TEMP_DATA_ANOMALOUS = 1
} ENUM_StatusTemperature;

/* =========================
 * ENUM_StatusWhiteLed
 * ========================= */
typedef enum {
    WHITE_OFF = 0,
    WHITE_ON = 1
} ENUM_StatusWhiteLed;

/* =========================
 * ENUM_TrackingObstacles
 * ========================= */
typedef enum {
    NOT_TRACKING = 0,
    NO_OBSTACLE = 1,
    OBSTACLE_FROM_LEFT = 2,
    OBSTACLE_FROM_RIGHT = 3
} ENUM_TrackingObstacles;

/* =========================
 * ENUM_UserAction
 * ========================= */
typedef enum {
    UA_NONE = 0,
    UA_FORWARD = 1,
    UA_ROTATE_LEFT = 2,
    UA_ROTATE_RIGHT = 3,
    UA_BACKWARD = 4,
    UA_BRAKING_HARD = 5,
    UA_BRAKING_SMOOTH = 6
} ENUM_UserAction;

#ifdef __cplusplus
}
#endif

#endif /* ENUMS_H */
