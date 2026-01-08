#include "uart_functions.h"

/* INCLUDE INUTILI SOLO PER DEBUGGING */
#include "print.h"
/*************************************/
static uint8_t receivedFlag = 0;  				// Avvenuta ricezione frame

static const uint8_t ack = 1;					// Ack
static const uint8_t nack = 0; 					// Nack

static uint8_t received_ack; 	  				// variabile di ricezione per l'ack

static UART_HandleTypeDef *current_handler; 	// Handler comunicazione

/* Init */
void setHandler(UART_HandleTypeDef *uart_handler) {
	current_handler = uart_handler;
}

/* Trasmissione */

//ritorna GPIO_PIN_RESET oppure GPIO_PIN_SET. Rispettivamente 0 o 1.
uint8_t checkRTR(void) {
	return HAL_GPIO_ReadPin(RTR_IN_GPIO_Port, RTR_IN_Pin);
}

void UART_TransmitIT(uint8_t *pData, size_t size) {
	printMsg("B2 Transmitted\n\r\r\n");
	//HAL_UART_Transmit(current_handler, pData, size, HAL_MAX_DELAY);
	HAL_UART_Transmit_IT(current_handler, pData, size);
}

/* Ricezione */

void setRTR(void) {
	HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_SET);
}

void resetRTR() {
	HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_RESET);
}

void UART_ReceiveIT(uint8_t *pData, size_t size) {
	receivedFlag = 0;             //pulisco il falg
	printMsg("B2 Wait receive\n\r");

	if (HAL_UART_Receive_IT(current_handler, pData, size) != HAL_OK) {
		printMsg("B2 RECEVE_INIT_ERR\n\r");
	}
}

uint8_t hasReceived(void) {
	return receivedFlag;
}

void UART_ReceiveAck(void) {
	received_ack = 0; // Pulizia
	UART_ReceiveIT(&received_ack, 1);
}

uint8_t UART_CheckAck(void) {
	return received_ack;
}

void UART_SendAck(void) {
	UART_TransmitIT((uint8_t*) &ack, 1);
}

void UART_SendNack(void) {
	UART_TransmitIT((uint8_t*) &nack, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == current_handler->Instance) {
		printMsg("B2 Received\n\r");
		if (receivedFlag == 0) {
			receivedFlag = 1;
		}
	}
}

