#include "lights_test.h"

#include "lights_init.h"     // ottiene le luci da testare e #include "a4wd3_led.h"

#include "stm32g4xx_hal.h" // HAL_Delay

#define LUCI_TEST_DELAY_MS 1000

void led_test_set(void)
{
    /* Assicurati di aver chiamato luci_init() nel main */

    for (int rep = 0; rep < 2; rep++)
    {
        /* --- ON (Set) --- */
        A4WD3_Red_Set(&led_left, GPIO_PIN_SET);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_Set(&led_right, GPIO_PIN_SET);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_Set(&led_left, GPIO_PIN_SET);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_Set(&led_right, GPIO_PIN_SET);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        /* --- OFF (Set) --- */
        A4WD3_White_Set(&led_left, GPIO_PIN_RESET);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_Set(&led_right, GPIO_PIN_RESET);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_Set(&led_left, GPIO_PIN_RESET);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_Set(&led_right, GPIO_PIN_RESET);
        HAL_Delay(LUCI_TEST_DELAY_MS);
    }
}

void led_test_toggle(void)
{
    /* Assicurati di aver chiamato luci_init() nel main */

    for (int rep = 0; rep < 2; rep++)
    {
        /* Red toggle */
        A4WD3_Red_Toggle(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_Toggle(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        /* White toggle */
        A4WD3_White_Toggle(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_Toggle(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);
    }
}

void led_test_all(void)
{
    /* Assicurati di aver chiamato luci_init() nel main */

    A4WD3_All_On(&led_left);
    HAL_Delay(LUCI_TEST_DELAY_MS);

    A4WD3_All_On(&led_right);
    HAL_Delay(LUCI_TEST_DELAY_MS);

    A4WD3_All_Off(&led_left);
    HAL_Delay(LUCI_TEST_DELAY_MS);

    A4WD3_All_Off(&led_right);
    HAL_Delay(LUCI_TEST_DELAY_MS);
}
