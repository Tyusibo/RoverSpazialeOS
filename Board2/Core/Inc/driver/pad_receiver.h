#ifndef INC_DRIVER_PAD_RECEIVER_H_
#define INC_DRIVER_PAD_RECEIVER_H_

#include "bus/sensors.h"

extern volatile uint8_t rx_pad_done;

// Avvia la richiesta di ricezione dati via Interrupt (non bloccante)
void PadReceiver_Request(void);

// Controlla se la ricezione è stata completata (1) o è in corso (0)
uint8_t PadReceiver_IsDone(void);

// Copia l'ultimo dato valido ricevuto nella struttura del controller
void PadReceiver_Read(BUS_RemoteController* data);

#endif /* INC_DRIVER_PAD_RECEIVER_H_ */
