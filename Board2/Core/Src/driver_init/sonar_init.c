#include "sonar_init.h"
#include "gpio.h"

extern TIM_HandleTypeDef htim2;

hcsr04_t sonarLeft, sonarFront, sonarRight;

uint8_t Sonar_InitAll(void) {
	if (hcsr04_init(&sonarLeft, SonarLeftTrig_GPIO_Port, SonarLeftTrig_Pin, &htim2,
			SONAR_LEFT_CHANNEL) != HCSR04_OK) return 1;
	if (hcsr04_init(&sonarFront, SonarFrontTrig_GPIO_Port, SonarFrontTrig_Pin, &htim2,
			SONAR_FRONT_CHANNEL) != HCSR04_OK) return 1 ;
	if (hcsr04_init(&sonarRight, SonarRightTrig_GPIO_Port, SonarRightTrig_Pin, &htim2,
			SONAR_RIGHT_CHANNEL) != HCSR04_OK) return 1;
	return 0;
}

void StartSonarTimer(void) {
    // Avvia il contatore base senza interruzioni di update (non servono per HCSR04 standard)
    HAL_TIM_Base_Start(sonarLeft.echo_tim);

    // Avvia i canali di Input Capture MA mantiene le interruzioni DISABILITATE.
    // Sarà hcsr04_trigger() ad abilitare l'interrupt specifico "on demand".
    HAL_TIM_IC_Start(sonarLeft.echo_tim, sonarLeft.echo_channel);
    HAL_TIM_IC_Start(sonarFront.echo_tim, sonarFront.echo_channel);
    HAL_TIM_IC_Start(sonarRight.echo_tim, sonarRight.echo_channel);
}

uint8_t all_sonar_done(void) {
	return (hcsr04_is_done(&sonarLeft) && hcsr04_is_done(&sonarFront)
			&& hcsr04_is_done(&sonarRight));
}
