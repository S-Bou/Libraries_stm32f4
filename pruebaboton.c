/*############################################ INCLUDES ####################################################*/
#include "pruebaboton.h"
/*############################################ VARIABLES ###################################################*/
bool tempVar = true;
/*############################################ CONECTIONS ##################################################*/
/*
	Button user to PA0 defined as GPIO_EXTI0
*/
/*####################################### FUNCTION FOR BUTTON ##############################################*/
/*	
This function reduces the possibility for the button do multiple signals.	
You should change in header file the defines for the port and pin where is button.
*/
void ButtonUserPressed(void)
{
		if(HAL_GPIO_ReadPin(PORTBUTON, PINBUTON) && tempVar)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(PORTBUTON, PINBUTON))
		{
//			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
			tempVar = false;
		}
	}
	else if(HAL_GPIO_ReadPin(PORTBUTON, PINBUTON)==GPIO_PIN_RESET)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(PORTBUTON, PINBUTON)==GPIO_PIN_RESET)
		{
			//Acction when push the button:

		}
	}
}
/*###################################### FUNCTIONS FOR PROBE INTERRUPT #####################################*/
void pruebaEXTint(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);	//Led Red   PD14 on
	//HAL_Delay(500);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);	//Led Blue   PD15 on
	//Delay_us(50000);
	HAL_Delay(1000);
	
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);	//Led Green  PD12 off
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);	//Led Orange PD13 off
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);  //Led Red    PD14 on
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);	//Led Blue   PD15 on
}
/*###################################### FUNCTIONS FOR usDELAYS ############################################*/
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
/*##########################################################################################################*/
void Delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0); //Set the counter value to 0
	while(__HAL_TIM_GET_COUNTER(&htim1) < us);	//Wait for the counter to reach the us input in the parameter
}
/*##########################################################################################################*/

