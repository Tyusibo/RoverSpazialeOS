#include "communication_test.h"

#include "main.h"     // per UserButton_Pin e HAL_*
#include "Board1.h"   // tipi e stati del modello
#include "print.h"    // printGlobalState, printDecision, printMsg

/* Nel tuo progetto era 3U: lo mantengo. Se cambia, aggiorna qui. */
#define Board1_IN_Execution            ((uint8_T)3U)

/* ------------------ Stato interno del test ------------------ */

// Cambia nella init con quello assegnato
static volatile CommunicationMode_t s_mode = COMMUNICATION_NO_STOP;

/* stop automatico (mode NO_STOP) quando premi bottone */
static volatile uint8_t s_autoEnabled = 1;

/* per stampare una sola volta quando si entra in Execution */
static uint8_t s_prevIsExecution = 0;

/* timer non bloccante per auto-toggle */
static uint32_t s_nextAutoToggleTick = 0;

static void toggle_continua(void)
{
    Board1_U.continua = (Board1_U.continua == 0) ? 1 : 0;
}

/* ------------------ API ------------------ */
void CommunicationTest_Init(CommunicationMode_t mode)
{
    s_mode = mode;

    s_autoEnabled = 1;
    s_prevIsExecution = 0;

    /* pianifico subito il prossimo auto-toggle (se serve) */
    s_nextAutoToggleTick = HAL_GetTick() + 1000U;

    /* opzionale: metto continua in uno stato noto */
    Board1_U.continua = 0;
}

void CommunicationTest_Task(void)
{
    const uint8_t isExecution = (Board1_DW.is_ExchangeDecision == Board1_IN_Execution) ? 1U : 0U;

    /* Rising edge: sono appena entrato in Execution -> stampo UNA volta */
    if (isExecution && !s_prevIsExecution)
    {
        printGlobalState(&Board1_B.board1GlobalState);
        printDecision(&Board1_DW.board2Decision);

        /* resetto anche la “cadenza” dell’auto-toggle quando entro in attesa */
        s_nextAutoToggleTick = HAL_GetTick() + 1000U;
    }

    s_prevIsExecution = isExecution;

    /* Avanzamento comunicazione */
    if (s_mode == COMMUNICATION_NO_STOP)
    {
        if (isExecution && s_autoEnabled)
        {
            const uint32_t now = HAL_GetTick();

            /* confronto tick in modo robusto (wrap-around safe) */
            if ((int32_t)(now - s_nextAutoToggleTick) >= 0)
            {
                toggle_continua();
                s_nextAutoToggleTick = now + 1000U;
            }
        }
    }
    else if (s_mode == COMMUNICATION_BUTTON_RESTART)
    {
        /* niente qui: il toggle lo fa la callback del bottone */
    }
    else /* COMMUNICATION_ONE_RUN */
    {
        /* niente: stampa già gestita sopra, qui non sblocchiamo mai */
    }
}

/* ------------------ Callback IRQ bottone ------------------ */
/* Deve esistere UNA SOLA definizione nel progetto */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin != UserButton_Pin)
        return;

    if (s_mode == COMMUNICATION_NO_STOP)
    {
        /* primo click: ferma l'automatismo */
        if (s_autoEnabled)
        {
            s_autoEnabled = 0;
        }
        /* se vuoi toggle stop/continue, puoi riattivare qui */
    }
    else if (s_mode == COMMUNICATION_BUTTON_RESTART)
    {
        /* Toggle manuale: sblocca quando sei in Execution */
        toggle_continua();
    }
    else /* COMMUNICATION_ONE_RUN */
    {
        /* ignora il bottone */
        return;
    }
}
