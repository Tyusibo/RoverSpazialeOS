#ifndef INC_DRIVER_INIT_LIGHTS_INIT_H_
#define INC_DRIVER_INIT_LIGHTS_INIT_H_

#include "a4wd3_led.h"

extern A4WD3_Led_t led_left;
extern A4WD3_Led_t led_right;

/* Inizializza la mappatura dei LED (da chiamare una volta) */
void led_init(void);

#endif /* INC_DRIVER_INIT_LIGHTS_INIT_H_ */
