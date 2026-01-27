#include "sync_start.h"

#include "gpio.h"  // For write pin functions

/* Pin configurati a runtime */
static GPIO_TypeDef *g_in_port  = NULL;
static uint16_t      g_in_pin   = 0;

static GPIO_TypeDef *g_out_port = NULL;
static uint16_t      g_out_pin  = 0;

/* Flag bitmasks configurati a runtime */
static uint32_t      g_flag_start = 0;
static uint32_t      g_flag_sync  = 0;
static uint32_t      g_flag_ack   = 0;

/* Oggetto EventFlags */
static osEventFlagsId_t g_flagsSync = NULL;

void Sync_Init(osEventFlagsId_t flagsSync,
               GPIO_TypeDef *in_port, uint16_t in_pin,
               GPIO_TypeDef *out_port, uint16_t out_pin,
               uint32_t flag_start, uint32_t flag_sync, uint32_t flag_ack)
{
  g_in_port  = in_port;
  g_in_pin   = in_pin;
  g_out_port = out_port;
  g_out_pin  = out_pin;
  
  g_flag_start = flag_start;
  g_flag_sync  = flag_sync;
  g_flag_ack   = flag_ack;
  
  g_flagsSync  = flagsSync;

  /* output a 0 per sicurezza */
  if (g_out_port != NULL) {
    HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_RESET);
  }
}

void Sync_WaitStart(void)
{
  if (g_flagsSync == NULL) {
    return;
  }
  uint32_t flags = osEventFlagsWait(g_flagsSync,
                                    g_flag_start,
                                    osFlagsWaitAny | osFlagsNoClear,
                                    osWaitForever);

  if ((int32_t)flags < 0) {
    osThreadTerminate(osThreadGetId());
    return;
  }

  if ((flags & g_flag_start) == 0U) {
    osThreadTerminate(osThreadGetId());
    return;
  }
}

/* ISR-safe */
void Sync_OnSyncEdgeFromISR(void)
{
  if (g_flagsSync != NULL) {
    (void)osEventFlagsSet(g_flagsSync, g_flag_sync);
  }
}

void Sync_OnAckEdgeFromISR(void)
{
  if (g_flagsSync != NULL) {
    (void)osEventFlagsSet(g_flagsSync, g_flag_ack);
  }
}

void SyncThread(void)
{
  
  if ((g_flagsSync == NULL) || (g_in_port == NULL) || (g_out_port == NULL)) {
    osThreadTerminate(osThreadGetId());
    return;
  }

#if defined(SYNC_BOARD1)
  /* MASTER:
     1) SYNC_OUT = 1
     2) aspetta ACK (EXTI -> FLAG_ACK)
     3) SYNC_OUT = 0
     4) delayUntil(t0) e set START
  */
  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_SET);

  (void)osEventFlagsWait(g_flagsSync, g_flag_ack,
                         osFlagsWaitAll, osWaitForever);

  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_RESET);

  uint32_t t0 = osKernelGetTickCount() + SYNC_K_TICKS;
  (void)osDelayUntil(t0);

  (void)osEventFlagsSet(g_flagsSync, g_flag_start);

#else /* BOARD2 */
  /* SLAVE:
     1) aspetta SYNC (EXTI -> FLAG_SYNC)
     2) ACK_OUT = 1 (impulso o livello)
     3) delayUntil(t0)
     4) ACK_OUT = 0 (se vuoi impulso)
     5) set START
  */
  (void)osEventFlagsWait(g_flagsSync, g_flag_sync,
                         osFlagsWaitAll, osWaitForever);

  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_SET);

  uint32_t t0 = osKernelGetTickCount() + SYNC_K_TICKS;
  (void)osDelayUntil(t0);

  /* impulso: abbasso dopo t0 (se preferisci livello stabile, commenta) */
  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_RESET);

  (void)osEventFlagsSet(g_flagsSync, g_flag_start);
#endif

  /* finito */
  osThreadTerminate(osThreadGetId());
}
