#ifndef INC_DRIVER_INIT_SONAR_INIT_H_
#define INC_DRIVER_INIT_SONAR_INIT_H_

#include "HCSR04.h"
#include "sonar_constants.h"

extern hcsr04_t sonarLeft;
extern hcsr04_t sonarFront;
extern hcsr04_t sonarRight;

uint8_t Sonar_InitAll(void);

void StartSonarTimer(void);

uint8_t all_sonar_done(void);

#endif /* INC_DRIVER_INIT_SONAR_INIT_H_ */
