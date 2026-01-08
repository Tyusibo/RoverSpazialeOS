#ifndef INC_UARTFUNCTIONS_H_
#define INC_UARTFUNCTIONS_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32g4xx_hal.h"   // definisce UART_HandleTypeDef

/* Init */
void setHandler(UART_HandleTypeDef* uart_handler);

/* Trasmissione */

uint8_t checkRTR(void);

void UART_TransmitIT(uint8_t *pData, size_t size);

void UART_ReceiveAck(void);

uint8_t UART_CheckAck(void);

/* Ricezione */

void setRTR();

void resetRTR();

void UART_ReceiveIT(uint8_t *pData, size_t size);

uint8_t hasReceived();

void UART_SendAck(void);

void UART_SendNack(void);

#endif /* INC_UARTFUNCTIONS_H_ */
