#include "uart_functions.h"

/* debug includes */
#include "print.h"
#include "debug.h"

/* --- CONFIGURAZIONE DEBUG --- */

#if VERBOSE_DEBUG_UART == 1
    #define PRINT_DBG(msg) printMsg(msg)
#else
#define PRINT_DBG(msg) ((void)0)
#endif
/* ---------------------------- */

volatile uint8_t flagRTR = 0; 					    // Flag RTR

volatile uint8_t receivedFlag = 0;  				// Avvenuta ricezione frame
volatile uint8_t errorReceiveFlag = 0; 			 // Avvenuto errore di ricezione
volatile uint8_t trasmissionStatus;

static const uint8_t ack =  0x00;					// Ack
static const uint8_t nack = 0xFF; 					// Nack

static uint8_t received_ack; 	  			// variabile di ricezione per l'ack

static UART_HandleTypeDef *current_handler; 	// Handler comunicazione

/* Handler */
void setComunicationHandler(UART_HandleTypeDef *uart_handler) {
	current_handler = uart_handler;
}

UART_HandleTypeDef* getComunicationHandler() {
	return current_handler;
}

void abortReceive() {
	HAL_UART_AbortReceive(current_handler);
	HAL_UART_AbortReceive_IT(current_handler);
}

void abortTransmit() {
	HAL_UART_AbortTransmit(current_handler);
	HAL_UART_AbortTransmit_IT(current_handler);
}

void abortTransmitAndReceive() {
	HAL_UART_Abort(current_handler);
	HAL_UART_Abort_IT(current_handler);
}

/* Trasmissione */

//ritorna GPIO_PIN_RESET oppure GPIO_PIN_SET. Rispettivamente 0 o 1.
uint8_t checkRTR(void) {
	// se rtr è 1 torna 1 ma prima metti flag a 0
	if (flagRTR == 1) {
		flagRTR = 0;
		PRINT_DBG("CHECK RTR: 1\n\r\r\n");
		return 1;
	} else {
		PRINT_DBG("CHECK RTR: 0\n\r\r\n");
		return 0;
	}
}

void UART_TransmitIT(uint8_t *pData, size_t size) {
	PRINT_DBG("B2 Transmitted\n\r\r\n");
	//HAL_UART_Transmit(current_handler, pData, size, HAL_MAX_DELAY);
	HAL_UART_Transmit_IT(current_handler, pData, size);
}

/* Ricezione */

void setRTR(void) {
	HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_SET);
	PRINT_DBG("SET RTR\n\r\r\n");
}

void resetRTR() {
	HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_RESET);
}

uint8_t UART_ReceiveIT(uint8_t *pData, size_t size) {
	//pulisco i flag
	receivedFlag = 0;
	errorReceiveFlag = 0;

	PRINT_DBG("B2 Wait receive\n\r");

	if (HAL_UART_Receive_IT(current_handler, pData, size) != HAL_OK) {
		HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET);
		PRINT_DBG("B2 RECEVE_INIT_ERR\n\r");
		return 0; //errore
	}
	return 1; //ok
}

uint8_t hasReceived(void) {
	return receivedFlag;
}

uint8_t errorReceived(void) {
	return errorReceiveFlag;
}

void UART_ReceiveAckIT(void) {
	received_ack = 0; // Pulizia
	UART_ReceiveIT(&received_ack, 1);
}

uint8_t UART_CheckAck(void) {
	if (received_ack == ack) {
		return 1;
	} else if (received_ack == nack) {
		return 0;
	} else {
		return 0;
	}
}

void UART_SendAckIT(void) {
	UART_TransmitIT((uint8_t*) &ack, 1);
}

void UART_SendNackIT(void) {
	UART_TransmitIT((uint8_t*) &nack, 1);
}

