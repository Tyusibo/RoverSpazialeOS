#ifndef INC_SYNCHRONIZATION_SYNC_START_H_
#define INC_SYNCHRONIZATION_SYNC_START_H_

#include <stdint.h>
#include "stm32g474xx.h"
#include "cmsis_os2.h"

#include "actual_board.h"

/* margine di sicurezza per partire "tra poco" */
#define SYNC_K_TICKS 10u

/* Inizializza i pin usati dal sync:
 * - Board1: in = ACK_IN,  out = SYNC_OUT
 * - Board2: in = SYNC_IN, out = ACK_OUT
 *
 * Configura anche i flag da usare per la sincronizzazione e l'handler
 */
void Sync_Init(osEventFlagsId_t flagsSync,
               GPIO_TypeDef *in_port, uint16_t in_pin,
               GPIO_TypeDef *out_port, uint16_t out_pin,
               uint32_t flag_start, uint32_t flag_sync, uint32_t flag_ack);

/* Chiamare all'inizio di OGNI task applicativo */
void Sync_WaitStart(void);

/* Da chiamare nei callback EXTI (HAL_GPIO_EXTI_Callback) */
void Sync_OnSyncEdgeFromISR(void);
void Sync_OnAckEdgeFromISR (void);

/* Wrapper: su Board1 setta ACK, su Board2 setta SYNC */
static inline void Sync_OnEdgeFromISR(void)
{
#if defined(SYNC_BOARD1)
  Sync_OnAckEdgeFromISR();
#else
  Sync_OnSyncEdgeFromISR();
#endif
}

/* Thread di sincronizzazione (da creare con osThreadNew) */
void SyncThread(void);

#endif /* INC_SYNCHRONIZATION_SYNC_START_H_ */
