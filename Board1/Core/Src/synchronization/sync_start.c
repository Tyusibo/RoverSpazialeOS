#include "sync_start.h"
#include "main.h"
#include "cmsis_os2.h"
#include "gpio.h"
#include "event_flags_constant.h"

/* Pin configurati a runtime */
static GPIO_TypeDef *g_in_port  = NULL;
static uint16_t      g_in_pin   = 0;

static GPIO_TypeDef *g_out_port = NULL;
static uint16_t      g_out_pin  = 0;

/* flagsSync passato come argument al thread */
void Sync_Init(GPIO_TypeDef *in_port, uint16_t in_pin,
               GPIO_TypeDef *out_port, uint16_t out_pin)
{
  g_in_port  = in_port;
  g_in_pin   = in_pin;
  g_out_port = out_port;
  g_out_pin  = out_pin;

  /* output a 0 per sicurezza */
  if (g_out_port != NULL) {
    HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_RESET);
  }
}

void Sync_WaitStart(osEventFlagsId_t flagsSync)
{
  uint32_t flags = osEventFlagsWait(flagsSync,
                                    FLAG_START,
                                    osFlagsWaitAny | osFlagsNoClear,
                                    osWaitForever);

  if ((int32_t)flags < 0) {
    osThreadTerminate(osThreadGetId());
    return;
  }

  if ((flags & FLAG_START) == 0U) {
    osThreadTerminate(osThreadGetId());
    return;
  }
}

/* ISR-safe */
void Sync_OnSyncEdgeFromISR(osEventFlagsId_t flagsSync)
{
  (void)osEventFlagsSet(flagsSync, FLAG_SYNC);
}

void Sync_OnAckEdgeFromISR(osEventFlagsId_t flagsSync)
{
  (void)osEventFlagsSet(flagsSync, FLAG_ACK);
}

void SyncThread(void *argument)
{
  osEventFlagsId_t flagsSync = (osEventFlagsId_t)argument;

  if ((flagsSync == NULL) || (g_in_port == NULL) || (g_out_port == NULL)) {
    osThreadTerminate(osThreadGetId());
    return;
  }

#if defined(BOARD1)
  /* MASTER:
     1) SYNC_OUT = 1
     2) aspetta ACK (EXTI -> FLAG_ACK)
     3) SYNC_OUT = 0
     4) delayUntil(t0) e set START
  */
  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_SET);

  (void)osEventFlagsWait(flagsSync, FLAG_ACK,
                         osFlagsWaitAll, osWaitForever);

  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_RESET);

  uint32_t t0 = osKernelGetTickCount() + SYNC_K_TICKS;
  (void)osDelayUntil(t0);

  (void)osEventFlagsSet(flagsSync, FLAG_START);

#else /* BOARD2 */
  /* SLAVE:
     1) aspetta SYNC (EXTI -> FLAG_SYNC)
     2) ACK_OUT = 1 (impulso o livello)
     3) delayUntil(t0)
     4) ACK_OUT = 0 (se vuoi impulso)
     5) set START
  */
  (void)osEventFlagsWait(flagsSync, FLAG_SYNC,
                         osFlagsWaitAll, osWaitForever);

  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_SET);

  uint32_t t0 = osKernelGetTickCount() + SYNC_K_TICKS;
  (void)osDelayUntil(t0);

  /* impulso: abbasso dopo t0 (se preferisci livello stabile, commenta) */
  HAL_GPIO_WritePin(g_out_port, g_out_pin, GPIO_PIN_RESET);

  (void)osEventFlagsSet(flagsSync, FLAG_START);
#endif

  /* finito */
  osThreadTerminate(osThreadGetId());
}
