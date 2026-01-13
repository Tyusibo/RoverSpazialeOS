#ifndef INC_DECISION_H_
#define INC_DECISION_H_

#include "enums.h"

/* SetPoint */
typedef struct {
    float leftAxis;
    float rightAxis;
} BUS_SetPoint;

/* Leds */
typedef struct {
    ENUM_StatusRedLed   left;
    ENUM_StatusRedLed   right;
} BUS_RedLeds;

typedef struct {
    ENUM_StatusWhiteLed left;
    ENUM_StatusWhiteLed right;
} BUS_WhiteLeds;

typedef struct {
    BUS_WhiteLeds white;
    BUS_RedLeds red;
} BUS_Leds;

/* Decision */
typedef struct {
	ENUM_Actuator   actuator;
    ENUM_UserAction userAction;
    ENUM_RoverAction roverAction;
    ENUM_SafeAction safeAction;
    BUS_SetPoint setPoint;
    BUS_Leds leds;
} BUS_Decision;

#endif /* INC_DECISION_H_ */