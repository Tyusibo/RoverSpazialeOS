#ifndef INC_SYNCHRONIZATION_SYNC_START_H_
#define INC_SYNCHRONIZATION_SYNC_START_H_

#include "cmsis_os2.h"
#include <stdint.h>

#include "actual_board.h"

/* margine di sicurezza per partire "tra poco" */
#define SYNC_K_TICKS 10u

/* Inizializza i pin usati dal sync:
 * - Board1: in = ACK_IN,  out = SYNC_OUT
 * - Board2: in = SYNC_IN, out = ACK_OUT
 */
void Sync_Init(GPIO_TypeDef *in_port, uint16_t in_pin,
               GPIO_TypeDef *out_port, uint16_t out_pin);

/* Chiamare all'inizio di OGNI task applicativo */
void Sync_WaitStart(osEventFlagsId_t flagsSync);

/* Da chiamare nei callback EXTI (HAL_GPIO_EXTI_Callback) */
void Sync_OnSyncEdgeFromISR(osEventFlagsId_t flagsSync);
void Sync_OnAckEdgeFromISR (osEventFlagsId_t flagsSync);

/* Wrapper: su Board1 setta ACK, su Board2 setta SYNC */
static inline void Sync_OnEdgeFromISR(osEventFlagsId_t flagsSync)
{
#if defined(BOARD1)
  Sync_OnAckEdgeFromISR(flagsSync);
#else
  Sync_OnSyncEdgeFromISR(flagsSync);
#endif
}

/* Thread di sincronizzazione (da creare con osThreadNew) */
void SyncThread(void *argument);

#endif /* INC_SYNCHRONIZATION_SYNC_START_H_ */
