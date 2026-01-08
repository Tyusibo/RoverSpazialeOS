#include "crc_functions.h"
#include "string.h"
#include "crc.h"


//Da levareeeeeeeeeeeeeeeeeee
#include "print.h"

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

    char msg[50];
    sprintf(msg, "CRC computed: 0x%08lX\r\n", crc_computed);
    uartPrint(msg);

    sprintf(msg, "CRC received: 0x%08lX\r\n", crc_received);
    uartPrint(msg);

    if (crc_computed == crc_received){
        return 1;
    }
    printMsg("Mismatch\n\r");
    return 0;
}
