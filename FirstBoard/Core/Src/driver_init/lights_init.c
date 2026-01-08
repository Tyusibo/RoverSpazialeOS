#include "lights_init.h"
#include "gpio.h"  // Per la conoscenza delle macro definite nel .ioc

A4WD3_Led_t led_left;
A4WD3_Led_t led_right;

void led_init(void)
{
    /* Init LED sinistro */
    A4WD3_Init(&led_left,
               RedLedLeft_GPIO_Port,  RedLedLeft_Pin,
               WhiteLedLeft_GPIO_Port, WhiteLedLeft_Pin);

    /* Init LED destro */
    A4WD3_Init(&led_right,
               RedLedRight_GPIO_Port,  RedLedRight_Pin,
               WhiteLedRight_GPIO_Port, WhiteLedRight_Pin);
}


