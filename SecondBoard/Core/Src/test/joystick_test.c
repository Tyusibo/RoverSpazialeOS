#include "print.h"
#include "Board2.h"
#include "i2c.h"

// Struttura pacchetto ricevuto via I2C
typedef struct __attribute__((packed)) {
    int16_t lx;
    int16_t ly;
    int16_t rx;
    int16_t ry;
    uint32_t buttons;
} PacketData;

// Variabile globale di test
PacketData gamepad_data_test;

// Test joystick: lettura I2C -> mapping -> stampa -> delay -> repeat
void JoystickTest(void)
{
    while (1) {

        // Ricezione dati dal gamepad (ESP32 o simile)
        HAL_I2C_Master_Receive(
            &hi2c1,
            0x60,
            (uint8_t*)&gamepad_data_test,
            sizeof(PacketData),
            10
        );

        // Mapping verso struttura BUS
        Board2_U.remoteController = (BUS_RemoteController){
        	gamepad_data_test.rx,
			gamepad_data_test.ly,
            (uint16_t)gamepad_data_test.buttons
        };

        // Stampa valori joystick
        printRemoteController(&Board2_U.remoteController);

        // Attesa 1 secondo (test bloccante)
        HAL_Delay(1000);
    }
}
