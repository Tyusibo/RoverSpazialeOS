#ifndef INC_DRIVER_INIT_SONAR_INIT_H_
#define INC_DRIVER_INIT_SONAR_INIT_H_

#include "HCSR04.h"

extern hcsr04_t sonarLeft;
extern hcsr04_t sonarFront;
extern hcsr04_t sonarRight;

void Sonar_InitAll(void);

void StartSonarTimer(void);

#endif /* INC_DRIVER_INIT_SONAR_INIT_H_ */
