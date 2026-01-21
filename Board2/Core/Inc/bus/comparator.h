#ifndef INC_BUS_COMPARATOR_H_
#define INC_BUS_COMPARATOR_H_

#include <stdint.h>
#include "decision.h"
#include "sensors.h"
#include "states.h"

/* sensors.h structs */
uint8_t BUS_Speed_Equals(const BUS_Speed *a, const BUS_Speed *b);
uint8_t BUS_Sonar_Equals(const BUS_Sonar *a, const BUS_Sonar *b);
uint8_t BUS_RemoteController_Equals(const BUS_RemoteController *a, const BUS_RemoteController *b);

/* states.h structs */
uint8_t BUS_LocalStateB1_Equals(const BUS_LocalStateB1 *a, const BUS_LocalStateB1 *b);
uint8_t BUS_LocalStateB2_Equals(const BUS_LocalStateB2 *a, const BUS_LocalStateB2 *b);
uint8_t BUS_GlobalState_Equals(const BUS_GlobalState *a, const BUS_GlobalState *b);

/* decision.h structs */
uint8_t BUS_SetPoint_Equals(const BUS_SetPoint *a, const BUS_SetPoint *b);
uint8_t BUS_RedLeds_Equals(const BUS_RedLeds *a, const BUS_RedLeds *b);
uint8_t BUS_WhiteLeds_Equals(const BUS_WhiteLeds *a, const BUS_WhiteLeds *b);
uint8_t BUS_Leds_Equals(const BUS_Leds *a, const BUS_Leds *b);
uint8_t BUS_Decision_Equals(const BUS_Decision *a, const BUS_Decision *b);

#endif /* INC_BUS_COMPARATOR_H_ */
