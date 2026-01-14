#ifndef INC_TEST_WHAT2TEST_H_
#define INC_TEST_WHAT2TEST_H_


/* -------------------------------------------------- */
/* Cosa testare                                       */
/* -------------------------------------------------- */
typedef enum {
    // Test sensori
    TEST_LUCI,          // Solo Board1
    TEST_MOTORI,        // Solo Board1

    TEST_COMMUNICATION_BYTE_RESTART_WITHOUT_SENSORS, // Avanza di uno step su ricezione byte (Dati Dummy)
    TEST_COMMUNICATION_BYTE_RESTART_WITH_SENSORS,    // Avanza di uno step su ricezione byte (Dati Reali)
    
    TEST_COMMUNICATION_BYTE_STOP_WITHOUT_SENSORS,    // Gira continuo, si ferma se riceve byte (Dati Dummy)
    TEST_COMMUNICATION_BYTE_STOP_WITH_SENSORS        // Gira continuo, si ferma se riceve byte (Dati Reali)

} TestSelection;

/* -------------------------------------------------- */
/* Selezione test attivo                              */
/* -------------------------------------------------- */
/* Cambia SOLO questo valore, flasha e testi altro */
#define WHAT_TO_TEST TEST_COMMUNICATION_BYTE_STOP_WITHOUT_SENSORS

#define TIME_TO_WAIT_BEFORE_RESTART 50

#endif /* INC_TEST_WHAT2TEST_H_ */
