#ifndef INC_TEST_WHAT2TEST_H_
#define INC_TEST_WHAT2TEST_H_


/* -------------------------------------------------- */
/* Cosa testare                                       */
/* -------------------------------------------------- */
typedef enum
{
    TEST_LUCI = 0,
    TEST_MOTORI = 1,
    TEST_COMMUNICATION_NO_STOP = 2,
    TEST_COMMUNICATION_BUTTON_RESTART = 3,
    TEST_COMMUNICATION_ONE_RUN = 4,
	NONE = 5
} WhatToTest_t;

/* -------------------------------------------------- */
/* Selezione test attivo                              */
/* -------------------------------------------------- */
/* Cambia SOLO questo valore, flasha e testi altro */
static const WhatToTest_t WHAT_TO_TEST = NONE;

#endif /* INC_TEST_WHAT2TEST_H_ */
