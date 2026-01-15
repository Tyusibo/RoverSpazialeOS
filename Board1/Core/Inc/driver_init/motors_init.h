#ifndef INC_DRIVER_INIT_MOTORS_INIT_H_
#define INC_DRIVER_INIT_MOTORS_INIT_H_

#include <stdint.h>

#include "motor_control.h"
#include "motor_constants.h"


typedef enum {
    MOTOR_FRONT_LEFT = 0,
    MOTOR_FRONT_RIGHT,
    MOTOR_REAR_RIGHT,
    MOTOR_REAR_LEFT,
    N_MOTORS
} MotorId;

 extern MotorControl motors[N_MOTORS];


void Motors_InitAll(void);

void Motors_StartAllPwm(void);
void Motors_SetDefaultCcr(uint32_t ccr);

#endif /* INC_DRIVER_INIT_MOTORS_INIT_H_ */
