#ifndef INC_BUS_DECISION_H_
#define INC_BUS_DECISION_H_

#include "decision_enums.h"

/* =========================
 * BUS_SetPoint
 * Setpoint per gli assi sinistro e destro
 * ========================= */
typedef struct {
    float leftAxis;
    float rightAxis;
} BUS_SetPoint;

/* =========================
 * BUS_RedLeds
 * Stato dei LED rossi
 * ========================= */
typedef struct {
    ENUM_StatusRedLed left;
    ENUM_StatusRedLed right;
} BUS_RedLeds;

/* =========================
 * BUS_WhiteLeds
 * Stato dei LED bianchi
 * ========================= */
typedef struct {
    ENUM_StatusWhiteLed left;
    ENUM_StatusWhiteLed right;
} BUS_WhiteLeds;

/* =========================
 * BUS_Leds
 * Stato complessivo dei LED
 * ========================= */
typedef struct {
    BUS_WhiteLeds white;
    BUS_RedLeds   red;
} BUS_Leds;

/* =========================
 * BUS_Decision
 * Struttura principale di decisione
 * ========================= */
typedef struct {
    ENUM_Actuator     actuator;
    ENUM_StatusRover  roverState;

    ENUM_UserAction   userAction;
    ENUM_RoverAction  roverAction;
    ENUM_SafeAction   safeAction;
    
    BUS_SetPoint      setPoint;
    BUS_Leds          leds;
} BUS_Decision;

#endif /* INC_BUS_DECISION_H_ */
