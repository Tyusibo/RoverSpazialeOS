#ifndef INC_TEST_SONAR_TEST_H_
#define INC_TEST_SONAR_TEST_H_

#include "sonar_init.h"


// Esegue una lettura sequenziale (Left -> Wait -> Front -> Wait -> Right -> Wait)
void testSonarSeq(void);

// Esegue una lettura parallela (Trigger All -> Wait All)
void testSonarParal(void);

// Loop infinito di test sonar
void SonarTest(uint8_t mode);

#endif /* INC_TEST_SONAR_TEST_H_ */
