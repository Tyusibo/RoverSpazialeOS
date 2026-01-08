#ifndef INC_TEST_WHAT2TEST_H_
#define INC_TEST_WHAT2TEST_H_

/* -------------------------------------------------- */
/* Cosa testare                                       */
/* -------------------------------------------------- */
typedef enum {
	TEST_SONAR = 0,
	TEST_REMOTE_CONTROLLER = 1,
	TEST_GYROSCOPE = 2,
	TEST_COMMUNICATION_NO_STOP = 3,
	TEST_COMMUNICATION_BUTTON_RESTART = 4,
	TEST_COMMUNICATION_ONE_RUN = 5,
	NONE = 6
} WhatToTest_t;

/* -------------------------------------------------- */
/* Selezione test attivo                              */
/* -------------------------------------------------- */
/* Cambia SOLO questo valore, flasha e testi altro */
static const WhatToTest_t WHAT_TO_TEST = TEST_REMOTE_CONTROLLER;

#endif /* INC_TEST_WHAT2TEST_H_ */
