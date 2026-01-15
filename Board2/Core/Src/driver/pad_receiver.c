#include "pad_receiver.h"
#include "i2c.h"

static BUS_RemoteController rx_buffer;   // Buffer di ricezione
static BUS_RemoteController valid_data;  // Ultimo dato valido
static volatile uint8_t rx_done = 1;     // Flag stato completamento (1=Done/Idle, 0=Busy)


// Nuova funzione pubblica chiamata dalla callback globale
void PadReceiver_RxCpltCallback(void) {
    valid_data = rx_buffer; // Aggiorna dato valido
    rx_done = 1;            // Segnala operazione conclusa
}

// Nuova funzione pubblica chiamata dalla callback di errore globale
void PadReceiver_ErrorCallback(void) {
    rx_done = 1;            // Segnala operazione conclusa (anche se errore, per non bloccare)
}

// Restituisce 1 se la richiesta è partita con successo, 0 altrimenti
uint8_t PadReceiver_Request(void) {
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY) {
        rx_done = 0; // Reset flag: inizio operazione
        
        // Avvia ricezione interrupt (Indirizzo 0x60)
        if (HAL_I2C_Master_Receive_IT(&hi2c1, 0x60, (uint8_t*) &rx_buffer, sizeof(BUS_RemoteController)) != HAL_OK) {
            rx_done = 1; // Errore avvio: ripristina stato DONE per non bloccare i loop di attesa
            return 0;    // Failed to start
        }
        return 1; // Successfully started
    } else {
        return 0; // Busy
    }
}

uint8_t PadReceiver_IsDone(void) {
    return rx_done;
}

void PadReceiver_Read(BUS_RemoteController* bus_rc) {
    bus_rc->x_lever = valid_data.x_lever;
    bus_rc->y_lever = valid_data.y_lever;
    bus_rc->buttons = valid_data.buttons;
}
