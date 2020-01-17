/*
Steps for config timer:
	- Select timer and chanel, in this case TIM3 Channel 1.
	- Select in channel PWM Generation CH1.
	- Need pulses for 20 ms frecuncy 50 Hz, to do this config prescaler (PSC - 15 bits value) and ARR.
	  In this case in clock configuration i have in APB1 Timer clocks 72 MHz.
	  72 MHz / 50 Hz = 1 440 000 -> Preescaler = 1440-1 and ARR = 1000.
	
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
