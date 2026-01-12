#ifndef INC_WCET_ESTIMATION_DWT_H_
#define INC_WCET_ESTIMATION_DWT_H_

#include <stdint.h>
#include "stm32g4xx.h"        // include corretto per DWT/CoreDebug/IRQn_Type ecc.
#include "stm32g4xx_hal.h"    // per HAL_RCC_GetHCLKFreq()

/* Init cycle counter */
void DWT_Init(void);

/* Misura - start/stop (ritorna cicli) */
static inline uint32_t DWT_Begin(void)
{
    __DSB();
    __ISB();
    return DWT->CYCCNT;
}

static inline uint32_t DWT_End(uint32_t start)
{
    __DSB();
    __ISB();
    return (uint32_t)(DWT->CYCCNT - start); // ok anche con overflow uint32
}

/* Conversioni */
uint32_t DWT_CyclesToUs_u32(uint32_t cycles);

/* Stampa con la tua print.h (cicli + us) */
void DWT_PrintCyclesAndUs(const char *tag, uint32_t cycles);

#endif /* INC_WCET_ESTIMATION_DWT_H_ */
