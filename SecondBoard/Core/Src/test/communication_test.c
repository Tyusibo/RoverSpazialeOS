#include "communication_test.h"

#include "main.h"     // UserButton_Pin, HAL_GetTick
#include "Board2.h"   // Board2_DW, Board2_U, Board2_B
#include "print.h"    // printGlobalState, printDecision

/* Nel tuo progetto era 2U: lo mantengo. Se cambia, aggiorna qui. */
#define Board2_IN_Execution ((uint8_T)2U)

/* ------------------ Stato interno del test ------------------ */

// Cambia nella init con quello assegnato
static volatile CommunicationMode_t s_mode = COMMUNICATION_NO_STOP;
static volatile uint8_t s_autoEnabled = 1;

static uint8_t s_prevIsExecution = 0;
static uint32_t s_nextAutoToggleTick = 0;

static void toggle_continua_only(void) {
	Board2_U.continua = (Board2_U.continua == 0) ? 1 : 0;
}

static void toggle_continua_and_remotecontroller(void) {
	if (Board2_U.continua == 0) {
		Board2_U.continua = 1;
		Board2_U.remoteController = (BUS_RemoteController ) { -3, 0, 0 };
	} else {
		Board2_U.remoteController = (BUS_RemoteController ) { 0, 5, 0 };
		Board2_U.continua = 0;
	}
}

void CommunicationTest_Init(CommunicationMode_t mode) {
	s_mode = mode;

	s_autoEnabled = 1;
	s_prevIsExecution = 0;

	s_nextAutoToggleTick = HAL_GetTick() + 1000U;

	Board2_U.continua = 0;
}

void CommunicationTest_Task(void) {
	const uint8_t isExecution =
			(Board2_DW.is_ExchangeDecision == Board2_IN_Execution) ? 1U : 0U;

	/* stampa 1 volta quando entra in Execution */
	if (isExecution && !s_prevIsExecution) {
		printGlobalState(&Board2_B.board2GlobalState);
		printDecision(&Board2_DW.board1Decision);

		s_nextAutoToggleTick = HAL_GetTick() + 1000U;
	}
	s_prevIsExecution = isExecution;

	/* avanzamento */
	if (s_mode == COMMUNICATION_NO_STOP) {
		if (isExecution && s_autoEnabled) {
			const uint32_t now = HAL_GetTick();
			if ((int32_t) (now - s_nextAutoToggleTick) >= 0) {
				/* qui NON tocchiamo remoteController: come nel tuo NO_STOP originale */
				toggle_continua_only();
				s_nextAutoToggleTick = now + 1000U;
			}
		}
	} else if (s_mode == COMMUNICATION_BUTTON_RESTART) {
		/* niente: gestito da bottone */
	} else /* COMMUNICATION_ONE_RUN */
	{
		/* niente: resta in Execution in attesa di continua (debug/manuale) */
	}
}

/* Deve esistere UNA SOLA definizione nel progetto */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin != UserButton_Pin)
		return;

	if (s_mode == COMMUNICATION_NO_STOP) {
		/* primo click: ferma l'automatismo */
		if (s_autoEnabled)
			s_autoEnabled = 0;
	} else if (s_mode == COMMUNICATION_BUTTON_RESTART) {
		/* come il tuo codice: toggle continua + set remoteController */
		toggle_continua_and_remotecontroller();
	} else /* COMMUNICATION_ONE_RUN */
	{
		/* ignora bottone */
		return;
	}
}
