#ifndef INC_BUS_DECISION_ENUMS_H_
#define INC_BUS_DECISION_ENUMS_H_

/* =========================
 * ENUM_Actuator
 * Descrive quale board/attuatore è attivo
 * ========================= */
typedef enum {
    BOARD1 = 0,
    BOARD2 = 1
} ENUM_Actuator;

/* =========================
 * ENUM_StatusRover
 * ========================= */
typedef enum {
    NORMAL = 0,               // Nessun FAULT, nessun ERROR

    ERROR_B1,                 // B1 in ERROR, B2 OK
    ERROR_B2,                 // B2 in ERROR, B1 OK
    ERROR_BOTH,               // B1 e B2 in ERROR

    FAULTY_B1_DEGRADED_B2,    // B1 FAULT -> rover faulty, B2 degradata/depotenziata
    FAULTY_B2_DEGRADED_B1,    // B2 FAULT -> rover faulty, B1 degradata/depotenziata

    EMERGENCY                // B1 FAULT, B2 FAULT
} ENUM_StatusRover;

/* =========================
 * ENUM_UserAction
 * Azione richiesta dall'utente
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

/* =========================
 * ENUM_RoverAction
 * Azione eseguita dal rover
 * ========================= */
typedef enum {
    RA_IDLE = 0,
    RA_FORWARD = 1,
    RA_ROTATE_LEFT = 2,
    RA_ROTATE_RIGHT = 3,
    RA_BRAKING_SMOOTH = 4,
    RA_BRAKING_MODERATE = 5,
    RA_BRAKING_HARD = 6
} ENUM_RoverAction;

/* =========================
 * ENUM_SafeAction
 * Azione di sicurezza
 * ========================= */
typedef enum {
    SA_NONE = 0,
    SA_SWERVE_LEFT = 1,
    SA_SWERVE_RIGHT = 2,
    SA_BRAKING_HARD = 3,
    SA_BRAKING_SMOOTH = 4
} ENUM_SafeAction;

/* =========================
 * ENUM_StatusRedLed
 * Stato del LED rosso
 * ========================= */
typedef enum {
    RED_OFF = 0,
    RED_BLINKING = 1,
    RED_ON = 2
} ENUM_StatusRedLed;

/* =========================
 * ENUM_StatusWhiteLed
 * Stato del LED bianco
 * ========================= */
typedef enum {
    WHITE_OFF = 0,
    WHITE_ON = 1
} ENUM_StatusWhiteLed;

#endif /* INC_BUS_DECISION_ENUMS_H_ */
