#ifndef INC_BUS_ENUMS_H_
#define INC_BUS_ENUMS_H_

#include <stdint.h>

/* =========================
* ENUM_CoherenceErrors
* ========================= */
typedef enum {
    NO_INCOHERENCE = 0,

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
* ENUM_StatusRemoteController
* ========================= */
typedef enum {
    RC_STATUS_OK = 0,
    RC_STATUS_NOT_OK = 1
} ENUM_StatusRemoteController;

/* =========================
* ENUM_StatusTemperature
* ========================= */
typedef enum {
    TEMP_DATA_OK = 0,
    TEMP_DATA_ANOMALOUS = 1
} ENUM_StatusTemperature;

/* =========================
* ENUM_TrackingObstacles
* ========================= */
typedef enum {
    NOT_TRACKING = 0,
    NO_OBSTACLE_MOVING = 1,
    OBSTACLE_MOVING = 2
} ENUM_TrackingObstacles;

/* =========================
* ENUM_Error
* ========================= */
typedef enum {
    NO_ERROR = 0,   // Nessun errore rilevato
    ERROR_PRESENT  // Almeno un errore rilevato
} ENUM_Error;

/* =========================
* ENUM_BoardStatus
* ========================= */
typedef enum {
    BOARD_OK = 0,     // Board operativa, nessun problema
    BOARD_ERROR,     // Errore non critico
    BOARD_FAULT      // Guasto critico (board non affidabile / non operativa)
} ENUM_BoardStatus;

#endif /* INC_BUS_ENUMS_H_ */
