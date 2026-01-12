#ifndef INC_TEST_LIGHTS_TEST_H_
#define INC_TEST_LIGHTS_TEST_H_

/* Test base dei LED (rosso/bianco, sinistra/destra) */
void led_test_set(void);     /* testa tutte le Set (ON poi OFF), ripetuto 2 volte */
void led_test_toggle(void);  /* testa tutti i Toggle, ripetuto 2 volte */
void led_test_all(void);     /* testa All_On e All_Off */

#endif /* INC_TEST_LIGHTS_TEST_H_ */
