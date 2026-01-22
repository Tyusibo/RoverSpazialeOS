#include "redLeds_functions.h"

#include "cmsis_os2.h"          // per osTimerStart/Stop
#include "lights_init.h"        // #include "a4wd3_led.h"

extern osTimerId_t toggleLeftRedLedHandle;
extern osTimerId_t toggleRightRedLedHandle;

#define TIMER_SW_PERIOD 500U  // in ms

/* LEFT RED LED */
void leftRedLedOff(void) {
	A4WD3_Red_Off(&led_left);
}

void leftRedLedOn(void) {
	A4WD3_Red_On(&led_left);
}

void startPeriodicLeftRedLed(void) {
	A4WD3_Red_On(&led_left);
	osTimerStart(toggleLeftRedLedHandle, TIMER_SW_PERIOD);

}

void stopPeriodicLeftRedLed(void) {
	osTimerStop(toggleLeftRedLedHandle);
}

/* RIGHT RED LED */
void rightRedLedOff(void) {
	A4WD3_Red_Off(&led_right);
}

void rightRedLedOn(void) {
	A4WD3_Red_On(&led_right);
}

void startPeriodicRightRedLed(void) {
	A4WD3_Red_On(&led_right); // opzionale
	osTimerStart(toggleRightRedLedHandle, TIMER_SW_PERIOD);
}

void stopPeriodicRightRedLed(void) {
	osTimerStop(toggleRightRedLedHandle);
}
