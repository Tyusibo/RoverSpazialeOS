#include "print.h"
#include "Board2.h"
#include "i2c.h"
#include "driver/pad_receiver.h"

// Test joystick: lettura via driver -> stampa -> delay -> repeat
void JoystickTest(void)
{
    while (1) {

        // Ricezione dati dal gamepad tramite driver
        PadReceiver_Update(&Board2_U.remoteController);

        // Stampa valori joystick
        printRemoteController(&Board2_U.remoteController);

        // Attesa 1 secondo (test bloccante)
        HAL_Delay(1000);
    }
}
