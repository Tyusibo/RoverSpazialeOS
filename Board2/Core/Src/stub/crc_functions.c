#include "crc_functions.h"
#include "string.h"
#include "crc.h"
#include "print.h" // Da levare se inutile in release

/* --- CONFIGURAZIONE DEBUG --- */
// 1 per abilitare le stampe, 0 per disabilitarle
#define VERBOSE_DEBUG 0 

#if VERBOSE_DEBUG == 1
    #define PRINT_DBG(msg) printMsg(msg)
    #define UART_PRINT(msg) uartPrint(msg)
#else
    #define PRINT_DBG(msg) ((void)0)
    #define UART_PRINT(msg) ((void)0)
#endif
/* ---------------------------- */

extern CRC_HandleTypeDef hcrc;


void computeCRC(uint8_t* buffer, uint32_t bufferLength){
    uint32_t crc;

    crc = HAL_CRC_Calculate(&hcrc, (uint32_t*) buffer, bufferLength);
    //crc =  0x41424344; //per rompere la board 1

    memcpy(&(buffer[bufferLength]), &crc, CRC_SIZE);
}

uint8_t compareCRC(uint8_t* buffer, uint32_t bufferLength){
    uint32_t crc_received;
    memcpy(&crc_received, &(buffer[bufferLength]), CRC_SIZE);

    uint32_t crc_computed;
    crc_computed = HAL_CRC_Calculate(&hcrc, (uint32_t*) buffer, bufferLength);

    /* Blocco stampe di debug esteso */
    #if VERBOSE_DEBUG == 1
        char msg[50];
        sprintf(msg, "CRC computed: 0x%08lX\r\n", crc_computed);
        UART_PRINT(msg);

        sprintf(msg, "CRC received: 0x%08lX\r\n", crc_received);
        UART_PRINT(msg);
    #endif

    if (crc_computed == crc_received){
        return 1;
    }
    PRINT_DBG("Mismatch\n\r");
    return 0;
}
