#ifndef INC_TEST_WHAT2TEST_H_
#define INC_TEST_WHAT2TEST_H_

/* -------------------------------------------------- */
/* Cosa testare                                       */
/* -------------------------------------------------- */
typedef enum {
	// Test sensori
	TEST_SONAR = 0,
	TEST_REMOTE_CONTROLLER = 1,
	TEST_GYROSCOPE = 2,

    TEST_COMMUNICATION_BYTE_RESTART_WITHOUT_SENSORS, // Avanza di uno step su ricezione byte (Dati Dummy)
    TEST_COMMUNICATION_BYTE_RESTART_WITH_SENSORS,    // Avanza di uno step su ricezione byte (Dati Reali)

    TEST_COMMUNICATION_BYTE_STOP_WITHOUT_SENSORS,    // Gira continuo, si ferma se riceve byte (Dati Dummy)
    TEST_COMMUNICATION_BYTE_STOP_WITH_SENSORS,       // Gira continuo, si ferma se riceve byte (Dati Reali)
    TEST_COMMUNICATION_NOSONAR                       // Dati reali (NO Sonar), Gira continuo

} TestSelection;

/* -------------------------------------------------- */
/* Selezione test attivo                              */
/* -------------------------------------------------- */
/* Cambia SOLO questo valore, flasha e testi altro */
#define WHAT_TO_TEST TEST_GYROSCOPE

#define TIME_TO_WAIT_BEFORE_RESTART 50

#endif /* INC_TEST_WHAT2TEST_H_ */
