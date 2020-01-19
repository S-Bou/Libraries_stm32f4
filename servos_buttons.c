
#include "servos_buttons.h"


/*########## FUNCTIONS FOR SERVOS ###############################################################*/
/*
Steps for config timer:
	- Select timer and chanel, in this case TIM3 Channel 1.
	- Select in channel PWM Generation CH1 ->X (This establishes pin PA6 as output).
	- Need pulses for 20 ms frecuency 50 Hz, to do this configure the below parameters:
          In this case in clock configuration I have in APB1 Timer clocks 72 MHz.
          72 MHz / 50 Hz = 1 440 000 -> Preescaler = 1440-1 and ARR = 1000-1.
          Prescaler (PSC - 16 bits value) = 14440-1 
          Counter Period (ARR)            = 1000-1
	- Remember declaring function that start PWM in main file, in this case: timer 3, chanel 1:
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
*/
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
/*########## FUNCTIONS FOR BUTTONS ##############################################################*/
/*	
	This function reduces the posibility for the button do multiple signals.
	In this proyect I establishes the pin PE0 as input (Pull-Up), with the user name "ButtonCicle"
*/
void ThereIsBall(void)
{
		if(HAL_GPIO_ReadPin(ButtonCicle_GPIO_Port, ButtonCicle_Pin) && tempBall)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(ButtonCicle_GPIO_Port, ButtonCicle_Pin))
		{
			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
			tempBall = false;
		}
	}
	else if(HAL_GPIO_ReadPin(ButtonCicle_GPIO_Port, ButtonCicle_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(ButtonCicle_GPIO_Port, ButtonCicle_Pin)==GPIO_PIN_RESET)
		{
			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
			//Acction
			tempBall = true;
		}
	}
}
/*########## FUNCTIONS FOR usDELAYS #############################################################*/
//void usDelay(uint32_t uSec)
//	{													//We need define timer-> ejem: #define usTIM TIM5
//	if(uSec < 2) uSec = 2;
//	usTIM->ARR = uSec - 1;		/*Sets the value in the auto-reload register*/
//	usTIM->EGR = 1;						/*Re-initialiser the timer*/
//	usTIM->SR &= ~1;					//Reset the flag
//	usTIM->CR1 |= 1;					//Enables the counter
//	while((usTIM->SR&0x0001) != 1);
//	usTIM->SR &= ~(0x0001);
//}
/*##############################################################################################*/
//void Delay_us(uint16_t us)
//{
//	__HAL_TIM_SET_COUNTER(&htim1, 0); //Set the counter value to 0
//	while(__HAL_TIM_GET_COUNTER(&htim1) < us);	//Wait for the counter to reach the us input in the parameter
//}
/*###############################################################################################*/