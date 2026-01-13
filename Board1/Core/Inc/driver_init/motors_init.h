#ifndef INC_DRIVER_INIT_MOTORS_INIT_H_
#define INC_DRIVER_INIT_MOTORS_INIT_H_

#include <stdint.h>
#include "motor_control.h"
#include "motor_constants.h"

#define N_MOTORS 4

extern MotorControl motors[N_MOTORS];

void Motors_InitAll(void);

#endif /* INC_DRIVER_INIT_MOTORS_INIT_H_ */
