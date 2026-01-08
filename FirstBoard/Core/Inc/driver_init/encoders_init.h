#ifndef INC_DRIVER_INIT_ENCODERS_INIT_H_
#define INC_DRIVER_INIT_ENCODERS_INIT_H_

#include <stdint.h>
#include "encoder.h"
#include "motor_constants.h"   // per TS, COUNTS (se li hai lì dentro)

#define N_MOTORS 4
#define N_ENCODER (N_MOTORS)

extern Encoder encoders[N_ENCODER];

void Encoders_InitAll(void);
void Encoders_StartAll(void);


#endif /* INC_DRIVER_INIT_ENCODERS_INIT_H_ */
