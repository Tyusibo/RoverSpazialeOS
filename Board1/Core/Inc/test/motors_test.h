#ifndef INC_TEST_MOTORS_TEST_H_
#define INC_TEST_MOTORS_TEST_H_

#include <stdint.h>

/**
 * Test in Open Loop:
 * Imposta il riferimento (SetReferenceRPM), chiama OpenLoopActuate
 * e poi stampa periodicamente le velocità misurate.
 */
void test_open_loop(float ref);

/**
 * Test in Closed Loop:
 * Esegue il loop di controllo (simulando Ts con HAL_Delay) finché
 * l'errore non scende sotto una soglia (o per un tempo limitato),
 * dopodiché smette di aggiornare la u (mantiene l'ultima) e stampa le velocità.
 */
void test_closed_loop(float ref);

#endif /* INC_TEST_MOTORS_TEST_H_ */
