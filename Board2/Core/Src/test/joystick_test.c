#include "print.h"
#include "Board2.h"
#include "i2c.h"
#include "driver/pad_receiver.h"

// Test joystick: lettura via driver -> stampa -> delay -> repeat
void JoystickTest(void) {
	while (1) {

		// 1. Richiesta lettura (non bloccante)
		PadReceiver_Request();
		// 2. Attesa attiva completamento
		while (!PadReceiver_IsDone())
			;
		// 3. Lettura dati
		BUS_RemoteController rc_data;
		PadReceiver_Read(&rc_data);
		// Stampa valori
		printRemoteController(&rc_data);

		// Attesa 1 secondo (test bloccante)
		HAL_Delay(1000);
	}
}
