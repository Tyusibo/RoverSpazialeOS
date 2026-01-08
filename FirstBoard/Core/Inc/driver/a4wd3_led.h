#ifndef INC_DRIVER_A4WD3_LED_H_
#define INC_DRIVER_A4WD3_LED_H_

#include "stm32g4xx_hal.h"

typedef GPIO_PinState A4WD3_LedState_t;

typedef struct {

    GPIO_TypeDef* 		  red_GPIO_Port;
    uint16_t              red_Pin;
    A4WD3_LedState_t      red_state;

    GPIO_TypeDef* 	      white_GPIO_Port;
    uint16_t              white_Pin;
    A4WD3_LedState_t      white_state;

} A4WD3_Led_t;

void A4WD3_Init(A4WD3_Led_t* dev,
                GPIO_TypeDef* red_port, uint16_t red_pin,
                GPIO_TypeDef* white_port, uint16_t white_pin);

void A4WD3_Red_Set(A4WD3_Led_t* dev, A4WD3_LedState_t state);
void A4WD3_Red_Toggle(A4WD3_Led_t* dev);

void A4WD3_White_Set(A4WD3_Led_t* dev, A4WD3_LedState_t state);
void A4WD3_White_Toggle(A4WD3_Led_t* dev);

void A4WD3_All_Off(A4WD3_Led_t* dev);
void A4WD3_All_On(A4WD3_Led_t* dev);

#endif /* INC_DRIVER_A4WD3_LED_H_ */
