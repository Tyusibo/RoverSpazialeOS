#include "crc_functions.h"
#include "string.h"
#include "crc.h"
#include "print.h"
#include "debug.h"

/* --- CONFIGURAZIONE DEBUG --- */

#if VERBOSE_DEBUG_CRC == 1
    #define PRINT_DBG(msg) printMsg(msg)
#else
    #define PRINT_DBG(msg) ((void)0)
#endif
/* ---------------------------- */

extern CRC_HandleTypeDef hcrc;


void computeCRC(uint8_t* buffer, uint32_t bufferLength){
    uint32_t crc;
    crc = HAL_CRC_Calculate(&hcrc, (uint32_t*) buffer, bufferLength);

    memcpy(&(buffer[bufferLength]), &crc, CRC_SIZE);
}

uint8_t compareCRC(uint8_t* buffer, uint32_t bufferLength){
    uint32_t crc_received;
    memcpy(&crc_received, &(buffer[bufferLength]), CRC_SIZE);

    uint32_t crc_computed;
    crc_computed = HAL_CRC_Calculate(&hcrc, (uint32_t*) buffer, bufferLength);

    if (crc_computed == crc_received){
        return 1;
    }

    PRINT_DBG("CRC MISMATCH\r\n");
    return 0;
}
