#include "driver/pad_receiver.h"
#include "i2c.h"

// Struttura pacchetto dati dal gamepad (ESP32)
typedef struct __attribute__((packed)) {
    int16_t ly;       // Asse Y sinistro
    int16_t rx;       // Asse X destro
    uint16_t buttons; // Pulsanti ridotti a 16 bit
} PacketData;

static PacketData rx_buffer;   // Buffer di ricezione
static PacketData valid_data;  // Ultimo dato valido
static volatile uint8_t rx_active = 0; // Flag stato ricezione

// Callback ricezione completata
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) {
        valid_data = rx_buffer; // Aggiorna dato valido
        rx_active = 0;          // Segnala fine
    }
}

// Callback errore
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) {
        // In caso di errore NON aggiorniamo valid_data (mantiene ultimo valore)
        rx_active = 0;          // Segnala fine (per sbloccare il main)
    }
}

void PadReceiver_Request(void) {
    if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY) {
        rx_active = 1;
        // Avvia ricezione interrupt (Indirizzo 0x60)
        if (HAL_I2C_Master_Receive_IT(&hi2c1, 0x60, (uint8_t*) &rx_buffer, sizeof(PacketData)) != HAL_OK) {
            rx_active = 0; // Errore avvio, sblocca subito
        }
    } else {
        rx_active = 0; // Periferica occupata
    }
}

uint8_t PadReceiver_IsDone(void) {
    return (rx_active == 0);
}

void PadReceiver_Read(BUS_RemoteController* bus_rc) {
    bus_rc->x_lever = valid_data.rx;
    bus_rc->y_lever = valid_data.ly;
    bus_rc->buttons = valid_data.buttons;
}