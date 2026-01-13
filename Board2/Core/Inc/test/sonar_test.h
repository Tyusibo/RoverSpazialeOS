#ifndef INC_TEST_SONAR_TEST_H_
#define INC_TEST_SONAR_TEST_H_

#include "sonar_init.h"

extern uint8_t dummyReceiveSonar;
extern uint8_t flagReceiveSonar;
// Loop infinito di test sonar (trigger -> wait done -> print -> delay 1s -> repeat)
void SonarTest(void);

#endif /* INC_TEST_SONAR_TEST_H_ */
