#include "sonar_test.h"
#include "print.h"
#include "HCSR04.h"
#include "tim.h"

// done flags (settati dalla callback al completamento misura)
static volatile uint8_t sonarLeft_done  = 0;
static volatile uint8_t sonarFront_done = 0;
static volatile uint8_t sonarRight_done = 0;

static inline uint8_t all_done(void)
{
    return (sonarLeft_done && sonarFront_done && sonarRight_done);
}

// Callback HAL: viene chiamata su ogni edge catturato
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance != TIM2) return;
//
//    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
//        uint8_t before = sonarLeft.capture_flag;
//        (void)hcsr04_read_distance(&sonarLeft);
//        if (before == 1 && sonarLeft.capture_flag == 0) sonarLeft_done = 1;
//
//    } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
//        uint8_t before = sonarFront.capture_flag;
//        (void)hcsr04_read_distance(&sonarFront);
//        if (before == 1 && sonarFront.capture_flag == 0) sonarFront_done = 1;
//
//    } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
//        uint8_t before = sonarRight.capture_flag;
//        (void)hcsr04_read_distance(&sonarRight);
//        if (before == 1 && sonarRight.capture_flag == 0) sonarRight_done = 1;
//    }
//}

void SonarTest(void)
{
    while (1) {
        // reset flags
        sonarLeft_done  = 0;
        sonarFront_done = 0;
        sonarRight_done = 0;

        // trigger tutti e 3
        (void)hcsr04_trigger(&sonarLeft);
        (void)hcsr04_trigger(&sonarFront);
        (void)hcsr04_trigger(&sonarRight);

        // polling finché non finiscono tutti e 3
        while (!all_done()) {
            // opzionale per non macinare CPU (non necessario)
            // HAL_Delay(1);
        }

        // stampa quando tutti completati
        BUS_Sonar busSonar = (BUS_Sonar){ sonarLeft.distance, sonarFront.distance, sonarRight.distance };
        printSonar(&busSonar);

        // aspetta 1 secondo e riparte
        HAL_Delay(1000);
    }
}
