/*
	Init declaring function that start PWM, in this case with timer 3 (chanel 1):
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
*/

#include "servos.h"

void PositionServo(uint8_t angle)
{
	htim3.Instance->CCR1 = angle;
}

void ContinuousServo(uint8_t init, uint8_t finish)
{
	for(uint8_t i=init; i<finish; i++)
	{
		htim3.Instance->CCR1 = i;
		HAL_Delay(100);
	}
}
