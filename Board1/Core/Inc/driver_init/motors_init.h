#ifndef INC_DRIVER_INIT_MOTORS_INIT_H_
#define INC_DRIVER_INIT_MOTORS_INIT_H_

#include <stdint.h>
#include "motor_control.h"
#include "motor_constants.h"   // MIN_VOLT, MAX_VOLT, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX, TS...

#define N_MOTORS 4

extern MotorControl motors[N_MOTORS];

void Motors_InitAll(void);
void Motors_StartAllPwm(void);
void Motors_SetDefaultCcr(uint32_t ccr);

#endif /* INC_DRIVER_INIT_MOTORS_INIT_H_ */
