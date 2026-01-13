#ifndef INC_DRIVER_PAD_RECEIVER_H_
#define INC_DRIVER_PAD_RECEIVER_H_

#include "sensors.h"

// Avvia la richiesta di ricezione dati via Interrupt (non bloccante)
uint8_t PadReceiver_Request(void);

// Controlla se la ricezione è stata completata (1) o è in corso (0)
uint8_t PadReceiver_IsDone(void);

// Copia l'ultimo dato valido ricevuto nella struttura del controller
void PadReceiver_Read(BUS_RemoteController* data);

// Callback chiamata alla fine della ricezione (definita in pad_receiver.c)
void PadReceiver_RxCpltCallback(void);

// Callback chiamata in caso di errore di ricezione (definita in pad_receiver.c)
void PadReceiver_ErrorCallback(void);

#endif /* INC_DRIVER_PAD_RECEIVER_H_ */
