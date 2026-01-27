#ifndef INC_SYNC_START_H_
#define INC_SYNC_START_H_

#include "cmsis_os2.h"
#include <stdint.h>

/* Configura questo valore: margine di sicurezza per partire "tra poco" */
#define SYNC_K_TICKS 10u

/* Chiamare all'inizio di OGNI task applicativo */
void Sync_WaitStart(osEventFlagsId_t flagsSync);

/* Da chiamare nei callback EXTI (HAL_GPIO_EXTI_Callback) */
void Sync_OnSyncEdgeFromISR(osEventFlagsId_t flagsSync);
//void Sync_OnAckEdgeFromISR(osEventFlagsId_t flagsSync);

void SyncThread(osEventFlagsId_t flagsSync);

#endif /* INC_SYNC_START_H_ */
