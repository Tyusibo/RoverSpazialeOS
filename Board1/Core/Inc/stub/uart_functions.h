#ifndef INC_STUB_UARTFUNCTIONS_H_
#define INC_STUB_UARTFUNCTIONS_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32g4xx_hal.h"   

extern volatile uint8_t receivedFlag;
extern volatile uint8_t errorReceiveFlag;
extern volatile uint8_t trasmissionStatus;


/* Handler */
void setComunicationHandler(UART_HandleTypeDef* uart_handler);

UART_HandleTypeDef * getComunicationHandler();

void abortReceive();

void abortTransmit();

void abortTransmitAndReceive();

/* Trasmissione */

uint8_t checkRTR(void);

void UART_TransmitIT(uint8_t *pData, size_t size);

void UART_ReceiveAckIT(void);

uint8_t UART_CheckAck(void);

/* Ricezione */

void setRTR();

void resetRTR();

uint8_t UART_ReceiveIT(uint8_t *pData, size_t size);

uint8_t hasReceived();

uint8_t errorReceived(void);

void UART_SendAckIT(void);

void UART_SendNackIT(void);

#endif /* INC_STUB_UARTFUNCTIONS_H_ */
