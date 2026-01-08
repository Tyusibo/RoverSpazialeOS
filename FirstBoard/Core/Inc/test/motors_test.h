#ifndef INC_TEST_MOTORS_TEST_H_
#define INC_TEST_MOTORS_TEST_H_

#include <stdint.h>


/**
 * Esegue la sequenza di test definita dentro motors_test.c.
 * Assumendo che:
 *  - Motors_InitAll() e StartMotorTimers() siano già stati chiamati
 *  - setPrinter(...) sia già stato impostato
 */
void MotorsTest_RunDefaultSequenceBlocking(void);



#endif /* INC_TEST_MOTORS_TEST_H_ */
