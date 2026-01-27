#include "sync_start.h"
#include "main.h"
#include "cmsis_os2.h"
#include "gpio.h"

#include "event_flags_constant.h"

/* =========================
   CONFIG GPIO (DA ADATTARE)
   =========================
   Assumo che tu abbia:
   - Board1: SYNC_OUT (output) e ACK_IN (input con EXTI)
   - Board2: SYNC_IN (input con EXTI) e ACK_OUT (output)
*/


void Sync_WaitStart(osEventFlagsId_t flagsSync)
{
	uint32_t flags = osEventFlagsWait(flagsSync,
	FLAG_START,
	osFlagsWaitAny | osFlagsNoClear,
	osWaitForever);

	/* Se osEventFlagsWait fallisce ritorna un codice errore (valore negativo) */
	if ((int32_t) flags < 0) {
		osThreadTerminate(osThreadGetId());
		return;
	}

	/* Se per qualche motivo il flag non è presente (non dovrebbe accadere) */
	if ((flags & FLAG_START) == 0U) {
		osThreadTerminate(osThreadGetId());
		return;
	}

	/* OK */
	return;
}

/* ISR-safe: CMSIS-RTOS2 consente osEventFlagsSet da ISR */
//void Sync_OnSyncEdgeFromISR(osEventFlagsId_t flagsSync)
//{
//  (void)osEventFlagsSet(flagsSync, FLAG_ACK);
//}

void Sync_OnAckEdgeFromISR(osEventFlagsId_t flagsSync)
{
  (void)osEventFlagsSet(flagsSync, FLAG_SYNC);
}

/* ============ Thread di sync ============ */
void SyncThread(osEventFlagsId_t flagsSync)
{

//  /* BOARD1 (MASTER):
//     1) alza SYNC_OUT
//     2) aspetta ACK_IN (arriva via EXTI -> FLAG_ACK_RX)
//     3) dopo handshake: aspetta fino a t0 e rilascia START
//  */
//
//  /* alza SYNC_OUT */
//  HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_SET);
//
//  /* aspetta ACK dalla Board2 */
//  osEventFlagsWait(flagsSync, FLAG_ACK,
//                         osFlagsWaitAll, osWaitForever);
//
//  /* opzionale: abbassa SYNC_OUT dopo ACK */
//  HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_RESET);
//
//  /* scegli un istante futuro */
//  uint32_t t0 = osKernelGetTickCount() + SYNC_K_TICKS;
//  osDelayUntil(t0);
//
//  /* rilascia i task */
//  (void)osEventFlagsSet(flagsSync, FLAG_START);

  /* BOARD2 (SLAVE):
     1) aspetta SYNC_IN (via EXTI -> FLAG_SYNC_RX)
     2) alza ACK_OUT
     3) aspetta fino a t0 e rilascia START
  */

  /* aspetta SYNC dal master */
  osEventFlagsWait(flagsSync, FLAG_SYNC,
                         osFlagsWaitAll, osWaitForever);

  /* segnala "sono pronta" */
  HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_SET);

  /* (opzionale) lascia ACK alto o fai un impulso breve */
  // Fatto da chat
//  osDelay(1);
//  HAL_GPIO_WritePin(ACK_GPIO_Port, ACK_Pin, GPIO_PIN_RESET);

  uint32_t t0 = osKernelGetTickCount() + SYNC_K_TICKS;
  (void)osDelayUntil(t0);

  // Fatto da me
  HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_RESET);

  osEventFlagsSet(flagsSync, FLAG_START);


}
