#ifndef INC_BUS_STATES_H_
#define INC_BUS_STATES_H_

#include "sensors.h"

/* LocalStateB1 */
typedef struct {
    BUS_Speed        speed;
    Temperature  temperature;
    BatteryLevel batteryLevel;
} BUS_LocalStateB1;

/* LocalStateB2 */
typedef struct {
    BUS_Sonar            sonar;
    Gyroscope        gyroscope;
    BUS_RemoteController remoteController;
} BUS_LocalStateB2;

/* GlobalState */
typedef struct {
    BUS_LocalStateB1 localStateB1;
    BUS_LocalStateB2 localStateB2;
} BUS_GlobalState;

#endif /* INC_BUS_STATES_H_ */
