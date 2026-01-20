#include "pad_receiver.h"
#include "i2c.h"

#define PAD_I2C_ADDRESS (0x60)

static BUS_RemoteController rx_buffer;   // Buffer di ricezione
static BUS_RemoteController valid_data;  // Ultimo dato valido
static volatile PadRxStatus_t rx_status = PAD_RX_NOT_RECEIVED;

// Restituisce PAD_OK se la richiesta è partita con successo, PAD_ERR altrimenti
int8_t PadReceiver_Request(void) {
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY) {
        rx_status = PAD_RX_IN_PROGRESS; // Reset flag: inizio operazione
        
        // Avvia ricezione interrupt (Indirizzo 0x60)
        if (HAL_I2C_Master_Receive_IT(&hi2c1, PAD_I2C_ADDRESS, (uint8_t*) &rx_buffer, sizeof(BUS_RemoteController)) != HAL_OK) {
            rx_status = PAD_RX_NOT_RECEIVED; // Errore avvio
            return PAD_ERR;    // Failed to start
        }
        return PAD_OK; // Successfully started
    } else {
        return PAD_ERR; // Busy
    }
}

PadRxStatus_t PadReceiver_GetStatus(void) {
    return rx_status;
}

// Copia l'ultimo dato valido ricevuto nella struttura del controller
void PadReceiver_Read(BUS_RemoteController* bus_rc) {
    bus_rc->x_lever = valid_data.x_lever;
    bus_rc->y_lever = valid_data.y_lever;
    bus_rc->buttons = valid_data.buttons;
}

// Nuova funzione pubblica chiamata dalla callback globale
void PadReceiver_RxCpltCallback(void) {
    valid_data = rx_buffer;      // Aggiorna dato valido
    rx_status = PAD_RX_SUCCESS;  // Segnala successo
}

// Nuova funzione pubblica chiamata dalla callback di errore globale
void PadReceiver_ErrorCallback(void) {
    rx_status = PAD_RX_ERROR;    // Segnala errore
}
