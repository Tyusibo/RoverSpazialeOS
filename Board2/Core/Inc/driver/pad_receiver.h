#ifndef INC_DRIVER_PAD_RECEIVER_H_
#define INC_DRIVER_PAD_RECEIVER_H_

#include "sensors.h"

// Define per return status di PadReceiver_Request
#define PAD_ERR     (-1)
#define PAD_OK      (0)

// Enum per lo stato della ricezione asincrona
typedef enum {
    PAD_RX_NOT_RECEIVED = 0, // Idle
    PAD_RX_IN_PROGRESS,      // Busy
    PAD_RX_SUCCESS,          // Done OK
    PAD_RX_ERROR             // Done Error
} PadRxStatus_t;

// Avvia la richiesta di ricezione dati via Interrupt (non bloccante)
// Restituisce PAD_OK se avviato correttamente, PAD_ERR se bus occupato o errore HAL.
int8_t PadReceiver_Request(void);

// Restituisce lo stato corrente della ricezione
PadRxStatus_t PadReceiver_GetStatus(void);

// Copia l'ultimo dato valido ricevuto nella struttura del controller
void PadReceiver_Read(BUS_RemoteController* data);

// Callback chiamata alla fine della ricezione (definita in pad_receiver.c)
void PadReceiver_RxCpltCallback(void);

// Callback chiamata in caso di errore di ricezione (definita in pad_receiver.c)
void PadReceiver_ErrorCallback(void);

#endif /* INC_DRIVER_PAD_RECEIVER_H_ */
