#ifndef INC_TEST_COMMUNICATION_TEST_H_
#define INC_TEST_COMMUNICATION_TEST_H_

#include <stdint.h>

typedef enum
{
    COMMUNICATION_NO_STOP = 0,
    COMMUNICATION_BUTTON_RESTART = 1,
    COMMUNICATION_ONE_RUN = 2
} CommunicationMode_t;

/**
 * @brief Inizializza il test comunicazione.
 *        - COMMUNICATION_NO_STOP: ogni ~1000ms fa toggle di continua (finché non premi il bottone per “fermarsi”).
 *        - COMMUNICATION_BUTTON_RESTART: il bottone fa toggle di continua (restart manuale).
 */
void CommunicationTest_Init(CommunicationMode_t mode);

/**
 * @brief Da chiamare nel while(1) dopo Board1_step().
 *        Gestisce stampa “una volta per ciclo” + avanzamento comunicazione secondo la modalità selezionata.
 */
void CommunicationTest_Task(void);

#endif /* INC_TEST_COMMUNICATION_TEST_H_ */
