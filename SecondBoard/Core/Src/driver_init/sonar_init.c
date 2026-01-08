#include "sonar_init.h"
#include "gpio.h"

extern TIM_HandleTypeDef htim2;

hcsr04_t sonarLeft, sonarFront, sonarRight;

void Sonar_InitAll(void) {
	if (hcsr04_init(&sonarLeft, SonarLeft_GPIO_Port, SonarLeft_Pin, &htim2,
			TIM_CHANNEL_1) != HCSR04_OK) return;
	if (hcsr04_init(&sonarFront, SonarFront_GPIO_Port, SonarFront_Pin, &htim2,
			TIM_CHANNEL_2) != HCSR04_OK) return;
	if (hcsr04_init(&sonarRight, SonarRight_GPIO_Port, SonarRight_Pin, &htim2,
			TIM_CHANNEL_4) != HCSR04_OK) return;
}

void StartSonarTimer(void) {
	HAL_TIM_Base_Start_IT(sonarLeft.echo_tim);

	HAL_TIM_IC_Start_IT(sonarLeft.echo_tim, sonarLeft.echo_channel);
	HAL_TIM_IC_Start_IT(sonarFront.echo_tim, sonarFront.echo_channel);
	HAL_TIM_IC_Start_IT(sonarRight.echo_tim, sonarRight.echo_channel);
}
