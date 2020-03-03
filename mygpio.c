/*################################### INCLUDES/DEFINES #####################################################*/
#include "mygpio.h"
/*################################### VARIABLES ############################################################*/

/*################################### DESCRIPTION/CONECTIONS ###############################################*/
/*
	1. Config pins as exit for leds.
		 GPIO_Pin_12 -> Green led
		 GPIO_Pin_13 -> Orange led
		 GPIO_Pin_14 -> Red led
		 GPIO_Pin_15 -> Blue led
		 2. Config pins as input, example:
		 Button user to PA0 defined as GPIO_EXTI0.
	   For know state of it use: GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); 
*/
/*################################### FUNCTION 1 ###########################################################*/
void GPIO_Output_Config(void) //configura apropiadamente la GPIO
{
	/*Configure GPIO pins: GreenLed_Pin OrangeLed_Pin RedLed_Pin BlueLed_Pin */
	GPIO_InitTypeDef puerto;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
	puerto.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	puerto.GPIO_Mode  = GPIO_Mode_OUT;
	puerto.GPIO_OType = GPIO_OType_PP; 
	puerto.GPIO_Speed = GPIO_Speed_100MHz;  
	puerto.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &puerto);	
	/*Configure GPIO pin Output Level as RESET, use GPIO_SetBits() for init as SET*/
  //GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}
/*################################### FUNCTION 2 ###########################################################*/
void GPIO_Input_Config(void) 
{
	GPIO_InitTypeDef puerto;   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	puerto.GPIO_Pin  = GPIO_Pin_0; 
	puerto.GPIO_Mode = GPIO_Mode_IN; 
	puerto.GPIO_PuPd = GPIO_PuPd_NOPULL;   
	GPIO_Init(GPIOA, &puerto); 
} 
/*################################### FUNCTION 3 ###########################################################*/


/*################################### FUNCTION 4 ###########################################################*/


/*################################### FUNCTION 5 ###########################################################*/

