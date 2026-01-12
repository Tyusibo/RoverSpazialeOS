#include "DWT.h"
#include "print.h"

void DWT_Init(void)
{
    /* Abilita tracing */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Reset contatore */
    DWT->CYCCNT = 0;

    /* Abilita cycle counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t DWT_CyclesToUs_u32(uint32_t cycles)
{
    uint32_t f_cpu = HAL_RCC_GetHCLKFreq(); // Hz (HCLK)

    /* us = cycles * 1e6 / f_cpu  (con rounding, senza float) */
    uint64_t num = (uint64_t)cycles * 1000000ULL + (f_cpu / 2U);
    return (uint32_t)(num / (uint64_t)f_cpu);
}

void DWT_PrintCyclesAndUs(const char *tag, uint32_t cycles)
{
    uint32_t us = DWT_CyclesToUs_u32(cycles);

    if (tag && tag[0] != '\0')
    {
        printLabel(tag);
        printMsg(" ");
    }

    printMsg("Cy=");
    printInt((int32_t)cycles);
    printMsg(" (");
    printInt((int32_t)us);
    printMsg(" us)");
    printNewLine();
}
